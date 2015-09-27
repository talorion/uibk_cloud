#include "tof_data_read_worker.hpp"

#include <configuration_element.hpp>

tof_data_read_worker::tof_data_read_worker(QObject *par) :
    QObject(par),
    m_dll(),
    m_tools_dll(),
    m_desc(),
    m_ptr(),
    m_bg_Spectrum(NULL),
    m_current_tof_Spectrum(NULL),
    m_next_tof_Spectrum(NULL),
    m_current_sum_Spectrum(NULL),
    m_next_sum_Spectrum(NULL),
    m_buf_index(0),
    m_data_mutex(),
    m_config_mutex(),
    last_tofdaq_error(0),
    m_config(),
    meantime(),
    groups(),
    bg_spect_cnt(0),
    bg_fact(1),
    measurement_running(false)
{
    m_dll.init();
    m_tools_dll.init();
    //    m_dll  = new data_aquisition_dll_wrapper();
    //    m_desc = new shared_memory_descriptor();
    //    m_ptr  = new shared_memory_pointer();

    m_current_tof_Spectrum  = new QVector<float> ();
    m_next_tof_Spectrum     = new QVector<float> ();

    m_current_sum_Spectrum  = new QVector<double> ();
    m_next_sum_Spectrum     = new QVector<double> ();

    m_bg_Spectrum           = new QVector<float> ();

    m_buf_index = 0;

    measurement_running = false;
    load_last_background();
}

tof_data_read_worker::~tof_data_read_worker()
{
    save_current_background();
}

//void tof_data_read_worker::run_state_machine()
//{
//    states state = IDLE;
//    while(true){
//        switch(state){
//        case IDLE:              {state =  idle(); break;}
//        case CHECK_AQ_ACTIVE:   {state =  check_aqu_active(state); break;}
//        case PREP_BUFFS:        {state =  prepare_buffers(); break;}
//        case WAIT_F_DTA:        {state =  wait_for_data(); break;}
//        case SWAP_BUFS:         {state =  swap_buffers(); break;}
//        case PROC_DTA:         {state =  process_data(); break;}
//        case FAILURE:         {state =  failure(state); break;}
//        default:{break;}
//        }
//    }
//}

//void tof_data_read_worker::stop_state_machine()
//{

//}

//QVector<float>& tof_data_read_worker::get_buffer()
//{
//    QMutexLocker locker(&m_data_mutex);
//    //if(!m_current_tof_Spectrum)

//    return *m_current_tof_Spectrum;
//}

states tof_data_read_worker::idle()
{
    //qDebug()<<"idle";
    QThread::currentThread()->sleep(1);
    return CHECK_AQ_ACTIVE;
}

states tof_data_read_worker::check_aqu_active(states last_state)
{
    Q_UNUSED(last_state);

    //qDebug()<<"check_aqu_active";
    states ret = IDLE;
    if(m_dll.aquisition_active()){
        //        switch(last_state){
        //        case IDLE:              {return PREP_BUFFS; }
        //        case PREP_BUFFS:        {return WAIT_F_DTA; }
        //        default:{return IDLE; }
        //        }
        if(measurement_running){
            ret = WAIT_F_DTA;
        }else{
            ret = PREP_BUFFS;
            emit measurement_started();
        }
        measurement_running = true;
    }else{
        if(measurement_running){
            emit measurement_stopped();
        }
        measurement_running = false;
        ret = IDLE;
    }
    return ret;
}

states tof_data_read_worker::prepare_buffers()
{
    //qDebug()<<"prepare_buffers";
    m_current_tof_Spectrum->clear();
    m_next_tof_Spectrum->clear();

    m_current_sum_Spectrum->clear();
    m_next_sum_Spectrum->clear();

    if(failed(m_dll.get_descriptor(m_desc)))
        return FAILURE;

    int NbrSamples    = m_desc.data()->NbrSamples;
    //int NbrPeaks      = m_desc.data()->NbrPeaks;
    int NbrSegments   = m_desc.data()->NbrSegments;
    //int NbrBufs       = m_desc.data()->NbrBufs;
    //int NbrWrites     = m_desc.data()->NbrWrites;

    //    QVector<double> sum_Spectrum(NbrSamples, -2);


    m_current_tof_Spectrum->resize(NbrSamples*NbrSegments);
    m_next_tof_Spectrum->resize(NbrSamples*NbrSegments);

    m_current_sum_Spectrum->resize(NbrSamples);
    m_next_sum_Spectrum->resize(NbrSamples);

    //    QVector<float> stick_Spectrum(NbrPeaks, -2);
    //    QVector<float> Masses(NbrPeaks, -2);


    return CHECK_AQ_ACTIVE;
}

states tof_data_read_worker::prepare_buffers_bg()
{
    states ret = prepare_buffers();

    m_bg_Spectrum->clear();
    m_bg_Spectrum->resize(m_current_tof_Spectrum->size());
    m_bg_Spectrum->fill(0);
    bg_spect_cnt = 0;
    return ret;
}

states tof_data_read_worker::wait_for_data()
{
    float NbrWaveforms = m_desc.data()->NbrWaveforms;
    float TofPeriod = m_desc.data()->TofPeriod;
    float NbrSegments = m_desc.data()->NbrSegments;
    float NbrBlocks = m_desc.data()->NbrBlocks;
    int NbrBufs = m_desc.data()->NbrBufs;
    //float SingleIonSignal = m_desc.data()->SingleIonSignal;
    //float SampleInterval = m_desc.data()->SampleInterval;

    float smpletime =   NbrWaveforms \
            *TofPeriod \
            *NbrSegments \
            *NbrBlocks \
            *1.0e-9;
    int timeout = (smpletime*1000)*3; //


    //if(failed(m_dll.wait_for_new_data(timeout, m_desc, m_ptr, true)))
    if(failed(m_dll.wait_for_new_data(timeout, m_desc, m_ptr, true)))
        return IDLE;
    //QThread::currentThread()->msleep(1000);

    //    QTime end = QTime::currentTime();
    //    end.addMSecs(timeout);
    //    while(QTime::currentTime()<end){
    //        m_dll.get_descriptor(m_desc);
    //        QThread::currentThread()->msleep(10);
    //        //int tmp =(m_desc.data()->Total)%NbrBufs;
    //        int tmp =(m_desc.data()->OverallBufsProcessed)%NbrBufs;
    //        if(m_buf_index != tmp ){
    //            m_buf_index = tmp;
    //            break;
    //        }
    //    }

    //m_dll.get_shared_memory(m_ptr);

    m_buf_index = m_desc.data()->iBuf;
    if(m_buf_index ==0)
        m_buf_index = NbrBufs-1;
    else
        m_buf_index = m_buf_index-1;

    if(failed(m_dll.get_tof_spectrum(*m_next_tof_Spectrum, m_buf_index))){
        qDebug()<<"get_tof_spectrum failed "<<last_tofdaq_error;
        return FAILURE;
    }
    //    if(failed(m_dll.lock_buf(1000, m_buf_index)))
    //            qDebug()<<"lock_buf failed "<<last_tofdaq_error;

    //    if(failed(m_dll.unlock_buf(m_buf_index))){
    //        qDebug()<<"unlock_buf failed "<<last_tofdaq_error;
    //    }
    //m_next_sum_Spectrum = m_ptr.data()->TofData[m_buf_index];
    //if(failed(m_dll.get_sum_spectrum(*m_next_sum_Spectrum)))
    //   return FAILURE;


    return SWAP_BUFS;
}

states tof_data_read_worker::swap_buffers()
{
    //qDebug()<<"swap_buffers";

    QMutexLocker locker(&m_data_mutex);

    QVector<float>* temp 	= m_current_tof_Spectrum;
    m_current_tof_Spectrum  = m_next_tof_Spectrum;
    m_next_tof_Spectrum 	= temp;

    QVector<double>* temp_sum 	= m_current_sum_Spectrum;
    m_current_sum_Spectrum  = m_next_sum_Spectrum;
    m_next_sum_Spectrum 	= temp_sum;

    return PROC_DTA;
}

states tof_data_read_worker::process_data()
{
    //qDebug()<<"process_data";

    QMutexLocker locker(&m_config_mutex);

    groups.clear();

    QDateTime start = QDateTime::currentDateTime();
    //QDateTime t;
    //t.start();
    int mode = 0;
    QVector<double> p(m_desc.data()->NbrMassCalibParams);
    QVector<double> mass(16);
    QVector<double> tof(16);
    QVector<double> weight(16);
    if(failed(m_dll.get_mass_calib(mode, p, mass, tof, weight))){
        qDebug()<<"get_mass_calib failed "<<last_tofdaq_error;
        return FAILURE;
    }

    //    int NbrSamples    = m_desc.data()->NbrSamples;
    //    //int NbrPeaks      = m_desc.data()->NbrPeaks;
    //    int NbrSegments   = m_desc.data()->NbrSegments;
    //    //int NbrBufs       = m_desc.data()->NbrBufs;
    //    //int NbrWrites     = m_desc.data()->NbrWrites;

    float NbrWaveforms = m_desc.data()->NbrWaveforms;
    float TofPeriod = m_desc.data()->TofPeriod;
    float NbrSegments = m_desc.data()->NbrSegments;
    float NbrBlocks = m_desc.data()->NbrBlocks;
    float SingleIonSignal = m_desc.data()->SingleIonSignal;
    float SampleInterval = m_desc.data()->SampleInterval;
    //float TotalBufsWritten = m_desc.data()->TotalBufsWritten;

    float smpletime =   NbrWaveforms \
            *TofPeriod \
            *NbrSegments \
            *NbrBlocks \
            *1.0e-9;


    //float raw_cnt_fct = (SingleIonSignal) / (SampleInterval * 1.0e+9);
    //float raw_cnt_fct = (SingleIonSignal) ;

    //int bgsize = m_bg_Spectrum->size();
    double sum_mass = 0;
    foreach (configuration_element* var, m_config.getElements()) {

        int m_lo = (qFloor(m_tools_dll.mass_to_tof(var->getMass_lo(), mode, p)));
        int m_hi = (qCeil(m_tools_dll.mass_to_tof(var->getMass_hi(), mode, p)));
        //qDebug()<<iWrite<<var->getName()<<m_lo<<m_hi;
        if(m_lo<0)
            continue;
        if(m_hi>=m_current_tof_Spectrum->size())
            continue;

        float sum = 0;
        float bg_sum = 0;
        for(int i = m_lo; i <= m_hi; i++ ){
            sum += (m_current_tof_Spectrum->at(i)) ;
            if(bg_spect_cnt>1 ){
                bg_sum += (m_bg_Spectrum->at(i));
            }
        }

        sum_mass =  sum
                *(1.0/SingleIonSignal)
                *(SampleInterval * 1.0e+9)
                *(1.0/smpletime);
        bg_sum = bg_sum*bg_fact/bg_spect_cnt;
        sum_mass -= bg_sum;
        sum_mass*=(var->getFact()/var->getSens());


        //qDebug()<<var->getName()<<sum_mass;
        if(!groups.contains(var->getName()))
            groups.insert(var->getName(), 0);
        groups[var->getName()] = groups[var->getName()]+ sum_mass;

    }
    QDateTime end = QDateTime::currentDateTime();
    qint64 elapsed_ms = start.msecsTo(end);
    meantime = start.addMSecs((elapsed_ms)/2);

    //m_dll.unlock_buf(m_buf_index);

    return CHECK_AQ_ACTIVE;
}

states tof_data_read_worker::record_bg_spec()
{
    float NbrWaveforms = m_desc.data()->NbrWaveforms;
    float TofPeriod = m_desc.data()->TofPeriod;
    float NbrSegments = m_desc.data()->NbrSegments;
    float NbrBlocks = m_desc.data()->NbrBlocks;
    float SingleIonSignal = m_desc.data()->SingleIonSignal;
    float SampleInterval = m_desc.data()->SampleInterval;
    //float TotalBufsWritten = m_desc.data()->TotalBufsWritten;

    float bg_smpletime =   NbrWaveforms
            *TofPeriod
            *NbrSegments
            *NbrBlocks
            *1.0e-9;

    if(m_current_tof_Spectrum->size() != m_bg_Spectrum->size()){
        m_bg_Spectrum->clear();
        m_bg_Spectrum->resize(m_current_tof_Spectrum->size());
        m_bg_Spectrum->fill(0);
        bg_spect_cnt = 0;
    }

    for(int i=0; i<m_current_tof_Spectrum->size();i++){
        float tmp = m_bg_Spectrum->at(i) + m_current_tof_Spectrum->at(i);
        m_bg_Spectrum->replace(i, tmp);
    }
    bg_fact =1.0
            *(1.0/SingleIonSignal)
            *(SampleInterval * 1.0e+9)
            *(1.0/bg_smpletime);

    bg_spect_cnt++;

    return CHECK_AQ_ACTIVE;
}

states tof_data_read_worker::failure(states last_state)
{
    qDebug()<<"failure";

    Q_UNUSED(last_state);

    emit tof_daq_dll_error(last_tofdaq_error);

    return IDLE;
}

bool tof_data_read_worker::failed(int ret)
{
    if( ret != 4 ){
        last_tofdaq_error = ret;
        return true;
    }
    return false;
}
QMap<QString, double> tof_data_read_worker::getGroups() const
{
    return groups;
}
QDateTime tof_data_read_worker::getMeantime() const
{
    return meantime;
}

QString tof_data_read_worker::get_bg_file()
{
    return m_config.getBg_filename();
}

void tof_data_read_worker::setConfig(const uibk_cloud_configuration &config)
{
    QMutexLocker locker(&m_config_mutex);
    m_config = config;
    //    groups.clear();
    //    foreach (configuration_element* var, m_config.getElements()) {
    //        if(!groups.contains(var->getName())){
    //            groups.insert(var->getName(), 0);
    //        }
    //    }

}

void tof_data_read_worker::save_current_background()
{
    if(!m_bg_Spectrum)
        return;
    if(m_bg_Spectrum->size()<1)
        return;
    if(bg_spect_cnt<1)
        return;

    QString bg_file = m_config.getBg_filename();
    if(bg_file.isEmpty() || bg_file.isNull())
        return;

    QFile file(bg_file);
    if(!file.open(QIODevice::WriteOnly))
        qDebug()<<QString("Cannot write file %1:\n%2.").arg(bg_file).arg(file.errorString());
    QDataStream out(&file);   // we will serialize the data into the file
    out << bg_spect_cnt;
    out << bg_fact;
    out << (*m_bg_Spectrum);


}

void tof_data_read_worker::load_last_background()
{
    QString bg_file = m_config.getBg_filename();
    if(bg_file.isEmpty() || bg_file.isNull())
        return;

    QFile file(bg_file);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<QString("Cannot read file %1:\n%2.").arg(bg_file).arg(file.errorString());
        return;
    }

    QDataStream in(&file);    // read the data serialized from the file
    QVector<float> bg;
    float cnt;
    float fct;
    in >> cnt >>fct>> bg;

    if(!m_bg_Spectrum)
        return;

    bg_spect_cnt = cnt;
    bg_fact = fct;
    m_bg_Spectrum->clear();
    *m_bg_Spectrum = bg;
}

void tof_data_read_worker::clear_bg()
{
    m_bg_Spectrum->clear();
    m_bg_Spectrum->resize(m_current_tof_Spectrum->size());
    m_bg_Spectrum->fill(0);
    bg_spect_cnt = 0;
}

void tof_data_read_worker::init()
{
    load_last_background();
}



