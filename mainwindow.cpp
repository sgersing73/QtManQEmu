#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDir dir;

    ui->setupUi(this);

    QString AppDataPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    dir.mkpath(AppDataPath);
    QString SettingsFile = AppDataPath + "/settings.ini";

    mSettings = new QSettings(SettingsFile, QSettings::IniFormat);

    restoreGeometry(mSettings->value("geometry").toByteArray());
    restoreState(mSettings->value("windowState").toByteArray());
    ui->splitter->restoreState(mSettings->value("splitter").toByteArray());

    ui->edtProgramDir->setText( mSettings->value("QEmuProgramDir").toString() );

    if ( ! mSettings->value("stylsheet").toByteArray().isEmpty() ) {

        QFile f ( mSettings->value("stylsheet").toByteArray() );
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);

        qApp->setStyleSheet(ts.readAll());
        f.close();
    }

    this->fillCboSystems();
    this->fillCboConfigName();

    mProcess = new QProcess(this);
    connect (mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
    connect (mProcess, SIGNAL(readyReadStandardError()), this, SLOT(processError()));

    this->createActions();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mSettings->setValue("geometry", saveGeometry());
    mSettings->setValue("windowState", saveState());
    mSettings->setValue("splitter",ui->splitter->saveState());
    mSettings->sync();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions() {

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAct = helpMenu->addAction(this->style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutLic = helpMenu->addAction(this->style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("&License"), this, &MainWindow::license);
    aboutLic->setStatusTip(tr("Show the application's License Box"));

    QAction *aboutQtAct = helpMenu->addAction(this->style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

}

void MainWindow::processOutput()
{
    while (mProcess->canReadLine()) {
        ui->edtOutput->append( mProcess->readLine().replace("\r\n", "") );
    }
}

void MainWindow::processError()
{
    ui->edtError->append(mProcess->readAllStandardError());
}

void MainWindow::license()
{
    QString   info;

    QFile f ( ":/docs/license.txt" );
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    info = ts.readAll();
    f.close();

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    font.setPixelSize(9);

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("License Informations"));
    msgBox.setText(info);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setFont(font);

    msgBox.exec();
}

void MainWindow::about()
{
    QSysInfo  systemInfo;
    QString   info;

    QStorageInfo storage = QStorageInfo::root();

    info.append(tr("M3uMan V1.0\n"));
    info.append("\n");
    info.append(tr("Windows Version:\t%1\n").arg(systemInfo.windowsVersion()));
    info.append(tr("Build Cpu Architecture:\t%1\n").arg(systemInfo.buildCpuArchitecture()));
    info.append(tr("Current Cpu Architecture: %1\n").arg(systemInfo.currentCpuArchitecture()));
    info.append(tr("Kernel Type:\t\t%1\n").arg(systemInfo.kernelType()));
    info.append(tr("Kernel Version:\t\t%1\n").arg(systemInfo.kernelVersion()));
    info.append(tr("Machine Host Name:\t%1\n").arg(systemInfo.machineHostName()));
    info.append(tr("Product Type:\t\t%1\n").arg(systemInfo.productType()));
    info.append(tr("Product Version:\t%1\n").arg(systemInfo.productVersion()));
    info.append(tr("Byte Order:\t\t%1\n").arg(systemInfo.buildAbi()));
    info.append(tr("Pretty ProductName:\t%1\n").arg(systemInfo.prettyProductName()));
    info.append("\n");

    QHostInfo hostInfo = QHostInfo::fromName( QHostInfo::localHostName() );

    if (!hostInfo.addresses().isEmpty()) {
        QHostAddress address = hostInfo.addresses().first();
        info.append(tr("IP Address:\t\t%1\n").arg(address.toString()));
    }

    info.append("\n");

    qDebug() << storage.rootPath();
    if (storage.isReadOnly())
        qDebug() << "isReadOnly:" << storage.isReadOnly();

    info.append(tr("Storage Name:\t\t%1\n").arg(storage.displayName()));
    info.append(tr("File System Type:\t%1\n").arg(QString::fromStdString(storage.fileSystemType().toStdString())));
    info.append(tr("Size (GB):\t\t%1\n").arg(storage.bytesTotal()/1024/1024/1024));
    info.append(tr("Free (GB):\t\t%1\n").arg(storage.bytesAvailable()/1024/1024/1024));

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    font.setPixelSize(9);

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About Application"));
    msgBox.setText(info);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setFont(font);

    msgBox.exec();

}

void MainWindow::on_cmdProgramDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::rootPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if ( !dir.isEmpty() ) {

        ui->edtProgramDir->setText(dir);

        this->fillCboSystems();

        mSettings->setValue("QEmuProgramDir", dir);
        mSettings->sync();
    }
}

void MainWindow::fillCboSystems()
{
    ui->cboSystems->clear();

    QDir dir(ui->edtProgramDir->text());

    for (const QString &filename : dir.entryList(QStringList() << "*system*", QDir::Files))
        ui->cboSystems->addItem(filename);
}

void MainWindow::on_cmdStart_clicked()
{
    QStringList para ;

    ui->edtOutput->clear();
    ui->edtError->clear();

    para << ui->edtSystemParam->text().split(" ")
         << ui->edtDiskParam->text().split(" ")
         << ui->edtCdromParam->text().split(" ")
         << ui->edtFloppyParam->text().split(" ");

    qDebug() << ui->edtProgramDir->text() + "/" + ui->cboSystems->currentText();

    para.removeAll("");

    qDebug() << para;

    mProcess->start(ui->edtProgramDir->text() + "/" + ui->cboSystems->currentText(), para);
}

void MainWindow::buildSystemParameter() {

    QString para;

    if ( !ui->edtMemSize->text().isEmpty() )
        para = " -m size=" + ui->edtMemSize->text();

    if ( !ui->edtSlots->text().isEmpty() )
        para = para + ",slots=" + ui->edtSlots->text();

    if ( !ui->edtMaxMem->text().isEmpty() )
        para = para + ",maxmem=" + ui->edtMaxMem->text();

    if ( ui->chkBootmenu->isChecked() )
        para = para + " -boot menu=on";
    else
        para = para + " -boot menu=off";

    if ( !ui->cboLanguage->currentText().isEmpty() )
        para = para + " -k " + ui->cboLanguage->currentText();

    if ( !ui->cboAccel->currentText().isEmpty() )
        para = para + " -accel " + ui->cboAccel->currentText();

    if ( !ui->edtAccelProp->text().isEmpty() )
        para = para + "," + ui->edtAccelProp->text();

    if ( !ui->edtSMPCpus->text().isEmpty() )
        para = para + " -smp cpus=" + ui->edtSMPCpus->text();

    if ( !ui->edtSMPMaxCpus->text().isEmpty() )
        para = para + ",maxcpus=" + ui->edtSMPMaxCpus->text();

    if ( !ui->edtSMPSockets->text().isEmpty() )
        para = para + ",sockets=" + ui->edtSMPSockets->text();

    if ( !ui->edtSMPDies->text().isEmpty() )
        para = para + ",dies=" + ui->edtSMPDies->text();

    if ( !ui->edtSMPClusters->text().isEmpty() )
        para = para + ",clusters=" + ui->edtSMPClusters->text();

    if ( !ui->edtSMPCores->text().isEmpty() )
        para = para + ",cores=" + ui->edtSMPCores->text();

    if ( !ui->edtSMPThreads->text().isEmpty() )
        para = para + ",threads=" + ui->edtSMPThreads->text();

    if ( !ui->edtUUID->text().isEmpty() )
        para = para + " -uuid " + ui->edtUUID->text();

    if ( !ui->edtCpuModel->text().isEmpty() )
        para = para + " -cpu " + ui->edtCpuModel->text();

    if ( !ui->edtCpuProp->text().isEmpty() )
        para = para + "," + ui->edtCpuProp->text();

    if ( !ui->edtSystemName->text().isEmpty() )
        para = para + " -system " + ui->edtSystemName->text();

    if ( !ui->edtExtra->text().isEmpty() )
        para = para + " " + ui->edtExtra->text();

/*
    if ( !ui->edtMemPath->text().isEmpty() )
        para = para + " -mem-path " + ui->edtMemPath->text();

    if ( ui->chkMemPreallocActive->isChecked() )
        para = para + " -mem-prealloc";
*/
    ui->edtSystemParam->setText(para);
}

void MainWindow::buildDiskParameter() {

    QString para;

    if ( !ui->edtDisk_0->text().isEmpty() )
        para = "-drive file=" + ui->edtDisk_0->text() + ",index=0,media=disk";

    if ( !ui->edtDisk_1->text().isEmpty() )
        para = " -drive file=" + ui->edtDisk_1->text() + ",index=1,media=disk";

    if ( !ui->edtDisk_2->text().isEmpty() )
        para = " -drive file=" + ui->edtDisk_2->text() + ",index=2,media=disk";

    if ( !ui->edtDisk_3->text().isEmpty() )
        para = " -drive file=" + ui->edtDisk_3->text() + ",index=3,media=disk";

    ui->edtDiskParam->setText(para);
}

void MainWindow::buildCdromParameter() {

    QString para;

    if ( !ui->edtCdrom_0->text().isEmpty() )
        para = "-drive file=" + ui->edtCdrom_0->text() + ",index=1,media=cdrom";

    if ( !ui->edtCdrom_1->text().isEmpty() )
        para = " -drive file=" + ui->edtCdrom_1->text() + ",index=3,media=cdrom";

    ui->edtCdromParam->setText(para);
}

void MainWindow::buildFloppyParameter() {

    QString para;

    if ( !ui->edtFloppy_0->text().isEmpty() )
        para = "-drive file=" + ui->edtFloppy_0->text() + ",index=0,if=floppy,format=raw";

    if ( !ui->edtFloppy_1->text().isEmpty() )
        para = " -drive file=" + ui->edtFloppy_1->text() + ",index=1,if=floppy,format=raw";

    ui->edtFloppyParam->setText(para);
}

void MainWindow::on_tbuCreateImageFileName_clicked()
{
    QString selFilter="All files (*.*)";
    QString fileName = QFileDialog::getSaveFileName(this,"Save file",QDir::currentPath(),
                                 "Text files (*.txt);;All files (*.*)",&selFilter);

    if ( ! fileName.isEmpty() ) {
        ui->edtCreateImageFileName->setText (fileName);
    }
}

void MainWindow::on_cmdCreateImage_clicked()
{
    QStringList para ;

    para << "create" << "-f" << ui->cboCreateImageFormat->currentText() << ui->edtCreateImageFileName->text() << ui->edtCreateImageFileSize->text();

    qDebug() << para;

    mProcess->start(ui->edtProgramDir->text() + "/" + "qemu-img", para);
}


void MainWindow::on_toolButton_clicked()
{
    QString image = QFileDialog::getOpenFileName(this, tr("Open Directory"), QDir::rootPath());

    if ( !image.isEmpty() ) {

        switch (ui->tolDisk->currentIndex()) {
        case 0:  ui->edtDisk_0->setText(image);break;
        case 1:  ui->edtDisk_1->setText(image);break;
        case 2:  ui->edtDisk_2->setText(image);break;
        case 3:  ui->edtDisk_3->setText(image);break;
        }
    }
}

void MainWindow::on_edtDisk_2_textChanged(const QString &arg1)
{
    this->buildDiskParameter();
}


void MainWindow::on_edtDisk_3_textChanged(const QString &arg1)
{
    this->buildDiskParameter();
}

void MainWindow::on_cmdHelp_clicked()
{
    QStringList para;

    ui->edtOutput->clear();
    ui->edtError->clear();

    para << "-h";

    mProcess->start(ui->edtProgramDir->text() + "/" + ui->cboSystems->currentText(), para);
}

void MainWindow::on_cmdVersion_clicked()
{
    QStringList para;

    ui->edtOutput->clear();
    ui->edtError->clear();

    para << "-version";

    mProcess->start(ui->edtProgramDir->text() + "/" + ui->cboSystems->currentText(), para);
}

void MainWindow::on_toolButton_2_clicked()
{
    QString image = QFileDialog::getOpenFileName(this, tr("Open Directory"), QDir::rootPath());

    if ( !image.isEmpty() ) {

        switch (ui->tolCdrom->currentIndex()) {
        case 0:  ui->edtCdrom_0->setText(image);break;
        case 1:  ui->edtCdrom_1->setText(image);break;
        }
    }
}

void MainWindow::on_toolButton_3_clicked()
{
    QString image = QFileDialog::getOpenFileName(this, tr("Open Directory"), QDir::rootPath());

    if ( !image.isEmpty() ) {

        switch (ui->tolFloppy->currentIndex()) {
        case 0:  ui->edtFloppy_0->setText(image);break;
        case 1:  ui->edtFloppy_1->setText(image);break;
        }
    }
}

void MainWindow::on_cboModulInfo_currentTextChanged(const QString &arg1)
{
    QStringList para;

    if ( ! ui->cboModulInfo->currentText().isEmpty() ) {

        ui->edtOutput->clear();
        ui->edtError->clear();

        para << ui->cboModulInfo->currentText() << "help";

        mProcess->start(ui->edtProgramDir->text() + "/" + ui->cboSystems->currentText(), para);
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionSave_triggered()
{
    bool ok;

    if ( ui->cboConfigName->currentText().isEmpty() ) {

        QString text = QInputDialog::getText(this, tr("Configuration Name"),
                                             tr("Name"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            ui->cboConfigName->setCurrentText( text );
        } else {
            return;
        }
    }

    mSettings->beginGroup( ui->cboConfigName->currentText() );

    mSettings->setValue("Disk_0", ui->edtDisk_0->text() );
    mSettings->setValue("Disk_1", ui->edtDisk_1->text() );
    mSettings->setValue("Disk_2", ui->edtDisk_2->text() );
    mSettings->setValue("Disk_3", ui->edtDisk_3->text() );
    mSettings->setValue("Cdrom_0", ui->edtCdrom_0->text() );
    mSettings->setValue("Cdrom_1", ui->edtCdrom_1->text() );
    mSettings->setValue("Floppy_0", ui->edtFloppy_0->text() );
    mSettings->setValue("Floppy_1", ui->edtFloppy_1->text() );

    mSettings->setValue("MaxMem", ui->edtMaxMem->text() );
    mSettings->setValue("MemSlots", ui->edtSlots->text() );
    mSettings->setValue("MemSize", ui->edtMemSize->text() );
    mSettings->setValue("Bootmenu", ui->chkBootmenu->isChecked() );
    mSettings->setValue("Accel", ui->cboAccel->currentText() );
    mSettings->setValue("Language", ui->cboLanguage->currentText() );

    mSettings->setValue("CPUs", ui->edtSMPCpus->text() );
    mSettings->setValue("maxCPUs", ui->edtSMPMaxCpus->text() );
    mSettings->setValue("Sockets", ui->edtSMPSockets->text() );
    mSettings->setValue("Dies", ui->edtSMPDies->text() );
    mSettings->setValue("Clusters", ui->edtSMPClusters->text() );
    mSettings->setValue("Cores", ui->edtSMPCores->text() );
    mSettings->setValue("Threads", ui->edtSMPThreads->text() );

    mSettings->setValue("UUID", ui->edtUUID->text() );
    mSettings->setValue("Extra", ui->edtExtra->text() );

    mSettings->setValue("CpuModel", ui->edtCpuModel->text() );
    mSettings->setValue("CpuModelProp", ui->edtCpuProp->text() );
    mSettings->setValue("SystemName", ui->edtSystemName->text() );

    mSettings->setValue("System", ui->cboSystems->currentText() );

    mSettings->endGroup();
    mSettings->sync();

    this->fillCboConfigName();
}

void MainWindow::fillCboConfigName() {

    ui->cboConfigName->clear();

    ui->cboConfigName->addItems( mSettings->childGroups() );
}

void MainWindow::on_cboConfigName_currentIndexChanged(int index)
{
    mSettings->beginGroup( ui->cboConfigName->currentText() );

    ui->edtDisk_0->setText( mSettings->value("Disk_0").toString() );
    ui->edtDisk_1->setText( mSettings->value("Disk_1").toString() );
    ui->edtDisk_2->setText( mSettings->value("Disk_2").toString() );
    ui->edtDisk_3->setText( mSettings->value("Disk_3").toString() );
    ui->edtCdrom_0->setText( mSettings->value("Cdrom_0").toString() );
    ui->edtCdrom_1->setText( mSettings->value("Cdrom_1").toString() );
    ui->edtFloppy_0->setText( mSettings->value("Floppy_0").toString() );
    ui->edtFloppy_1->setText( mSettings->value("Floppy_1").toString() );

    ui->edtMaxMem->setText( mSettings->value("MaxMem").toString() );
    ui->edtSlots->setText( mSettings->value("MemSlots").toString() );
    ui->edtMemSize->setText( mSettings->value("MemSize").toString() );
    ui->chkBootmenu->setChecked( mSettings->value("Bootmenu").toBool() );
    ui->cboAccel->setCurrentText( mSettings->value("Accel").toString() );
    ui->cboLanguage->setCurrentText( mSettings->value("Language").toString() );

    ui->edtSMPCpus->setText( mSettings->value("CPUs").toString() );
    ui->edtSMPMaxCpus->setText( mSettings->value("maxCPUs").toString() );
    ui->edtSMPSockets->setText( mSettings->value("Sockets").toString() );
    ui->edtSMPDies->setText( mSettings->value("Dies").toString() );
    ui->edtSMPClusters->setText( mSettings->value("Clusters").toString() );
    ui->edtSMPCores->setText( mSettings->value("Cores").toString() );
    ui->edtSMPThreads->setText( mSettings->value("Threads").toString() );

    ui->edtUUID->setText( mSettings->value("UUID").toString() );
    ui->edtExtra->setText( mSettings->value("Extra").toString() );

    ui->edtCpuModel->setText( mSettings->value("CpuModel").toString() );
    ui->edtCpuProp->setText( mSettings->value("CpuModelProp").toString() );
    ui->edtSystemName->setText( mSettings->value("SystemName").toString() );

    ui->cboSystems->setCurrentText( mSettings->value("System").toString() );

    mSettings->endGroup();
}

void MainWindow::on_actionDelete_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, this->windowTitle(), "Are you sure want to delete?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        mSettings->beginGroup(ui->cboConfigName->currentText());
        mSettings->remove("");
        mSettings->endGroup();
        mSettings->sync();

        this->fillCboConfigName();
    }
}

void MainWindow::on_cmdUUID_clicked()
{
    ui->edtUUID->setText ( QUuid::createUuid().toString().remove("{").remove("}") ) ;
}

void MainWindow::on_actionStylsheet_triggered()
{
    QString path = mSettings->value("stylsheetpath").toByteArray();

    QString fileName = QFileDialog::getOpenFileName(this, ("Open qss stylsheet File"),
                                                     path,
                                                     ("qss stylsheet file (*.qss)"));
    QFile f(fileName);
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    } else {

        QFileInfo fi(fileName);

        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);

        qApp->setStyleSheet(ts.readAll());

        mSettings->setValue("stylsheet", fileName);
        mSettings->setValue("stylsheetpath", fi.path());
        mSettings->sync();
    }
}


