#include "uibk_cloud_mainwindow.hpp"
#include <QApplication>
#include <QObject>
#include <QDebug>

#include "tof_data_read_worker/tof_data_read_thread.h"

#include "tof_data_export_worker/tof_data_export_thread.h"

#include "ucc_factory.h"

#include "uibk_cloud_global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<groups_t>("groups_t");
    qRegisterMetaType<averages_t>("averages_t");

    QCoreApplication::setOrganizationName("uibk_cloud");
    QCoreApplication::setOrganizationDomain("uibk.ac.at");
    QCoreApplication::setApplicationName("uibk_cloud");
    QCoreApplication::setApplicationVersion("0.0.1");

    //uibk_cloud_configuration tmp = ucc_factory::create_dummy_ucc();
//    QString fileName = "massListforZabbix.txt";
//    uibk_cloud_configuration tmp = ucc_factory::create_ucc_from_file(fileName);
//    qDebug()<<tmp.getElements().size();

    tof_data_read_thread tdrt;
    QObject::connect(&a,SIGNAL(aboutToQuit()),&tdrt,SLOT(quit()));
    //tdrt.start_reading(tmp);

    tof_data_export_thread tdet;
    QObject::connect(&a,SIGNAL(aboutToQuit()),&tdet,SLOT(quit()));
    QObject::connect(&tdrt,SIGNAL(data_processed(QDateTime,QMap<QString,double>)),&tdet,SIGNAL(receive_data(QDateTime,QMap<QString,double>)));
    QObject::connect(&tdrt,SIGNAL(measurement_stopped()),&tdet,SIGNAL(measurement_stopped()));
    QObject::connect(&tdrt,SIGNAL(measurement_started()),&tdet,SIGNAL(measurement_started()));
    tdet.start();

    uibk_cloud_mainwindow w;
    QObject::connect(&tdrt,SIGNAL(data_processed(QDateTime,QMap<QString,double>)),&w,SLOT(receive_data(QDateTime,QMap<QString,double>)));
    QObject::connect(&tdrt,SIGNAL(measurement_started()),&w,SLOT(reset_graph()),Qt::QueuedConnection);
    QObject::connect(&w,SIGNAL(config_file_changed(QString)), &tdrt,SLOT(start_reading(QString)));
    QObject::connect(&w,SIGNAL(start_bg_recording()), &tdrt, SLOT(start_reading_bg()));
    QObject::connect(&w,SIGNAL(stop_bg_recording()), &tdrt, SLOT(start_reading()));
    QObject::connect(&tdrt,SIGNAL(bg_meas_state_changed(bool)),&w,SLOT(bg_meas_state_changed(bool)));
    QObject::connect(&w, SIGNAL(start_exoprt()),&tdet,SIGNAL(enable_exporting()));
    QObject::connect(&w, SIGNAL(stop_export()),&tdet,SIGNAL(disabele_exporting()));
    QObject::connect(&tdet, SIGNAL(data_exported(QDateTime,averages_t)),&w,SLOT(receive_exported_data(QDateTime,averages_t)));
    w.show();

    // read csv file:
    // ignore lines starting with #
    //  name |  min-mass |   max-mass | sens-fact(/)  |   fact(*)
    //  calculate SUM get all raw spec in min/max range sum up all values
    //  calculate SINGLE_MASS   = SUM*fact/sensfact
    //  calculate GROUP_SUM     = sum all SINGLE_MASS with same name
    //  calculate MEAN over 5min= put all GROUP_SUMs in  buffer
    //  save
    //  disable save to ZABBX

    return a.exec();
}
