#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T19:33:00
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uibk_cloud
TEMPLATE = app

unix|win32-g++: {
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
    QMAKE_CXXFLAGS += -isystem '$ORIGIN'/../../../libs/qcustomplot
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wextra
    QMAKE_CXXFLAGS += -ansi
    QMAKE_CXXFLAGS += -pedantic
    QMAKE_CXXFLAGS += -Wshadow
    QMAKE_CXXFLAGS += -Weffc++
    QMAKE_CXXFLAGS += -Wstrict-aliasing
}

#==========Deploy
win32: {
    TARGET_CUSTOM_EXT = .exe

    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/debug/))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/release/))
    }

    DEPLOY_COMMAND = windeployqt
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} --dir $${DLLDESTDIR} --no-translations $${DEPLOY_TARGET}
}
#==========================================


SOURCES += main.cpp\
        uibk_cloud_mainwindow.cpp \
    tof_data_read_worker/tof_data_read_worker.cpp \
    tofwerk_dll_wrapper/data_aquisition_dll_wrapper.cpp \
    tofwerk_dll_wrapper/shared_memory_descriptor.cpp \
    tofwerk_dll_wrapper/shared_memory_pointer.cpp \
    tofwerk_dll_wrapper/twtoolswrapper.cpp \
    uibk_cloud_configuration.cpp \
    configuration_element.cpp \
    tof_data_read_worker/tof_data_read_thread.cpp \
    ucc_factory.cpp \
    tof_data_export_worker/tof_data_export_worker.cpp \
    tof_data_export_worker/tof_data_export_thread.cpp

HEADERS  += uibk_cloud_mainwindow.hpp \
    tof_data_read_worker/tof_data_read_worker.hpp \
    tofwerk_dll_wrapper/data_aquisition_dll_wrapper.h \
    tofwerk_dll_wrapper/shared_memory_descriptor.h \
    tofwerk_dll_wrapper/shared_memory_pointer.h \
    tofwerk_dll_wrapper/twtoolswrapper.h \
    uibk_cloud_configuration.hpp \
    configuration_element.hpp \
    tof_data_read_worker/tof_data_read_thread.h \
    ucc_factory.h \
    tof_data_export_worker/tof_data_export_worker.h \
    tof_data_export_worker/tof_data_export_thread.h \
    uibk_cloud_global.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/release/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/debug/ -lqcustomplot
else:unix: LIBS += -L$$OUT_PWD/../qcustomplot/ -lqcustomplot

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/release/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/debug/libqcustomplot.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/release/qcustomplot.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/debug/qcustomplot.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/libqcustomplot.a

unix: INCLUDEPATH += $$PWD/../../../libs/TofDaq_1.97_API/include
unix: DEPENDPATH += $$PWD/../../../libs/TofDaq_1.97_API/include

win32-g++: INCLUDEPATH += $$PWD/../../../libs/TofDaq_1.97_API/include
win32-g++: DEPENDPATH += $$PWD/../../../libs/TofDaq_1.97_API/include

RESOURCES += \
    resources.qrc

DISTFILES += \
    misc/massListforZabbix.txt
