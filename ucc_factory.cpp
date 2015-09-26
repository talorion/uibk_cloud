#include "ucc_factory.h"

#include "configuration_element.hpp"

#include <QFile>

ucc_factory::ucc_factory()
{

}

ucc_factory::~ucc_factory()
{

}

uibk_cloud_configuration ucc_factory::create_ucc_from_file(QString fileName)
{
    uibk_cloud_configuration cfg;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        //        QMessageBox::warning(this, tr("Application"),
        qDebug()<<QString("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString());
        return cfg;
    }
    QTextStream in(&file);


    QString ln;
    QStringList lnSplit;
    QString name;
    double mass_lo;
    double mass_hi;
    double fact;
    double sens;
    bool ok;
    while(!in.atEnd()){
        ln = in.readLine();
        lnSplit = ln.split(';');
        if(lnSplit.size()< 6)
            continue;
        name = lnSplit[0];
        mass_lo = lnSplit[2].toDouble(&ok);
        if(!ok)
            continue;
        mass_hi = lnSplit[3].toDouble(&ok);
        if(!ok)
            continue;
        fact = lnSplit[4].toDouble(&ok);
        if(!ok)
            continue;
        sens = lnSplit[5].toDouble(&ok);
        if(!ok)
            continue;


        cfg.add_element(name, mass_lo, mass_hi,fact,sens);
    }

    return cfg;
}

uibk_cloud_configuration ucc_factory::create_dummy_ucc()
{
    uibk_cloud_configuration cfg;

    cfg.add_element("a-pen", 137.18, 137.24, 1.1, 4 );
    //cfg.add_element("a-pen", 137.18, 137.24, 1, 1 );
    cfg.add_element("a-pen", 81.105, 81.135, 1, 3 );
    cfg.add_element("decanal", 157.22, 157.28, 1.05, 10);


    return cfg;
}

