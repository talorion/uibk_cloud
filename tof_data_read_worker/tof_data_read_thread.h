#ifndef TOF_DATA_READ_THREAD_H
#define TOF_DATA_READ_THREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QMap>
#include <QTime>

#include "uibk_cloud_global.h"
#include "uibk_cloud_configuration.hpp"

class tof_data_read_thread : public QThread
{
    Q_OBJECT
public:
    explicit tof_data_read_thread(QObject *par = 0);
    ~tof_data_read_thread();
    Q_DISABLE_COPY(tof_data_read_thread)


    bool getBg_meas() const;

private:
    void setBg_meas(bool value);

public slots:
    void start_reading();
    void start_reading(const uibk_cloud_configuration &config);
    void start_reading(const QString &fileName, const QString &bg_fileName);
    void start_reading(const QString &fileName);
    void start_reading_bg();
    void pause_reading();


private slots:
    void do_measurement_started();
    void do_measurement_stopped();

signals:
    void data_processed(QDateTime meantime, groups_t groups);
    void measurement_started();
    void measurement_stopped();
    void bg_meas_state_changed(bool);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    QMutex mutex;
    QWaitCondition condition;

    uibk_cloud_configuration m_config;

    bool restart;
    bool pause;
    bool abort;

    bool bg_meas;
};

#endif // TOF_DATA_READ_THREAD_H
