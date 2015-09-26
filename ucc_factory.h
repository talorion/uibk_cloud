#ifndef UCC_FACTORY_H
#define UCC_FACTORY_H

#include "uibk_cloud_configuration.hpp"

#include "tofwerk_dll_wrapper/twtoolswrapper.h"
class ucc_factory
{
private:
    ucc_factory();
    ~ucc_factory();
    Q_DISABLE_COPY(ucc_factory)

public:

    static uibk_cloud_configuration create_ucc_from_file(QString filename);
    static uibk_cloud_configuration create_dummy_ucc();

private:
    //static TwToolsWrapper tw_tools;

};

#endif // UCC_FACTORY_H
