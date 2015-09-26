#include "configuration_element.hpp"

//configuration_element::configuration_element(QObject *parent) : QObject(parent)
configuration_element::configuration_element():
    name(),
    mass_lo(0),
    mass_hi(0),
    fact(0),
    sens(0)
{

}

configuration_element::~configuration_element()
{

}
QString configuration_element::getName() const
{
    return name;
}

void configuration_element::setName(const QString &value)
{
    name = value;
}

double configuration_element::getMass_lo() const
{
    return mass_lo;
}

void configuration_element::setMass_lo(double value)
{
    mass_lo = value;
}

double configuration_element::getMass_hi() const
{
    return mass_hi;
}

void configuration_element::setMass_hi(double value)
{
    mass_hi = value;
}
double configuration_element::getFact() const
{
    return fact;
}

void configuration_element::setFact(double value)
{
    fact = value;
}
double configuration_element::getSens() const
{
    return sens;
}

void configuration_element::setSens(double value)
{
    sens = value;
}






