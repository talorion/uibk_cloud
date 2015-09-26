#ifndef SHARED_MEMORY_DESCRIPTOR_H
#define SHARED_MEMORY_DESCRIPTOR_H

#include <QtCore>
#include <QObject>

#ifndef Q_OS_WIN
typedef qint64 __int64;
//typedef quint64 __int64;
#ifdef TOFDAQDLL_EXPORTS
#define TOFWERK_DAQ_API Q_DECL_EXPORT
#else
#define TOFWERK_DAQ_API Q_DECL_IMPORT
#endif
#endif
#include <TofDaqDll.h>

class shared_memory_descriptor:public QObject
{
    Q_OBJECT
public:
    shared_memory_descriptor(QObject *par = 0);
    ~shared_memory_descriptor();
    Q_DISABLE_COPY(shared_memory_descriptor)

    TSharedMemoryDesc* data()const{return m_data;}

private:
    TSharedMemoryDesc* m_data;
};
//Q_DECLARE_METATYPE(shared_memory_descriptor)

#endif // SHARED_MEMORY_DESCRIPTOR_H
