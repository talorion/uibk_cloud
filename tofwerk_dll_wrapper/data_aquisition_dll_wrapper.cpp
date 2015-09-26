#include "data_aquisition_dll_wrapper.h"

#include <QDebug>
#include <QtGlobal>

//#include "tof_daq_specific/data_aquisition_dll_tools.h"
#include "shared_memory_descriptor.h"
#include "shared_memory_pointer.h"

#if defined( Q_OS_WIN )
#include <Windows.h>
#endif

data_aquisition_dll_wrapper::data_aquisition_dll_wrapper(QObject *par) :
    QObject(par),
     m_InitializeDll(NULL),
     m_CleanupDll(NULL),
     m_StartAcquisition(NULL),
     m_StopAcquisition(NULL),
     m_AquisitionActive(NULL),
     m_RegisterUserDat(NULL),
     m_UnregisterUserData(NULL),
     m_GetSpectrum(NULL),
     m_GetAverageSpectrum(NULL),
    m_GetTraces(NULL),
     m_GetParameter(NULL),
     m_GetParameterInt(NULL),
     m_GetParameterBool(NULL),
     m_GetParameterFloat(NULL),
     m_GetParameterInt64(NULL),
     m_GetParameterDouble(NULL),
     m_WaitForNewData(NULL),
     m_UpdateUserData(NULL),
     m_SetParameter(NULL),
     m_SetParameterInt(NULL),
     m_SetParameterBool(NULL),
     m_SetParameterFloat(NULL),
     m_SetParameterInt64(NULL),
     m_SetParameterDouble(NULL),
     m_GetDescriptor(NULL),
     m_GetSharedMemory(NULL),
     m_SetMassCalib(NULL),
     m_GetMassCalib(NULL),
     m_data_aquisition_dll(NULL),
     m_success_return(4)
{

}

data_aquisition_dll_wrapper::~data_aquisition_dll_wrapper()
{

}

void data_aquisition_dll_wrapper::init(QString dll_name)
{
    //QString dll_name = "C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll";
    m_success_return = 4;

    QFileInfo f(dll_name);
    QString absolutePath = f.absoluteDir().absolutePath();
    QString name = f.fileName();


#if defined( Q_OS_WIN )
    //LPCSTR lstr = absolutePath.toStdString().c_str();
    //LPCWSTR lstr = absolutePath.toUtf8();
    LPCWSTR str = (const wchar_t*)absolutePath.utf16();
    SetDllDirectory( str );
    qDebug()<<"add Library path: "<<absolutePath;
#endif

    m_data_aquisition_dll = new QLibrary(name);
    if (!m_data_aquisition_dll->load()){
        qDebug()<<m_data_aquisition_dll->errorString();
        return ;
    }

    QString meth = "TwInitializeDll";
    m_InitializeDll = resolve_method<InitializeDll_prototype>(meth);

    meth = "TwCleanupDll";
    m_CleanupDll= resolve_method<CleanupDll_prototype>(meth);

    meth = "TwStartAcquisition";
    m_StartAcquisition= resolve_method<StartAcquisition_prototype>(meth);

    meth = "TwStopAcquisition";
    m_StopAcquisition= resolve_method<StopAcquisition_prototype>(meth);

    meth = "TwDaqActive";
    m_AquisitionActive= resolve_method<DaqActive_prototype>(meth);

    meth = "TwRegisterUserDataBuf";
    m_RegisterUserDat= resolve_method<RegisterUserData_prototype>(meth);

    meth = "TwUnregisterUserData";
    m_UnregisterUserData= resolve_method<UnregisterUserData_prototype>(meth);

    meth = "TwGetTofSpectrumFromShMem";
    m_GetSpectrum= resolve_method<GetSpectrum_prototype>(meth);

    meth = "TwGetSumSpectrumFromShMem";
    m_GetAverageSpectrum= resolve_method<GetAverageSpectrum_prototype>(meth);

    meth = "TwGetStickSpectrumFromShMem";
    m_GetTraces= resolve_method<GetTraces_prototype>(meth);

    meth = "TwGetDaqParameter";
    m_GetParameter= resolve_method<GetParameter_prototype>(meth);

    meth = "TwGetDaqParameterInt";
    m_GetParameterInt= resolve_method<GetParameterInt_prototype>(meth);

    meth = "TwGetDaqParameterBool";
    m_GetParameterBool= resolve_method<GetParameterBool_prototype>(meth);

    meth = "TwGetDaqParameterFloat";
    m_GetParameterFloat= resolve_method<GetParameterFloat_prototype>(meth);

    meth = "TwGetDaqParameterInt64";
    m_GetParameterInt64= resolve_method<GetParameterInt64_prototype>(meth);

    meth = "TwGetDaqParameterDouble";
    m_GetParameterDouble= resolve_method<GetParameterDouble_prototype>(meth);

    meth = "TwWaitForNewData";
    m_WaitForNewData= resolve_method<WaitForNewData_prototype>(meth);

    meth = "TwUpdateUserData";
    m_UpdateUserData = resolve_method<UpdateUserData_prototype>(meth);

    meth = "TwSetDaqParameter";
    m_SetParameter = resolve_method<SetParameter_prototyper>(meth);

    meth = "TwSetDaqParameterInt";
    m_SetParameterInt = resolve_method<SetParameterInt_prototype>(meth);

    meth = "TwSetDaqParameterBool";
    m_SetParameterBool = resolve_method<SetParameterBool_prototype>(meth);

    meth = "TwSetDaqParameterFloat";
    m_SetParameterFloat = resolve_method<SetParameterFloat_prototype>(meth);

    meth = "TwSetDaqParameterInt64";
    m_SetParameterInt64 = resolve_method<SetParameterInt64_prototype>(meth);

    meth = "TwSetDaqParameterDouble";
    m_SetParameterDouble = resolve_method<SetParameterDouble_prototype>(meth);

    //GetDescriptor_prototype m_GetDescriptor;
    meth = "TwGetDescriptor";
    m_GetDescriptor = resolve_method<GetDescriptor_prototype>(meth);

    //GetSharedMemory_prototype m_GetSharedMemory;
    meth = "TwGetSharedMemory";
    m_GetSharedMemory = resolve_method<GetSharedMemory_prototype>(meth);

    //SetMassCalib_prototype m_SetMassCalib;
    meth = "TwSetMassCalib";
    m_SetMassCalib = resolve_method<SetMassCalib_prototype>(meth);

    //GetMassCalib_prototype m_GetMassCalib;
    meth = "TwGetMassCalib";
    m_GetMassCalib = resolve_method<GetMassCalib_prototype>(meth);

    initialize_dll();

}

void data_aquisition_dll_wrapper::dispose()
{
    cleanup_dll();

    m_InitializeDll = NULL;
    m_CleanupDll = NULL;
    m_StartAcquisition = NULL;
    m_StopAcquisition = NULL;
    m_AquisitionActive = NULL;
    m_RegisterUserDat = NULL;
    m_UnregisterUserData = NULL;
    m_GetSpectrum = NULL;
    m_GetAverageSpectrum = NULL;
    m_GetTraces = NULL;
    m_GetParameter = NULL;
    m_GetParameterInt = NULL;
    m_GetParameterBool = NULL;
    m_GetParameterFloat = NULL;
    m_GetParameterInt64 = NULL;
    m_GetParameterDouble = NULL;
    m_WaitForNewData = NULL;
    m_UpdateUserData = NULL;
    m_SetParameter = NULL;
    m_SetParameterInt = NULL;
    m_SetParameterBool = NULL;
    m_SetParameterFloat = NULL;
    m_SetParameterInt64 = NULL;
    m_SetParameterDouble = NULL;

    m_data_aquisition_dll->unload();
    delete m_data_aquisition_dll;
    m_data_aquisition_dll = NULL;
}

int data_aquisition_dll_wrapper::initialize_dll()
{
    if(m_InitializeDll){
        return twErrChk(m_InitializeDll());
    }
    return -1;
}

void data_aquisition_dll_wrapper::cleanup_dll()
{
    if(m_CleanupDll){
        m_CleanupDll();
    }
}

int data_aquisition_dll_wrapper::start_aquisition()
{
    if(m_StartAcquisition){
        return twErrChk(m_StartAcquisition());
    }
    return -1;
}

int data_aquisition_dll_wrapper::stop_aquisition()
{
    if(m_StopAcquisition){
        return twErrChk(m_StopAcquisition());
    }
    return -1;
}

bool data_aquisition_dll_wrapper::aquisition_active()
{
    if(m_AquisitionActive){
        return m_AquisitionActive();
    }
    return false;
}

int  data_aquisition_dll_wrapper::register_user_data(const QStringList& value_names, const QString& path , int cmp_lvl)
{
    if(m_RegisterUserDat){
        int NbrElements = value_names.size();
        QByteArray path_ba = path.toLocal8Bit();
        QByteArray desc_ba;
        QByteArray desc_elem_ba;
        foreach (QString des_elem, value_names) {
            desc_elem_ba = des_elem.toLocal8Bit();
            desc_elem_ba.resize(256);
            desc_ba.append(desc_elem_ba);
            desc_elem_ba.clear();
        }

        int ret = twErrChk(m_RegisterUserDat(path_ba.data(), NbrElements, desc_ba.data(), cmp_lvl));

        return ret;
    }
    return -1;
}

int data_aquisition_dll_wrapper::UpdateUserData( QVector<double> &Data, const QString &path)
{
    if(m_UpdateUserData){
        QByteArray path_ba = path.toLocal8Bit();
        int NbrElements = Data.size();

        return twErrChk(m_UpdateUserData(path_ba.data(),NbrElements, Data.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::unregister_user_data(const QString& path)
{
    QByteArray path_ba = path.toLocal8Bit();

    if(m_UnregisterUserData){
        int ret = twErrChk(m_UnregisterUserData(path_ba.data()));
        return ret;
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_tof_spectrum(QVector<float>& buffer_Spectrum, int BufIndex , int SegmentIndex, int SegmentEndIndex, bool Normalize)
{
    if(m_GetSpectrum){
        return twErrChk(m_GetSpectrum(buffer_Spectrum.data(), SegmentIndex, SegmentEndIndex, BufIndex, Normalize));
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_sum_spectrum(QVector<double> &buffer_avg_spectrum, bool Normalize)
{
    if(m_GetAverageSpectrum){
        return twErrChk(m_GetAverageSpectrum(buffer_avg_spectrum.data(), Normalize));
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_stick_spectrum(QVector<float>& buffer_Spectrum, QVector<float>& buffer_Masses, int BufIndex, int SegmentIndex, int SegmentEndIndex)
{
    if(m_GetTraces){
        return twErrChk(m_GetTraces(buffer_Spectrum.data(), buffer_Masses.data(), SegmentIndex, SegmentEndIndex, BufIndex));
    }
    return -1;
}

int data_aquisition_dll_wrapper::wait_for_new_data(int Timeout, shared_memory_descriptor &pBufDesc, shared_memory_pointer &pShMem, bool WaitForEventReset)
{
    if(m_WaitForNewData){
        return twErrChk(m_WaitForNewData(Timeout, pBufDesc.data(), pShMem.data(), WaitForEventReset));
    }
    return -1;
}

QString data_aquisition_dll_wrapper::read_parameter(const QString& para)
{
    if(m_GetParameter){
        QByteArray para_ba = para.toLocal8Bit();

        char* tmp = (m_GetParameter(para_ba.data()));
        return QString(tmp);
    }
    return "";
}

int data_aquisition_dll_wrapper::read_int_parameter(const QString& para)
{
    if(m_GetParameterInt){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterInt(para_ba.data()));
    }
    return -1;
}

bool data_aquisition_dll_wrapper::read_bool_parameter(const QString& para)
{
    if(m_GetParameterBool){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterBool(para_ba.data()));
    }
    return false;
}

float data_aquisition_dll_wrapper::read_float_parameter(const QString& para)
{
    if(m_GetParameterFloat){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterFloat(para_ba.data()));
    }
    return -1;
}

qint64 data_aquisition_dll_wrapper::read_int64_parameter(const QString& para)
{
    if(m_GetParameterInt64){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterInt64(para_ba.data()));
    }
    return -1;
}

double data_aquisition_dll_wrapper::read_double_parameter(const QString& para)
{
    if(m_GetParameterDouble){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterDouble(para_ba.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_descriptor(shared_memory_descriptor &pBufDesc)
{
    if(m_GetDescriptor){
        return twErrChk(m_GetDescriptor(pBufDesc.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_shared_memory(shared_memory_pointer &pShMem , bool KeepSharedMemMapped)
{
    if(m_GetSharedMemory){
        return twErrChk(m_GetSharedMemory(pShMem.data(),KeepSharedMemMapped));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_parameter(const QString &para, QString value)
{
    if(m_SetParameter){
        QByteArray para_ba = para.toLocal8Bit();
        QByteArray value_ba = value.toLocal8Bit();

        return twErrChk(m_SetParameter(para_ba.data(),value_ba.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_int_parameter(const QString &para, int value)
{
    if(m_SetParameterInt){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterInt(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_bool_parameter(const QString &para, bool value)
{
    if(m_SetParameterBool){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterBool(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_float_parameter(const QString &para, float value)
{
    if(m_SetParameterFloat){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterFloat(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_int64_parameter(const QString &para, qint64 value)
{
    if(m_SetParameterInt64){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterInt64(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_double_parameter(const QString &para, double value)
{
    if(m_SetParameterDouble){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterDouble(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_mass_calib(int &mode, QVector<double> &p, QVector<double> &mass, QVector<double> &tof, QVector<double> &weight)
{

    if(m_GetMassCalib){
        int nbrParams = p.length();
        int nbrPoints = mass.length();

        double* pmass = NULL;
        double* ptof= NULL;
        double* pweight = NULL;
        if(mass.isEmpty()){
            pmass = NULL;
            ptof= NULL;
            pweight = NULL;
        }else{
            pmass =mass.data();
            ptof =tof.data();
            pweight =weight.data();
        }
        return twErrChk(m_GetMassCalib(&mode, &nbrParams, p.data(), &nbrPoints, pmass, ptof, pweight));
    }
    return -1;
}

int data_aquisition_dll_wrapper::set_mass_calib(int mode, QVector<double> &p, QVector<double> &mass, QVector<double> &tof, QVector<double> &weight)
{
    if(m_SetMassCalib){
        int nbrParams = p.length();
        int nbrPoints = mass.length();

        if(mass.contains(0) || tof.contains(0) || weight.contains(0))
        {
            return -2;
        }
        return twErrChk(m_SetMassCalib(mode, nbrParams, p.data() ,nbrPoints, mass.data(), tof.data(), weight.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::twErrChk(int err) const
{
    if(err != 4){
        qDebug()<<"\nTWERROR"<<err;
    }
    return err;
}


