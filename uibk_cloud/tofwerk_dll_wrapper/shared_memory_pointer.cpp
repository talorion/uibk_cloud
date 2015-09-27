#include "shared_memory_pointer.h"

#include <QVector>
#include <QDebug>
shared_memory_pointer::shared_memory_pointer(QObject *par)
    : QObject(par),
      m_data(NULL)
{
m_data = new TSharedMemoryPointer();
}

shared_memory_pointer::~shared_memory_pointer()
{

}


TSharedMemoryPointer *shared_memory_pointer::data()
{
    return m_data;
}









