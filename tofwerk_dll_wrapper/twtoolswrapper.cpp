#include "twtoolswrapper.h"

#include <QFileInfo>
#include <QDir>


#if defined( Q_OS_WIN )
#include <Windows.h>
#endif


TwToolsWrapper::TwToolsWrapper(QObject *par) :
    QObject(par),
    m_data_aquisition_dll(NULL),
    m_dll_name(),
    m_TranslateReturnValue(NULL),
    m_FitSinglePeak(NULL),
    m_FitSinglePeak2(NULL),
    m_EvalSinglePeak(NULL),
    m_MultiPeakFit(NULL),
    m_EvalMultiPeak(NULL),
    m_FitResolution(NULL),
    m_EvalResolution(NULL),
    m_GetMoleculeMass(NULL),
    m_GetIsotopePattern(NULL),
    m_GetIsotopePattern2(NULL),
    m_DecomposeMass(NULL),
    m_GetComposition(NULL),
    m_NistLibrarySearch(NULL),
    m_NistLibraryQueryResult(NULL),
    m_Tof2Mass(NULL),
    m_Mass2Tof(NULL),
    m_MassCalibrate(NULL),
    m_BruteForceCalibrate(NULL),
    m_GetMassCalibInfo(NULL),
    m_EncImsCorrelateProfile(NULL),
    m_EncImsSharpenProfile(NULL),
    m_EncImsDenoiseProfile(NULL),
    m_EncImsCorrelateMultiProfiles(NULL),
    m_EncImsCleanup(NULL),
    m_SiInitializeHistograms(NULL),
    m_SiSetProcessingOptions(NULL),
    m_SiProcessSpectrum(NULL),
    m_SiGetHistogram(NULL),
    m_SiGetSumHistogram(NULL),
    m_SiResetHistograms(NULL),
    m_SiCleanup(NULL),
    m_SiFitPhd(NULL),
    m_SiEvalPhd(NULL),
    m_SiFitRateFromPhd(NULL),
    m_FindTpsIp(NULL)
{

}

TwToolsWrapper::~TwToolsWrapper()
{

}

int TwToolsWrapper::init(QString dll_name)
{
    QFileInfo f(dll_name);
    QString name = f.fileName();

#if defined( Q_OS_WIN )
    QString absolutePath = f.absoluteDir().absolutePath();

    LPCWSTR str = (const wchar_t*)absolutePath.utf16();
    SetDllDirectory( str );
    qDebug()<<"add Library path: "<<absolutePath;
#endif

    m_data_aquisition_dll = new QLibrary(name);
    if (!m_data_aquisition_dll->load()){
        qDebug()<<m_data_aquisition_dll->errorString();
        return -1;
    }
    m_dll_name = dll_name;

    QString meth = "TwTranslateReturnValue";
    m_TranslateReturnValue = resolve_method<TwTranslateReturnValue_prototype>(meth);

    meth = "TwFitSinglePeak";
    m_FitSinglePeak = resolve_method<TwFitSinglePeak_prototype>(meth);
    meth = "TwFitSinglePeak2";
    m_FitSinglePeak2 = resolve_method<TwFitSinglePeak2_prototype>(meth);
    meth = "TwEvalSinglePeak";
    m_EvalSinglePeak = resolve_method<TwEvalSinglePeak_prototype>(meth);
    meth = "TwMultiPeakFit";
    m_MultiPeakFit = resolve_method<TwMultiPeakFit_prototype>(meth);
    meth = "TwEvalMultiPeak";
    m_EvalMultiPeak = resolve_method<TwEvalMultiPeak_prototype>(meth);

    meth = "TwFitResolution";
    m_FitResolution = resolve_method<TwFitResolution_prototype>(meth);
    meth = "TwEvalResolution";
    m_EvalResolution = resolve_method<TwEvalResolution_prototype>(meth);

    meth = "TwGetMoleculeMass";
    m_GetMoleculeMass = resolve_method<TwGetMoleculeMass_prototype>(meth);
    meth = "TwGetIsotopePattern";
    m_GetIsotopePattern = resolve_method<TwGetIsotopePattern_prototype>(meth);
    meth = "TwGetIsotopePattern2";
    m_GetIsotopePattern2 = resolve_method<TwGetIsotopePattern2_prototype>(meth);
    meth = "TwDecomposeMass";
    m_DecomposeMass = resolve_method<TwDecomposeMass_prototype>(meth);
    meth = "TwGetComposition";
    m_GetComposition = resolve_method<TwGetComposition_prototype>(meth);
    meth = "TwNistLibrarySearch";
    m_NistLibrarySearch = resolve_method<TwNistLibrarySearch_prototype>(meth);
    meth = "TwNistLibraryQueryResult";
    m_NistLibraryQueryResult = resolve_method<TwNistLibraryQueryResult_prototype>(meth);

    meth = "TwTof2Mass";
    m_Tof2Mass = resolve_method<TwTof2Mass_prototype>(meth);
    meth = "TwMass2Tof";
    m_Mass2Tof = resolve_method<TwMass2Tof_prototype>(meth);
    meth = "TwMassCalibrate";
    m_MassCalibrate = resolve_method<TwMassCalibrate_prototype>(meth);
    meth = "TwBruteForceCalibrate";
    m_BruteForceCalibrate = resolve_method<TwBruteForceCalibrate_prototype>(meth);
    meth = "TwGetMassCalibInfo";
    m_GetMassCalibInfo = resolve_method<TwGetMassCalibInfo_prototype>(meth);

    meth = "TwEncImsCorrelateProfile";
    m_EncImsCorrelateProfile = resolve_method<TwEncImsCorrelateProfile_prototype>(meth);
    meth = "TwEncImsSharpenProfile";
    m_EncImsSharpenProfile = resolve_method<TwEncImsSharpenProfile_prototype>(meth);
    meth = "TwEncImsDenoiseProfile";
    m_EncImsDenoiseProfile = resolve_method<TwEncImsDenoiseProfile_prototype>(meth);
    meth = "TwEncImsCorrelateMultiProfiles";
    m_EncImsCorrelateMultiProfiles = resolve_method<TwEncImsCorrelateMultiProfiles_prototype>(meth);
    meth = "TwEncImsCleanup";
    m_EncImsCleanup = resolve_method<TwEncImsCleanup_prototype>(meth);

    meth = "TwSiInitializeHistograms";
    m_SiInitializeHistograms = resolve_method<TwSiInitializeHistograms_prototype>(meth);
    meth = "TwSiSetProcessingOptions";
    m_SiSetProcessingOptions = resolve_method<TwSiSetProcessingOptions_prototype>(meth);
    meth = "TwSiProcessSpectrum";
    m_SiProcessSpectrum = resolve_method<TwSiProcessSpectrum_prototype>(meth);
    meth = "TwSiGetHistogram";
    m_SiGetHistogram = resolve_method<TwSiGetHistogram_prototype>(meth);
    meth = "TwSiGetSumHistogram";
    m_SiGetSumHistogram = resolve_method<TwSiGetSumHistogram_prototype>(meth);
    meth = "TwSiResetHistograms";
    m_SiResetHistograms = resolve_method<TwSiResetHistograms_prototype>(meth);
    meth = "TwSiCleanup";
    m_SiCleanup = resolve_method<TwSiCleanup_prototype>(meth);
    meth = "TwSiFitPhd";
    m_SiFitPhd = resolve_method<TwSiFitPhd_prototype>(meth);
    meth = "TwSiEvalPhd";
    m_SiEvalPhd = resolve_method<TwSiEvalPhd_prototype>(meth);
    meth = "TwSiFitRateFromPhd";
    m_SiFitRateFromPhd = resolve_method<TwSiFitRateFromPhd_prototype>(meth);

    meth = "TwFindTpsIp";
    m_FindTpsIp = resolve_method<TwFindTpsIp_prototype>(meth);


    return 0;
}

void TwToolsWrapper::dispose()
{

}

QString TwToolsWrapper::translate_return_value(int ReturnValue) const
{
    if(m_TranslateReturnValue){
        char* tmp = (m_TranslateReturnValue(ReturnValue));
        return QString(tmp);
    }
    return "";
}

int TwToolsWrapper::fit_single_peak(QVector<double>& yVals, QVector<double>& xVals, int peakType, double& blOffset, double& blSlope, double& amplitude, double& fwhmLo, double& fwhmHi, double& peakPos, double& mu) const
{
    if(m_FitSinglePeak){
        int nbrDataPoints = qMin(yVals.length(), xVals.length());
        double *p_xVals = xVals.data();
        if(xVals.empty()){
            nbrDataPoints = yVals.length();
            p_xVals = NULL;
        }

        return (m_FitSinglePeak(nbrDataPoints, yVals.data(), p_xVals, peakType, &blOffset, &blSlope, &amplitude, &fwhmLo, &fwhmHi, &peakPos, &mu));
    }
    return -1;
}

int TwToolsWrapper::fit_single_peak_2(QVector<double>& yVals, QVector<double>& xVals, int peakType, double& param)
{
    if(m_FitSinglePeak2){
        int nbrDataPoints = qMin(yVals.length(), xVals.length());
        double *p_xVals = xVals.data();
        if(xVals.empty()){
            nbrDataPoints = yVals.length();
            p_xVals = NULL;
        }
        return (m_FitSinglePeak2(nbrDataPoints, yVals.data(), p_xVals, peakType, &param));
    }
    return -1;
}

double TwToolsWrapper::eval_single_peak(double xVal, QVector<double>& param) const
{
    if(m_EvalSinglePeak){
        return (m_EvalSinglePeak(xVal, param.data()));
    }
    return -1;
}

int TwToolsWrapper::multi_peak_fit(QVector<double>& dataX, QVector<double>& dataY, QVector<double>& mass, QVector<double>& intensity, QVector<double>& commonPar, int options) const
{
    if(m_MultiPeakFit){
        int nbrDataPoints = qMin(dataX.length(), dataY.length());
        int nbrPeaks = mass.length();
        if(intensity.length() < nbrPeaks)
            intensity.resize(nbrPeaks);

        return (m_MultiPeakFit(nbrDataPoints, dataX.data(), dataY.data(), nbrPeaks, mass.data(), intensity.data(), commonPar.data(), options));
    }
    return -1;
}

double TwToolsWrapper::eval_multi_peak(double x,  QVector<double>& mass, QVector<double>& intensity, QVector<double>& commonPar) const
{
    if(m_EvalMultiPeak){
        int nbrPeaks = mass.length();
        return (m_EvalMultiPeak(x, nbrPeaks, mass.data(), intensity.data(), commonPar.data()));
    }
    return -1;
}

int TwToolsWrapper::fit_resolution(QVector<double>& mass, QVector<double>& resolution, double &R0, double &m0, double &dm) const
{
    if(m_FitResolution){
        int nbrPoints = mass.length();

        return (m_FitResolution(nbrPoints, mass.data(), resolution.data(), &R0, &m0, &dm));
    }
    return -1;
}

double TwToolsWrapper::eval_resolution(double R0, double m0, double dm, double mass) const
{
    if(m_EvalResolution){
        return (m_EvalResolution(R0, m0, dm, mass));
    }
    return -1;
}

//QScriptValue TwToolsWrapper::get_molecule_mass(QString molecule) const
//{
//    if(m_GetMoleculeMass){
//        QByteArray molecule_ba = molecule.toLocal8Bit();
//        double mass = 0;
//        int ret = (m_GetMoleculeMass(molecule_ba.data(), &mass));

//        if(engine() != 0){
//            QScriptValue val = engine()->newObject();
//            val.setProperty("TwRetVal",ret);
//            val.setProperty("mass",mass);
//            return val;
//        }
//    }
//    return QScriptValue();
//}

int TwToolsWrapper::get_molecule_mass(QString molecule, double &mass) const
{
    if(m_GetMoleculeMass){
        QByteArray molecule_ba = molecule.toLocal8Bit();
        return (m_GetMoleculeMass(molecule_ba.data(), &mass));
    }
    return -1;
}

int TwToolsWrapper::get_isotope_pattern(QString molecule, double abundanceLimit,  QVector<double>&isoMass, QVector<double>&isoAbundance) const
{
    if(m_GetIsotopePattern){
        QByteArray molecule_ba = molecule.toLocal8Bit();
        int nbrIsotopes = 0;
        //Parses the molecular formula and returns the isotope pattern.
        //Either supply sufficiently large buffers for isoMass and isoAbundance and expect a TwValueAdjusted return value or
        //call the function first with *nbrIsotopes set to 0 and NULL for isoMass and isoAbundance to get the number of isotopes generated,
        //then allocate correspondingly sized arrays and call the function again (expect TwSuccess).
        (m_GetIsotopePattern(molecule_ba.data(), abundanceLimit, &nbrIsotopes, NULL, NULL));

        if(isoMass.length()< nbrIsotopes)
            isoMass.resize(nbrIsotopes);

        if(isoAbundance.length()< nbrIsotopes)
            isoAbundance.resize(nbrIsotopes);

        return (m_GetIsotopePattern(molecule_ba.data(), abundanceLimit, &nbrIsotopes, isoMass.data(), isoAbundance.data()));
    }
    return -1;
}

int TwToolsWrapper::get_isotope_pattern_2(QString molecule, double abundanceLimit,  QVector<double>&isoMass, QVector<double>&isoAbundance) const
{
    if(m_GetIsotopePattern2){
        QByteArray molecule_ba = molecule.toLocal8Bit();
        int nbrIsotopes =0;
        //Parses the molecular formula and returns the isotope pattern.
        //Either supply sufficiently large buffers for isoMass and isoAbundance and expect a TwValueAdjusted return value or
        //call the function first with *nbrIsotopes set to 0 and NULL for isoMass and isoAbundance to get the number of isotopes generated,
        //then allocate correspondingly sized arrays and call the function again (expect TwSuccess).
        (m_GetIsotopePattern2(molecule_ba.data(), abundanceLimit, &nbrIsotopes, NULL, NULL));

        if(isoMass.length()< nbrIsotopes)
            isoMass.resize(nbrIsotopes);

        if(isoAbundance.length()< nbrIsotopes)
            isoAbundance.resize(nbrIsotopes);

        return (m_GetIsotopePattern2(molecule_ba.data(), abundanceLimit, &nbrIsotopes, isoMass.data(), isoAbundance.data()));
    }
    return -1;
}

int TwToolsWrapper::decompose_mass(double targetMass, double tolerance, QVector<double>&atomMass, QStringList atomLabel, int nbrFilters, int &elementIndex1, int &elementIndex2, double &filterMinVal, double &filterMaxVal, int &nbrCompomers) const
{
    if(m_DecomposeMass){

        int nbrAtoms = atomMass.length();
        QByteArray atomLabel_ba;
        QByteArray atomLabel_elem_ba;
        foreach (QString des_elem, atomLabel) {
            atomLabel_elem_ba = des_elem.toLocal8Bit();
            atomLabel_elem_ba.append((char)0);
            atomLabel_ba.append(atomLabel_elem_ba);
        }

        return (m_DecomposeMass(targetMass, tolerance, nbrAtoms, atomMass.data(), atomLabel_ba.data(), nbrFilters, &elementIndex1, &elementIndex2, &filterMinVal, &filterMaxVal, &nbrCompomers));
    }
    return -1;
}

int TwToolsWrapper::get_composition(int index, QString sumFormula, double &mass, double &massError) const
{
    if(m_GetComposition){
        QByteArray sumFormula_ba = sumFormula.toLocal8Bit();
        int sumFormulaLength = sumFormula.length();
        return (m_GetComposition(index, sumFormula_ba.data(), &sumFormulaLength, &mass, &massError));
    }
    return -1;
}

int TwToolsWrapper::nist_library_search(int nbrSticks, QVector<double>&stickMass, QVector<double>&stickIntensity, bool interactive, unsigned int &nbrResults) const
{
    if(m_NistLibrarySearch){
        return (m_NistLibrarySearch(nbrSticks, stickMass.data(), stickIntensity.data(), interactive, &nbrResults));
    }
    return -1;
}

int TwToolsWrapper::nist_library_query_result(int index, int propertyIndex, QString& value) const
{
    if(m_NistLibraryQueryResult){
        int *valueLen = new int;
        (m_NistLibraryQueryResult(index, propertyIndex, valueLen, NULL));

        QByteArray value_ba;
        value_ba.resize(*valueLen+1);
        int ret = (m_NistLibraryQueryResult(index, propertyIndex, valueLen, value_ba.data()));

        value = QString::fromLocal8Bit(value_ba);
        delete valueLen;
        return ret;
    }
    return -1;
}

double TwToolsWrapper::tof_to_mass(double tofSample, int massCalibMode, QVector<double>&p) const
{
    if(m_Tof2Mass){
        return (m_Tof2Mass(tofSample, massCalibMode, p.data()));
    }
    return -1;
}

double TwToolsWrapper::mass_to_tof(double mass, int massCalibMode, QVector<double>&p) const
{
    if(m_Mass2Tof){
        return (m_Mass2Tof(mass, massCalibMode, p.data()));
    }
    return -1;
}

int TwToolsWrapper::mass_calibrate(int massCalibMode,  QVector<double>&mass, QVector<double>&tof, QVector<double>&weight,  QVector<double>&p, QVector<double>&legacyA, QVector<double>&legacyB) const
{
    if(m_MassCalibrate){
        int nbrPoints  = mass.length();
        double* weight_p = weight.data();
        if(weight.isEmpty())
            weight_p = NULL;
        int nbrParams = p.length();
        double* legacyA_p = legacyA.data();
        if(legacyA.isEmpty())
            legacyA_p = NULL;
        double* legacyB_p = legacyB.data();
        if(legacyB.isEmpty())
            legacyB_p = NULL;

        return (m_MassCalibrate(massCalibMode, nbrPoints, mass.data(), tof.data(), weight_p, &nbrParams, p.data(), legacyA_p, legacyB_p));
    }
    return -1;
}

int TwToolsWrapper::brute_force_calibrate(QVector<double>&spectrum, int nbrPeaks, bool extendSearch, int triggerDelay, int pulseWidth, double sampleInterval, QVector<double>&a, QVector<double>&b) const
{
    if(m_BruteForceCalibrate){
        int specLength = spectrum.length();

        return (m_BruteForceCalibrate(specLength, spectrum.data(), nbrPeaks, extendSearch, triggerDelay, pulseWidth, sampleInterval, a.data(), b.data()));
    }
    return -1;
}

int TwToolsWrapper::get_mass_calibInfo(int massCalibMode, QString& description, QVector<int>&nbrParams) const
{
    if(m_GetMassCalibInfo){
        QByteArray description_ba =description.toLocal8Bit();
        if(description_ba.length()<64)
            description_ba.resize(64);
        int ret = (m_GetMassCalibInfo(massCalibMode, description_ba.data(), nbrParams.data()));

        description = QString::fromLocal8Bit(description_ba);
        return ret;
    }
    return -1;
}

int TwToolsWrapper::enc_ims_correlate_profile(QVector<float>&profile, int opMode, QVector<int>&par) const
{
    if(m_EncImsCorrelateProfile){
        return (m_EncImsCorrelateProfile(profile.data(), opMode, par.data()));
    }
    return -1;
}

int TwToolsWrapper::enc_ims_sharpen_profile(QVector<float>&profile, int opMode, QVector<int>&par, float sharpness) const
{
    if(m_EncImsSharpenProfile){
        return (m_EncImsSharpenProfile(profile.data(), opMode, par.data(), sharpness));
    }
    return -1;
}

int TwToolsWrapper::enc_ims_denoise_profile(QVector<float>&profile, int opMode, QVector<int>&par, float denoise, float sharpness) const
{
    if(m_EncImsDenoiseProfile){
        return (m_EncImsDenoiseProfile(profile.data(), opMode, par.data(), denoise, sharpness));
    }
    return -1;
}

int TwToolsWrapper::enc_ims_correlate_multi_profiles(void *data, int dataType, int nbrProfiles, int calcMethod, float correlateThreshold, int opMode, int *par) const
{
    if(m_EncImsCorrelateMultiProfiles){
        return (m_EncImsCorrelateMultiProfiles(data, dataType, nbrProfiles, calcMethod, correlateThreshold, opMode, par));
    }
    return -1;
}

int TwToolsWrapper::enc_ims_cleanup() const
{
    if(m_EncImsCleanup){
        return (m_EncImsCleanup());
    }
    return -1;
}

int TwToolsWrapper::si_initialize_histograms( QVector<double> &loMass, QVector<double> &hiMass, QVector<int> &specType) const
{
    if(m_SiInitializeHistograms){
        int nbrHistograms = loMass.length();

        int* specType_p = specType.data();
        if(specType.isEmpty())
            specType_p = NULL;

        return (m_SiInitializeHistograms(nbrHistograms, loMass.data(), hiMass.data(), specType_p));
    }
    return -1;
}

int TwToolsWrapper::si_set_processing_options(QString option, double value, int specType) const
{
    if(m_SiSetProcessingOptions){
        QByteArray option_ba = option.toLocal8Bit();
        return (m_SiSetProcessingOptions(option_ba.data(), value, specType));
    }
    return -1;
}

int TwToolsWrapper::si_process_spectrum(QVector<float> &spectrum,  int specType, float &blFromData, float &thrFromData) const
{
    if(m_SiProcessSpectrum){
        int nbrSamples = spectrum.length();
        return (m_SiProcessSpectrum(spectrum.data(), nbrSamples, specType, &blFromData, &thrFromData));
    }
    return -1;
}

int TwToolsWrapper::si_get_histogram(int histogramIndex, QVector<float> &intensity, QVector<unsigned int> &counts,  unsigned int &spectrumCount, double &meanValue) const
{
    if(m_SiGetHistogram){
        unsigned int arrayLength =intensity.length();
        if(arrayLength <= 0){
            (m_SiGetHistogram(histogramIndex, NULL, NULL, &arrayLength, &spectrumCount, &meanValue));

            unsigned int tmp = intensity.length();
            if(tmp < arrayLength)
                intensity.resize(arrayLength);

            tmp = counts.length();
            if(tmp < arrayLength)
                counts.resize(arrayLength);
        }

        return (m_SiGetHistogram(histogramIndex, intensity.data(), counts.data(), &arrayLength, &spectrumCount, &meanValue));
    }
    return -1;
}

int TwToolsWrapper::si_get_sum_histogram(int specType, QVector<float> &intensity, QVector<unsigned int> &counts,  unsigned int &spectrumCount, double &meanValue, double minMass, double maxMass, double minRate, double maxRate) const
{
    if(m_SiGetSumHistogram){

        unsigned int arrayLength =intensity.length();
        if(arrayLength <= 0){
            (m_SiGetSumHistogram(specType, NULL, NULL, &arrayLength, &spectrumCount, &meanValue, minMass, maxMass, minRate, maxRate));

            unsigned int tmp = intensity.length();
            if(tmp < arrayLength)
                intensity.resize(arrayLength);

            tmp = counts.length();
            if(tmp < arrayLength)
                counts.resize(arrayLength);
        }
        return (m_SiGetSumHistogram(specType, intensity.data(), counts.data(), &arrayLength, &spectrumCount, &meanValue, minMass, maxMass, minRate, maxRate));
    }
    return -1;
}

int TwToolsWrapper::si_reset_histograms() const
{
    if(m_SiResetHistograms){
        return (m_SiResetHistograms());
    }
    return -1;
}

int TwToolsWrapper::si_cleanup() const
{
    if(m_SiCleanup){
        return (m_SiCleanup());
    }
    return -1;
}

int TwToolsWrapper::si_fit_phd(QVector<double>&intensity, QVector<double>&counts, double *fwhm, double *a, QVector<double>&par) const
{
    if(m_SiFitPhd){
        int nbrPoints = 0;
        if(par.length()<4)
            par.resize(4);
        return (m_SiFitPhd(nbrPoints, intensity.data(), counts.data(), fwhm, a, par.data()));
    }
    return -1;
}

double TwToolsWrapper::si_eval_phd(QVector<double>&par, double intensity) const
{
    if(m_SiEvalPhd){
        if(par.length()<4)
            par.resize(4);
        return (m_SiEvalPhd(par.data(), intensity));
    }
    return -1;
}

int TwToolsWrapper::si_fit_rate_from_phd(int nbrPoints, double *intensity, double *counts, double *siPar, double *rate, double *fitCounts, int *nbrNIonTraces, double **nIonTrace) const
{
    if(m_SiFitRateFromPhd){
        return (m_SiFitRateFromPhd(nbrPoints, intensity, counts, siPar, rate, fitCounts, nbrNIonTraces, nIonTrace));
    }
    return -1;
}

int TwToolsWrapper::find_tps_ip(QString tpsSerial, int timeout,  QString &hostStr) const
{
    if(m_FindTpsIp){
        QByteArray tpsSerial_ba = tpsSerial.toLocal8Bit();
        int hostStrLen = hostStr.length();
        QByteArray hostStr_ba = hostStr.toLocal8Bit();

        return (m_FindTpsIp(tpsSerial_ba.data(), timeout, &hostStrLen, hostStr_ba.data()));
    }
    return -1;
}

