#include "tof_data_export_thread.h"

#include "tof_data_export_worker.h"

tof_data_export_thread::tof_data_export_thread(QObject *par) :
    QThread(par)
{

}

tof_data_export_thread::~tof_data_export_thread()
{

}

void tof_data_export_thread::run()
{
    tof_data_export_worker wrk;
    connect(this,SIGNAL(receive_data(QDateTime,QMap<QString,double>)),&wrk,SLOT(receive_data(QDateTime,QMap<QString,double>)));
    connect(this,SIGNAL(enable_exporting()),&wrk,SLOT(enable_exporting()));
    connect(this,SIGNAL(disabele_exporting()),&wrk,SLOT(disabele_exporting()));
    //connect(this,SIGNAL(measurement_stopped()),&wrk,SLOT(measurement_stopped()));
    connect(this,SIGNAL(measurement_stopped()),&wrk,SLOT(measurement_stopped()));

    wrk.enable_exporting();
    wrk.set_export_interval(300000);//5min
    //wrk.set_export_interval(5000);//5min
    exec();
    wrk.disabele_exporting();

}

