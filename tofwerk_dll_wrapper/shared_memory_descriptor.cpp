#include "shared_memory_descriptor.h"

shared_memory_descriptor::shared_memory_descriptor(QObject *par):
    QObject(par),
    m_data(0)
{
     m_data = new TSharedMemoryDesc();
}

shared_memory_descriptor::~shared_memory_descriptor()
{

}



