#ifndef SHARED_MEMORY_POINTER_H
#define SHARED_MEMORY_POINTER_H

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

class shared_memory_pointer : public QObject
{
    Q_OBJECT
public:
    explicit shared_memory_pointer(QObject *par = 0);
    ~shared_memory_pointer();
    Q_DISABLE_COPY(shared_memory_pointer)

    TSharedMemoryPointer *data() ;

signals:

public slots:

private:
    TSharedMemoryPointer *m_data;

};

#endif // SHARED_MEMORY_POINTER_H
