#include "tof_data_export_thread.h"

#include "tof_data_export_worker.h"

#include <QSettings>

tof_data_export_thread::tof_data_export_thread(QObject *par) :
    QThread(par)
{

}

tof_data_export_thread::~tof_data_export_thread()
{

}

int tof_data_export_thread::get_export_interval()
{
    QSettings settings;
    settings.beginGroup("tof_data_export_thread");
    int export_interval = settings.value("export_interval",300000).toInt();
    //QString bg_filename = settings.value("bg_file", "background.dat").toString();
    settings.endGroup();

    return export_interval;

}

int tof_data_export_thread::save_export_interval(int interval)
{
    QSettings settings;
    settings.beginGroup("tof_data_export_thread");
    settings.setValue("export_interval",interval);
    settings.endGroup();

    return interval;
}

void tof_data_export_thread::run()
{
    tof_data_export_worker wrk;
    connect(this,SIGNAL(receive_data(QDateTime,QMap<QString,double>)),&wrk,SLOT(receive_data(QDateTime,QMap<QString,double>)));
    connect(this,SIGNAL(enable_exporting()),&wrk,SLOT(enable_exporting()));
    connect(this,SIGNAL(disabele_exporting()),&wrk,SLOT(disabele_exporting()));
    //connect(this,SIGNAL(measurement_stopped()),&wrk,SLOT(measurement_stopped()));
    connect(this,SIGNAL(measurement_stopped()),&wrk,SLOT(measurement_stopped()));

    connect(&wrk,SIGNAL(data_exported(QDateTime,averages_t)),this,SIGNAL(data_exported(QDateTime,averages_t)));

    wrk.disabele_exporting();
    wrk.set_export_interval(get_export_interval());
    //wrk.set_export_interval(300000);//5min
    //wrk.set_export_interval(5000);//5sec
    exec();
    //wrk.disabele_exporting();
    save_export_interval(wrk.get_export_interval());

}

