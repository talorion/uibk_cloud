#include "tof_data_read_thread.h"

#include <QTimer>
#include <QDateTime>
#include <QDebug>

#include "tof_data_read_worker.hpp"
#include "ucc_factory.h"

tof_data_read_thread::tof_data_read_thread(QObject *par) :
    QThread(par),
    mutex(),
    condition(),
    m_config(),
    restart(false),
    pause(false),
    abort(false),
    bg_meas(false)
{
    restart = false;
    abort = false;
    pause = false;

    QSettings settings;
    settings.beginGroup("tof_data_read_thread");
    QString filename = settings.value("config_file").toString();
    QString bg_filename = settings.value("bg_file", "background.dat").toString();
    settings.endGroup();

    if(!filename.isEmpty())
        start_reading(filename,bg_filename);

}

tof_data_read_thread::~tof_data_read_thread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    if(!m_config.getFilename().isEmpty()){
        QSettings settings;
        settings.beginGroup("tof_data_read_thread");
        settings.setValue("config_file",m_config.getFilename());
        settings.setValue("bg_file",m_config.getBg_filename());
        settings.endGroup();
    }

    wait();
}

void tof_data_read_thread::start_reading()
{
    if(m_config.getElements().size() >0){
        start_reading(m_config);
    }
}

void tof_data_read_thread::start_reading(const QString &fileName, const QString &bg_fileName){
    //QString fileName = "massListforZabbix.txt";
    this->m_config = ucc_factory::create_ucc_from_file(fileName);
    //qDebug()<<m_config.getElements().size();
    //m_config.setFilename(fileName);
    if(m_config.getElements().size() >0){
        m_config.setFilename(fileName);
        m_config.setBg_filename(bg_fileName);
        start_reading(m_config);
    }
}

void tof_data_read_thread::start_reading(const QString &fileName)
{
    QSettings settings;
    settings.beginGroup("tof_data_read_thread");
    //QString filename = settings.value("config_file").toString();
    QString bg_filename = settings.value("bg_file", "background.dat").toString();
    settings.endGroup();

    start_reading(fileName,bg_filename);
}


void tof_data_read_thread::start_reading(const uibk_cloud_configuration &config)
{
    QMutexLocker locker(&mutex);

    this->m_config = config;
    setBg_meas(false);
    //qDebug()<<m_config.getElements().size();
    //    this->centerX = centerX;
    //    this->centerY = centerY;
    //    this->scaleFactor = scaleFactor;
    //    this->resultSize = resultSize;



    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void tof_data_read_thread::start_reading_bg()
{
    QMutexLocker locker(&mutex);

    emit bg_measurement_started();
    setBg_meas(true);
    //this->m_config = config;
    //qDebug()<<m_config.getElements().size();
    //    this->centerX = centerX;
    //    this->centerY = centerY;
    //    this->scaleFactor = scaleFactor;
    //    this->resultSize = resultSize;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }


}

void tof_data_read_thread::pause_reading()
{
    mutex.lock();
    pause = true;
    condition.wakeOne();
    mutex.unlock();
}

void tof_data_read_thread::do_measurement_started()
{
    emit measurement_started();
}

void tof_data_read_thread::do_measurement_stopped()
{
    emit measurement_stopped();
}

void tof_data_read_thread::run()
{
    bool myoldbgs = false;
    mutex.lock();
    tof_data_read_worker wrk;
    connect(&wrk,SIGNAL(measurement_started()),this,SLOT(do_measurement_started()),Qt::QueuedConnection);
    connect(&wrk,SIGNAL(measurement_stopped()),this,SLOT(do_measurement_stopped()),Qt::QueuedConnection);
    connect(this,SIGNAL(bg_measurement_started()),&wrk,SLOT(clear_bg()),Qt::QueuedConnection);
    mutex.unlock();

    forever {
        mutex.lock();
        //wrk.setConfig(m_config);
        uibk_cloud_configuration l_config = m_config;
        bool bgs = bg_meas;

        wrk.save_current_background();
        mutex.unlock();

        states state = IDLE;
        states laststate = IDLE;
        while(state != FAILURE ){
            if (pause)
                break;
            if (restart)
                break;
            if (abort)
                return;

            switch(state){
            case IDLE:              {state =  wrk.idle(); break;}
            case CHECK_AQ_ACTIVE:   {state =  wrk.check_aqu_active(laststate); break;}
            case PREP_BUFFS:        {

                if(bgs){
                    state =  wrk.prepare_buffers_bg();
                }else{
                    wrk.setConfig(l_config);
                    wrk.init();
                    state =  wrk.prepare_buffers();
                    wrk.save_current_background();
                }
                break;
            }
            case WAIT_F_DTA:        {state =  wrk.wait_for_data(); break;}
            case SWAP_BUFS:         {state =  wrk.swap_buffers(); break;}
            case PROC_DTA:          {

                if(bgs){
                    if(myoldbgs != bgs)
                    {
                        wrk.clear_bg();

                    }
                    state = wrk.record_bg_spec();
                }else{
                    state =  wrk.process_data();
                    if(state != FAILURE){
                        QDateTime t                     = wrk.getMeantime();
                        QMap<QString, double> dta   = wrk.getGroups();
                        qDebug() << "(tof_data_read_thread::run()) Emiting data_processed()";
                        emit data_processed(t, dta);
                    }
                }
                myoldbgs = bgs;
                break;
            }
                //case FAILURE:           {state =  wrk.failure(state); break;}
            default:{break;}
            }
            if(state == FAILURE)
                wrk.failure(laststate);

        }

        mutex.lock();
        pause   = false;
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();

    }
}
bool tof_data_read_thread::getBg_meas() const
{
    return bg_meas;
}

void tof_data_read_thread::setBg_meas(bool value)
{
    if(value!=bg_meas){
        bg_meas = value;
        emit bg_meas_state_changed(bg_meas);
    }
}




