#include "uibk_cloud_mainwindow.hpp"

#include <QMessageBox>

uibk_cloud_mainwindow::uibk_cloud_mainwindow(QWidget *par) :
    QMainWindow(par),
    main_layout(NULL),
    central_wdgt(NULL),
    editor(NULL),
    plot(NULL),
    fileMenu(NULL),
    helpMenu(NULL),
    fileToolBar(NULL),
    newAct(NULL),
    openAct(NULL),
    saveAct(NULL),
    saveAsAct(NULL),
    exitAct(NULL),
    aboutAct(NULL),
    recordBackgroundAction(NULL),
    curFile(),
    graphs(),
    lowerRange(0),
    upperRange(0),
    bg_meas_state(false),
    enable_export_control(NULL),
    enable_export_indicator(NULL)
{
    setWindowTitle(tr("uibk cloud"));

    setupEditor();
    setupGraph();

    central_wdgt = new QWidget(this);
    enable_export_control = new QCheckBox("enable data export");
    connect(enable_export_control, SIGNAL(clicked(bool)),this,SLOT(toggle_enable_export(bool)));
    enable_export_indicator = new QLabel();
    main_layout = new QVBoxLayout();
    //main_layout->addWidget(editor,1);
    main_layout->addWidget(enable_export_control);
    main_layout->addWidget(enable_export_indicator);
    main_layout->addWidget(plot,3);
    central_wdgt->setLayout(main_layout);
    setCentralWidget(central_wdgt);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
}

uibk_cloud_mainwindow::~uibk_cloud_mainwindow()
{

}

void uibk_cloud_mainwindow::setupEditor()
{
    editor = new QTextEdit();

    newFile();
}

void uibk_cloud_mainwindow::setupGraph()
{
    plot = new QCustomPlot();
    plot->yAxis->setAutoTickCount(2);
    plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    plot->yAxis->setScaleLogBase(10);
    plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot->xAxis->setAutoTickCount(2);
    plot->xAxis->setRangeLower(QDateTime::currentMSecsSinceEpoch()/1000.0);
    plot->xAxis->setDateTimeFormat("hh:mm:ss");
    plot->setWindowFlags(Qt::ToolTip);
    plot->xAxis->setDateTimeFormat("hh:mm:ss");
    plot->legend->setVisible(true);

    QDateTime t = QDateTime::currentDateTime();
    double curr =t.currentMSecsSinceEpoch()/1000.0;
    plot->xAxis->setRangeLower(curr);
}

void uibk_cloud_mainwindow::createActions()
{
    newAct = new QAction(QIcon(":/images/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    recordBackgroundAction = new QAction(QIcon(":/images/images/record.png"), tr("&Record BAckground"), this);
    recordBackgroundAction->setStatusTip(tr("Start recording BG disables yabbix export"));
    connect(recordBackgroundAction, SIGNAL(triggered()), this, SLOT(record_bg()));

}

void uibk_cloud_mainwindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

}

void uibk_cloud_mainwindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    fileToolBar->addSeparator();

    fileToolBar->addAction(recordBackgroundAction);
}

void uibk_cloud_mainwindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool uibk_cloud_mainwindow::maybeSave()
{
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void uibk_cloud_mainwindow::loadFile(const QString &fileName)
{
    emit config_file_changed(fileName);
    //QString fileName_tmp(fileName);
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    file.close();
    //emit config_file_changed(fileName_tmp);
}

bool uibk_cloud_mainwindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << editor->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void uibk_cloud_mainwindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    editor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled";
    //else
    //emit config_file_changed(fileName);

    setWindowFilePath(shownName);
}

QString uibk_cloud_mainwindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void uibk_cloud_mainwindow::receive_data(QDateTime mtime, groups_t groups)
{
    //qDebug()<<"tick";
    //QMap<QString, QCPGraph*> graphs;
    //QMap<QString, QCPGraph*>::iterator
    groups_t::iterator it;
    double time = mtime.currentMSecsSinceEpoch()/1000.0;
    QPen tmp_g_brusch;

    int i =0;
    for(it=groups.begin();it!=groups.end();++it){
        QMap<QString, QCPGraph*>::iterator gr=  graphs.find(it.key());
        if(gr==graphs.end()){
            QCPGraph* tmp_g=plot->addGraph();
            tmp_g->setName(it.key());
            tmp_g->addToLegend();
            tmp_g_brusch = tmp_g->pen();
            tmp_g_brusch.setColor(QColor((Qt::GlobalColor)(i)));
            tmp_g->setPen(tmp_g_brusch);
            gr = graphs.insert(it.key(),tmp_g);
        }
        QCPGraph* tmp = gr.value();
        if(!tmp)
            continue;
        double value = it.value();
        lowerRange = qMin(value, lowerRange);
        upperRange = qMax(value, upperRange);

        tmp->setName(it.key()+" "+QString::number(value));
        tmp->addData(time, value);
        i++;
        i = i%15+4;
    }
    plot->xAxis->setRangeUpper(time+30);
    //plot->xAxis->setRangeLower(time - 60);
    plot->yAxis->setRangeLower(lowerRange-10);
    plot->yAxis->setRangeUpper(upperRange+10);

    plot->replot();
}

void uibk_cloud_mainwindow::receive_exported_data(QDateTime mtime, averages_t groups)
{
    enable_export_control->setChecked(true);
    enable_export_indicator->setText("exported "+QString::number(groups.size())+"values at " +mtime.toString());
    reset_graph();
}

void uibk_cloud_mainwindow::reset_graph()
{
    qDebug()<<"AAA";
    graphs.clear();
    plot->clearGraphs();

    QDateTime t = QDateTime::currentDateTime();
    double curr =t.currentMSecsSinceEpoch()/1000.0;
    plot->xAxis->setRangeLower(curr);
}

void uibk_cloud_mainwindow::bg_meas_state_changed(bool std)
{
    bg_meas_state = std;
    if(!bg_meas_state){
        recordBackgroundAction->setIcon(QIcon(":/images/images/record.png"));
        //stop_bg_recording();
    }else
    {
        recordBackgroundAction->setIcon(QIcon(":/images/images/record-red.png"));
        // start_bg_recording();
    }

}

void uibk_cloud_mainwindow::newFile()
{
    if (maybeSave()) {
        editor->clear();
        setCurrentFile("");
    }
}

void uibk_cloud_mainwindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "uibk cloud config file (*.txt)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool uibk_cloud_mainwindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool uibk_cloud_mainwindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File"), "", "uibk cloud config file (*.txt)");
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return saveFile(files.at(0));
}

void uibk_cloud_mainwindow::about()
{
    QMessageBox::about(this, tr("About Nephele"),
                       tr("<b>Nephele</b> provides a script functionality "
                          "bla bla "
                          "bla bla"));
}

void uibk_cloud_mainwindow::record_bg()
{
    if(bg_meas_state){
        recordBackgroundAction->setIcon(QIcon(":/images/images/record.png"));
        emit stop_bg_recording();
    }else
    {
        recordBackgroundAction->setIcon(QIcon(":/images/images/record-red.png"));
        emit start_bg_recording();
    }
}

void uibk_cloud_mainwindow::toggle_enable_export(bool checked)
{
    if(checked)
        emit start_exoprt();
    else
        emit stop_export();
}
