#include "uibk_cloud_configuration.hpp"

//uibk_cloud_configuration::uibk_cloud_configuration(QObject *parent) : QObject(parent)
uibk_cloud_configuration::uibk_cloud_configuration():
    filename(),
    elements()
{

}

uibk_cloud_configuration::~uibk_cloud_configuration()
{
//    foreach (configuration_element* var, elements) {
//        if(var)
//            delete var;
//    }
//    elements.clear();

}

void uibk_cloud_configuration::add_element(QString name, double mass_lo, double mass_hi, double fact, double sens)
{
    configuration_element* elem = new configuration_element();
    elem->setName(name);
    elem->setMass_lo(mass_lo);
    elem->setMass_hi(mass_hi);
    elem->setFact(fact);
    elem->setSens(sens);

    elements.append(elem);
}


QList<configuration_element*> uibk_cloud_configuration::getElements() const
{
    return elements;
}
QString uibk_cloud_configuration::getFilename() const
{
    return filename;
}

void uibk_cloud_configuration::setFilename(const QString &value)
{
    filename = value;
}
QString uibk_cloud_configuration::getBg_filename() const
{
    return bg_filename;
}

void uibk_cloud_configuration::setBg_filename(const QString &value)
{
    bg_filename = value;
}




