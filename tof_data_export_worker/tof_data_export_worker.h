#ifndef TOF_DATA_EXPORT_WORKER_H
#define TOF_DATA_EXPORT_WORKER_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QDateTime>

#include "uibk_cloud_global.h"

class tof_data_export_worker : public QObject
{
    Q_OBJECT
public:
    explicit tof_data_export_worker(QObject *par = 0);
    ~tof_data_export_worker();

    void init(QStringList values);

signals:
    void new_average(QDateTime, averages_t);
    void data_exported(QDateTime, averages_t);
    void error();

public slots:
    //void init(QStringList values);

    void receive_data(QDateTime mtime, groups_t groups);
    void timer_tick();
    void enable_exporting();
    void disabele_exporting();
    void set_export_interval(int msec);
    void measurement_stopped();
    void measurement_started();

private:
    void reset_data();
    void export_data();
    void export_nans();
    void calculate_averages();

    QString generate_filename();

private:
    QMap<QString, QList<double> > values;
    QList<QDateTime> meantimes;
    QTimer export_timer;

    QMap<QString, double > last_averages;
    QDateTime last_avrg_time;

    QMap<QString, double > averages;
    QDateTime avrg_time;

    bool last_export_state;


};

#endif // TOF_DATA_EXPORT_WORKER_H
