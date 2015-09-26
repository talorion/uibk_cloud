#ifndef UIBK_CLOUD_CONFIGURATION_HPP
#define UIBK_CLOUD_CONFIGURATION_HPP

#include <QObject>

#include <configuration_element.hpp>

class uibk_cloud_configuration
        //: public QObject
{
    friend class ucc_factory;
    //Q_OBJECT
public:
    //explicit uibk_cloud_configuration(QObject *parent = 0);
    explicit uibk_cloud_configuration();
    ~uibk_cloud_configuration();
    //Q_DISABLE_COPY(uibk_cloud_configuration)

    void add_element(QString name, double mass_lo,double mass_hi,double fact,double sens);

public:
    QList<configuration_element *> getElements() const;

    //uibk_cloud_configuration* clone() { return new uibk_cloud_configuration(*this); }

    QString getFilename() const;
    void setFilename(const QString &value);

    QString getBg_filename() const;
    void setBg_filename(const QString &value);

signals:

public slots:

private:
    QString filename;
    QString bg_filename;
    QList<configuration_element*> elements;
};

#endif // UIBK_CLOUD_CONFIGURATION_HPP
