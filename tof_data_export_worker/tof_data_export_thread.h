#ifndef TOF_DATA_EXPORT_THREAD_H
#define TOF_DATA_EXPORT_THREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QDateTime>
#include "uibk_cloud_global.h"

class tof_data_export_thread : public QThread
{
    Q_OBJECT
public:
    explicit tof_data_export_thread(QObject *par = 0);
    ~tof_data_export_thread();

signals:
    void receive_data(QDateTime meantime, groups_t groups);
    void enable_exporting();
    void disabele_exporting();
    void measurement_stopped();
    void measurement_started();

public slots:

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;
};

#endif // TOF_DATA_EXPORT_THREAD_H
