#ifndef CONFIGURATION_ELEMENT_HPP
#define CONFIGURATION_ELEMENT_HPP

#include <QObject>

class configuration_element
        //: public QObject
{
    //Q_OBJECT
public:
    //explicit configuration_element(QObject *parent = 0);
    explicit configuration_element();
    ~configuration_element();
    Q_DISABLE_COPY(configuration_element)

    QString getName() const;
    void setName(const QString &value);

    double getMass_lo() const;
    void setMass_lo(double value);

    double getMass_hi() const;
    void setMass_hi(double value);

    double getFact() const;
    void setFact(double value);

    double getSens() const;
    void setSens(double value);

private:
    QString name;
    double mass_lo;
    double mass_hi;
    double fact;
    double sens;
};

#endif // CONFIGURATION_ELEMENT_HPP
