#ifndef TWTOOLSWRAPPER_H
#define TWTOOLSWRAPPER_H

#include <QObject>
#include <QLibrary>
#include <QDebug>

typedef char*   (*TwTranslateReturnValue_prototype)(int );

typedef int     (*TwFitSinglePeak_prototype)(int , double* , double* , int , double* , double* , double* , double* , double* , double* , double* );
typedef int     (*TwFitSinglePeak2_prototype)(int , double* , double* , int , double* );
typedef double  (*TwEvalSinglePeak_prototype)(double , double* );
typedef int     (*TwMultiPeakFit_prototype)(int , double* , double* , int , double* , double* , double* , int );
typedef double  (*TwEvalMultiPeak_prototype)(double , int , double* , double* , double* );

typedef int     (*TwFitResolution_prototype)(int , double* , double* , double* , double* , double* );
typedef double  (*TwEvalResolution_prototype)(double , double , double , double );

typedef int     (*TwGetMoleculeMass_prototype)(char* , double* );
typedef int     (*TwGetIsotopePattern_prototype)(char* , double , int* , double* , double* );
typedef int     (*TwGetIsotopePattern2_prototype)(char* , double , int* , double* , double* );
typedef int     (*TwDecomposeMass_prototype)(double , double , int , double* , char* , int , int* , int* , double* , double* , int* );
typedef int     (*TwGetComposition_prototype)(int , char* , int* , double* , double* );
typedef int     (*TwNistLibrarySearch_prototype)(int , double* , double* , bool , unsigned int* );
typedef int     (*TwNistLibraryQueryResult_prototype)(int , int , int* , char* );

typedef double  (*TwTof2Mass_prototype)(double , int , double* );
typedef double  (*TwMass2Tof_prototype)(double , int , double* );
typedef int     (*TwMassCalibrate_prototype)(int , int , double* , double* , double* , int* , double* , double* , double* );
typedef int     (*TwBruteForceCalibrate_prototype)(int , double* , int , bool , int , int , double , double* , double* );
typedef int     (*TwGetMassCalibInfo_prototype)(int , char* , int* );

typedef int     (*TwEncImsCorrelateProfile_prototype)(float* , int , int* );
typedef int     (*TwEncImsSharpenProfile_prototype)(float* , int , int* , float );
typedef int     (*TwEncImsDenoiseProfile_prototype)(float* , int , int* , float , float );
typedef int     (*TwEncImsCorrelateMultiProfiles_prototype)(void* , int , int , int , float , int , int* );
typedef int     (*TwEncImsCleanup_prototype)(void);

typedef int     (*TwSiInitializeHistograms_prototype)(int , double* , double* , int* );
typedef int     (*TwSiSetProcessingOptions_prototype)(char* , double , int );
typedef int     (*TwSiProcessSpectrum_prototype)(float* , int , int , float* , float* );
typedef int     (*TwSiGetHistogram_prototype)(int , float* , unsigned int* , unsigned int* , unsigned int* , double* );
typedef int     (*TwSiGetSumHistogram_prototype)(int , float* , unsigned int* , unsigned int* , unsigned int* , double* , double , double , double , double );
typedef int     (*TwSiResetHistograms_prototype)(void);
typedef int     (*TwSiCleanup_prototype)(void);
typedef int     (*TwSiFitPhd_prototype)(int , double* , double* , double* , double* , double* );
typedef double  (*TwSiEvalPhd_prototype)(double* , double );
typedef int     (*TwSiFitRateFromPhd_prototype)(int , double* , double* , double* , double* , double* , int* , double** );

typedef int     (*TwFindTpsIp_prototype)(char* , int , int* , char* );

class TwToolsWrapper :
        public QObject
{
    Q_OBJECT
public:
    explicit TwToolsWrapper(QObject *par = 0);
    ~TwToolsWrapper();
    Q_DISABLE_COPY(TwToolsWrapper)

    int init(QString dll_name = "C:\\Tofwerk\\TofDaq_1.97_noHW\\TwToolDll.dll");
    void dispose();

public:
    Q_INVOKABLE QString translate_return_value(int ReturnValue) const;

    Q_INVOKABLE int fit_single_peak(QVector<double>& yVals, QVector<double>& xVals, int peakType, double &blOffset, double &blSlope, double &amplitude, double &fwhmLo, double &fwhmHi, double &peakPos, double &mu)const;
    Q_INVOKABLE int fit_single_peak_2(QVector<double> &yVals, QVector<double> &xVals, int peakType, double &param);
    Q_INVOKABLE double eval_single_peak(double xVal, QVector<double> &param)const;
    Q_INVOKABLE int multi_peak_fit (QVector<double>& dataX, QVector<double>& dataY, QVector<double>& mass, QVector<double>& intensity, QVector<double>& commonPar, int options)const;
    Q_INVOKABLE double eval_multi_peak (double x, QVector<double> &mass, QVector<double> &intensity, QVector<double> &commonPar)const;

    Q_INVOKABLE int fit_resolution(QVector<double> &mass, QVector<double> &resolution, double &R0, double &m0, double &dm)const;
    Q_INVOKABLE double eval_resolution(double R0, double m0, double dm, double mass)const;

    Q_INVOKABLE int get_molecule_mass(QString molecule, double& mass)const;
    //Q_INVOKABLE QScriptValue get_molecule_mass(QString molecule)const;
    Q_INVOKABLE int get_isotope_pattern(QString molecule, double abundanceLimit, QVector<double>& isoMass, QVector<double>& isoAbundance)const;
    Q_INVOKABLE int get_isotope_pattern_2(QString molecule, double abundanceLimit, QVector<double> &isoMass, QVector<double> &isoAbundance)const;
    Q_INVOKABLE int decompose_mass(double targetMass, double tolerance, QVector<double> &atomMass, QStringList atomLabel, int nbrFilters, int &elementIndex1, int &elementIndex2, double& filterMinVal, double& filterMaxVal, int& nbrCompomers)const;
    Q_INVOKABLE int get_composition(int index, QString sumFormula, double &mass, double &massError)const;
    Q_INVOKABLE int nist_library_search(int nbrSticks, QVector<double>&stickMass, QVector<double>& stickIntensity, bool interactive, unsigned int& nbrResults)const;
    Q_INVOKABLE int nist_library_query_result(int index, int propertyIndex, QString &value)const;

    Q_INVOKABLE double tof_to_mass(double tofSample, int massCalibMode, QVector<double> &p)const;
    Q_INVOKABLE double mass_to_tof(double mass, int massCalibMode, QVector<double> &p)const;
    Q_INVOKABLE int mass_calibrate(int massCalibMode, QVector<double> &mass, QVector<double> &tof, QVector<double> &weight, QVector<double> &p, QVector<double> &legacyA, QVector<double> &legacyB)const;
    Q_INVOKABLE int brute_force_calibrate(QVector<double>&spectrum, int nbrPeaks, bool extendSearch, int triggerDelay, int pulseWidth, double sampleInterval, QVector<double>&a, QVector<double>&b)const;
    Q_INVOKABLE int get_mass_calibInfo(int massCalibMode, QString &description, QVector<int> &nbrParams)const;

    Q_INVOKABLE int enc_ims_correlate_profile (QVector<float> &profile, int opMode, QVector<int> &par)const;
    Q_INVOKABLE int enc_ims_sharpen_profile (QVector<float> &profile, int opMode, QVector<int> &par, float sharpness)const;
    Q_INVOKABLE int enc_ims_denoise_profile (QVector<float> &profile, int opMode, QVector<int> &par, float denoise, float sharpness)const;
    Q_INVOKABLE int enc_ims_correlate_multi_profiles (void* data, int dataType, int nbrProfiles, int calcMethod, float correlateThreshold, int opMode, int* par)const;
    Q_INVOKABLE int enc_ims_cleanup (void)const;
    Q_INVOKABLE int si_initialize_histograms (QVector<double> &loMass, QVector<double> &hiMass, QVector<int> &specType)const;
    Q_INVOKABLE int si_set_processing_options (QString option, double value, int specType)const;
    Q_INVOKABLE int si_process_spectrum (QVector<float> &spectrum, int specType, float &blFromData, float &thrFromData)const;
    Q_INVOKABLE int si_get_histogram (int histogramIndex, QVector<float> &intensity, QVector<unsigned int> &counts, unsigned int &spectrumCount, double &meanValue)const;
    Q_INVOKABLE int si_get_sum_histogram (int specType, QVector<float> &intensity, QVector<unsigned int> &counts, unsigned int &spectrumCount, double &meanValue, double minMass, double maxMass, double minRate, double maxRate)const;
    Q_INVOKABLE int si_reset_histograms (void)const;
    Q_INVOKABLE int si_cleanup (void)const;
    Q_INVOKABLE int si_fit_phd (QVector<double> &intensity, QVector<double> &counts, double* fwhm, double* a, QVector<double> &par)const;
    Q_INVOKABLE double si_eval_phd (QVector<double> &par, double intensity)const;
    Q_INVOKABLE int si_fit_rate_from_phd (int nbrPoints, double *intensity, double *counts, double *siPar, double *rate, double *fitCounts, int *nbrNIonTraces, double **nIonTrace)const;

    Q_INVOKABLE int find_tps_ip(QString tpsSerial, int timeout,QString &hostStr)const;

private:
    template<typename T>
    T resolve_method(const QString& method_name);

private:
    QLibrary* m_data_aquisition_dll;
    QString m_dll_name;

    TwTranslateReturnValue_prototype m_TranslateReturnValue;

    TwFitSinglePeak_prototype m_FitSinglePeak;
    TwFitSinglePeak2_prototype m_FitSinglePeak2;
    TwEvalSinglePeak_prototype m_EvalSinglePeak;
    TwMultiPeakFit_prototype m_MultiPeakFit;
    TwEvalMultiPeak_prototype m_EvalMultiPeak;

    TwFitResolution_prototype m_FitResolution;
    TwEvalResolution_prototype m_EvalResolution;

    TwGetMoleculeMass_prototype m_GetMoleculeMass;
    TwGetIsotopePattern_prototype m_GetIsotopePattern;
    TwGetIsotopePattern2_prototype m_GetIsotopePattern2;
    TwDecomposeMass_prototype m_DecomposeMass;
    TwGetComposition_prototype m_GetComposition;
    TwNistLibrarySearch_prototype m_NistLibrarySearch;
    TwNistLibraryQueryResult_prototype m_NistLibraryQueryResult;

    TwTof2Mass_prototype m_Tof2Mass;
    TwMass2Tof_prototype m_Mass2Tof;
    TwMassCalibrate_prototype m_MassCalibrate;
    TwBruteForceCalibrate_prototype m_BruteForceCalibrate;
    TwGetMassCalibInfo_prototype m_GetMassCalibInfo;

    TwEncImsCorrelateProfile_prototype m_EncImsCorrelateProfile;
    TwEncImsSharpenProfile_prototype m_EncImsSharpenProfile;
    TwEncImsDenoiseProfile_prototype m_EncImsDenoiseProfile;
    TwEncImsCorrelateMultiProfiles_prototype m_EncImsCorrelateMultiProfiles;
    TwEncImsCleanup_prototype m_EncImsCleanup;

    TwSiInitializeHistograms_prototype m_SiInitializeHistograms;
    TwSiSetProcessingOptions_prototype m_SiSetProcessingOptions;
    TwSiProcessSpectrum_prototype m_SiProcessSpectrum;
    TwSiGetHistogram_prototype m_SiGetHistogram;
    TwSiGetSumHistogram_prototype m_SiGetSumHistogram;
    TwSiResetHistograms_prototype m_SiResetHistograms;
    TwSiCleanup_prototype m_SiCleanup;
    TwSiFitPhd_prototype m_SiFitPhd;
    TwSiEvalPhd_prototype m_SiEvalPhd;
    TwSiFitRateFromPhd_prototype m_SiFitRateFromPhd;

    TwFindTpsIp_prototype m_FindTpsIp;

};

template<typename T>
T TwToolsWrapper::resolve_method(const QString& method_name)
{
    if(!m_data_aquisition_dll)
        return NULL;

    if(!m_data_aquisition_dll->isLoaded())
        return NULL;

    if(method_name.trimmed().isEmpty())
        return NULL;

    QString decorated_method_name = "_"+method_name;

    T tmp = (T) m_data_aquisition_dll->resolve(decorated_method_name.toLocal8Bit());
    if (!tmp){
        qDebug()<<"Could not resolve "+method_name<<m_data_aquisition_dll->errorString();
        return NULL;
    }
    return tmp;
}


#endif // TWTOOLSWRAPPER_H
