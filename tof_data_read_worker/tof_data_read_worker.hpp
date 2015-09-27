#ifndef TOF_DATA_READ_WORKER_HPP
#define TOF_DATA_READ_WORKER_HPP

#include <QObject>
#include <QVector>
#include <QMap>
#include <QDateTime>

#include "uibk_cloud_global.h"
#include "uibk_cloud_configuration.hpp"

#include "tofwerk_dll_wrapper/data_aquisition_dll_wrapper.h"
#include "tofwerk_dll_wrapper/twtoolswrapper.h"
#include "tofwerk_dll_wrapper/shared_memory_descriptor.h"
#include "tofwerk_dll_wrapper/shared_memory_pointer.h"

typedef enum states_{IDLE, CHECK_AQ_ACTIVE, PREP_BUFFS, WAIT_F_DTA, SWAP_BUFS, PROC_DTA,FAILURE}states;

class tof_data_read_worker : public QObject
{
    Q_OBJECT
public:
    explicit tof_data_read_worker(QObject *par = 0);
    ~tof_data_read_worker();
    Q_DISABLE_COPY(tof_data_read_worker)

public slots:
//    void run_state_machine();
//    void stop_state_machine();

    void setConfig(const uibk_cloud_configuration &config);

    void save_current_background();
    void load_last_background();
    void clear_bg();

signals:
    void tof_daq_dll_error(int);
    void measurement_started();
    void measurement_stopped();

public:
    void init();

    states idle();
    //states reg_usr_data();
    states check_aqu_active(states last_state);
    states prepare_buffers();
    states prepare_buffers_bg();
    //states update_usr_data();
    states wait_for_data();
    states swap_buffers();
    states process_data();
    states record_bg_spec();

    states failure(states last_state);

    void exit();

    bool failed(int ret);
    QMap<QString, double> getGroups() const;

    QDateTime getMeantime() const;

private:
    QString get_bg_file();

private:
    data_aquisition_dll_wrapper m_dll;
    TwToolsWrapper m_tools_dll;
    shared_memory_descriptor m_desc;
    shared_memory_pointer m_ptr;

    QVector<float>* m_bg_Spectrum;

    QVector<float>* m_current_tof_Spectrum;
    QVector<float>* m_next_tof_Spectrum;

    QVector<double>* m_current_sum_Spectrum;
    QVector<double>* m_next_sum_Spectrum;

    int m_buf_index;

    QMutex m_data_mutex;
    QMutex m_config_mutex;

    int last_tofdaq_error;

    uibk_cloud_configuration m_config;

    QDateTime meantime;
    QMap<QString, double> groups;

    float bg_spect_cnt;
    float bg_fact ;

    bool measurement_running;

};

#endif // TOF_DATA_READ_WORKER_HPP
