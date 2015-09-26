#ifndef UIBK_CLOUD_MAINWINDOW_HPP
#define UIBK_CLOUD_MAINWINDOW_HPP

#include <QMainWindow>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>

#include <qcustomplot.hpp>

#include "uibk_cloud_global.h"

class uibk_cloud_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit uibk_cloud_mainwindow(QWidget *par = 0);
    ~uibk_cloud_mainwindow();
    Q_DISABLE_COPY(uibk_cloud_mainwindow)

signals:
    void config_file_changed(const QString &fileName);
    void start_bg_recording();
    void stop_bg_recording();
    void start_exoprt();
    void stop_export();

private:
    void setupEditor();
    void setupGraph();

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    bool maybeSave();

    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

public slots:
    //void view_average(QDateTime, averages_t);
    void receive_data(QDateTime mtime, groups_t groups);

    void reset_graph();

    void bg_meas_state_changed(bool std);

private slots:
    void newFile();
    void open();
    bool save();

    bool saveAs();
    void about();

    void record_bg();

private:
    QVBoxLayout * main_layout;

    QWidget* central_wdgt;
    QTextEdit *editor;
    QCustomPlot *plot;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;

    QAction *recordBackgroundAction;

    QString curFile;

    QMap<QString, QCPGraph*> graphs;
    double lowerRange ;
    double upperRange ;


    bool bg_meas_state;

};

#endif // UIBK_CLOUD_MAINWINDOW_HPP
