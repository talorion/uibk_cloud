#include "tof_data_export_worker.h"
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QTextStream>

tof_data_export_worker::tof_data_export_worker(QObject *par) :
    QObject(par),
    values(),
    meantimes(),
    export_timer(),
    last_averages(),
    last_avrg_time(),
    averages(),
    avrg_time(),
    last_export_state(false),
    flod()
{
    connect(&export_timer,SIGNAL(timeout()), this, SLOT(timer_tick()));

}

tof_data_export_worker::~tof_data_export_worker()
{
save_folder();
}

void tof_data_export_worker::receive_data(QDateTime mtime, groups_t groups)
{
    //qDebug()<<meantime<<groups;

    meantimes.append(mtime);
    groups_t::iterator it;
    QMap<QString, QList<double> >::iterator vit;
    for(it = groups.begin(); it != groups.end(); ++it){
        vit = values.find( it.key());
        if(vit == values.end()){
            vit =values.insert(it.key(),QList<double>());
            // continue;
        }
        vit.value().append(it.value());
    }
    calculate_averages();
}

void tof_data_export_worker::set_export_interval(int msec)
{
    export_timer.setInterval(msec);
}

int tof_data_export_worker::get_export_interval()
{
    return export_timer.interval();
}

void tof_data_export_worker::measurement_stopped()
{
    bool tmp = export_timer.isActive();
    disabele_exporting();
    last_export_state = tmp;
}

void tof_data_export_worker::measurement_started()
{
    if(last_export_state){
        enable_exporting();
    }
}

void tof_data_export_worker::timer_tick()
{
    //
    if(averages.isEmpty())
        export_nans();
    else
        export_data();
    reset_data();
}

void tof_data_export_worker::enable_exporting()
{
    export_timer.start();
}

void tof_data_export_worker::disabele_exporting()
{
    last_export_state = false;
    export_timer.stop();
}

void tof_data_export_worker::reset_data()
{
    averages.clear();
    avrg_time.setTime_t(0);
}

void tof_data_export_worker::export_data()
{
    qDebug()<<"tof_data_export_worker::timer_tick";
    QString filename = generate_filename();
    QFile data(filename);
    QString delimiter = ";";
    QString newline = "\n";
    if (!data.open(QFile::WriteOnly | QFile::Append )){
        qDebug()<<QString("Cannot write file %1:\n%2.").arg(filename).arg(data.errorString());
        return;
    }

    QTextStream output(&data);

    output<<QString::number(avrg_time.toMSecsSinceEpoch())<<delimiter;
    QMap<QString, double >::iterator vit;
    for(vit = averages.begin(); vit != averages.end(); ++vit){
        output<<QString::number(vit.value());
        if(vit != averages.end()-1)
            output<<delimiter;
    }
    output<<newline;
    output.flush();

    emit data_exported(avrg_time, averages);
    //data.close();
}

void tof_data_export_worker::export_nans()
{
    //    QString filename = "file.txt";
    //    QFile data(filename);
    //    QString delimiter = ";";
    //    QString newline = "\n";
    //    if (!data.open(QFile::WriteOnly | QFile::Append )){
    //        qDebug()<<QString("Cannot read file %1:\n%2.").arg(filename).arg(data.errorString());
    //        return;
    //    }

    //    QTextStream output(&data);
    //    last_avrg_time = QDateTime::currentDateTime();
    //    output<<QString::number(last_avrg_time.toMSecsSinceEpoch())<<delimiter;
    //    QMap<QString, double >::iterator vit;
    //    for(vit = last_averages.begin(); vit != last_averages.end(); ++vit){
    //        output<<QString::number(qQNaN());
    //        if(vit != last_averages.end()-1)
    //            output<<delimiter;
    //    }
    //    output<<newline;
    //output.flush();

    //data.close();
}

void tof_data_export_worker::calculate_averages()
{
    quint64 diff = 0;
    if(meantimes.size()>1)
        diff = meantimes.first().msecsTo(meantimes.last());

    if(meantimes.size()>0){
        avrg_time = meantimes.first().addMSecs(diff/2);
        last_avrg_time = avrg_time;
    }

    QMap<QString, QList<double> >::iterator vit;
    for(vit = values.begin(); vit != values.end(); ++vit){
        double avrg = 0;
        foreach (double var, vit.value()) {
            avrg += var;
        }
        avrg = avrg/vit.value().size();
        averages[vit.key()] = avrg;
        last_averages[vit.key()] = avrg;
    }
    //qDebug()<<averages;
    last_averages = averages;

    emit new_average(avrg_time, averages);
    //timer_tick();
}

QString tof_data_export_worker::generate_filename()
{
    QSettings settings;
    settings.beginGroup("tof_data_export_worker");
    QString export_file_folder = settings.value("export_file_folder","D:\\CLOUDX\\Zabbix").toString();
    //QString bg_filename = settings.value("bg_file", "background.dat").toString();
    settings.endGroup();

    flod = export_file_folder;

    //QString export_file_folder = "C:\\Tofwerk\\Data\\Zabbix";
    //QString export_file_folder = "D:\\CLOUDX\\Zabbix";
    QDateTime now = QDateTime::currentDateTime();
    QString export_file_suffux = "Zabbix_data.txt";
    return export_file_folder+"\\"+now.toString("yyMMdd_")+export_file_suffux;
    //return "file.txt";
}

void tof_data_export_worker::save_folder()
{
    if(flod.isNull() || flod.isEmpty())
        return;

    QSettings settings;
    settings.beginGroup("tof_data_export_worker");
    //QString export_file_folder = settings.value("export_file_folder","D:\\CLOUDX\\Zabbix").toString();
    settings.setValue("export_file_folder", flod);
    //QString bg_filename = settings.value("bg_file", "background.dat").toString();
    settings.endGroup();
}

