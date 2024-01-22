#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_edtSlots_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtMaxMem_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtMemSize_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtDisk_0_textChanged(const QString &arg1)
{
    this->buildDiskParameter();
}

void MainWindow::on_edtDisk_1_textChanged(const QString &arg1)
{
    this->buildDiskParameter();
}

void MainWindow::on_edtCdrom_0_textChanged(const QString &arg1)
{
    this->buildCdromParameter();
}

void MainWindow::on_edtCdrom_1_textChanged(const QString &arg1)
{
    this->buildCdromParameter();
}

void MainWindow::on_edtFloppy_0_textChanged(const QString &arg1)
{
    this->buildFloppyParameter();
}

void MainWindow::on_edtFloppy_1_textChanged(const QString &arg1)
{
    this->buildFloppyParameter();
}

void MainWindow::on_edtCpuModel_textChanged(const QString &arg1)
{
   this->buildSystemParameter();
}

void MainWindow::on_edtUUID_textChanged(const QString &arg1)
{
   this->buildSystemParameter();
}

void MainWindow::on_edtExtra_textChanged(const QString &arg1)
{
   this->buildSystemParameter();
}

void MainWindow::on_cboSystems_currentTextChanged(const QString &arg1)
{
    this->on_cmdVersion_clicked();
}

void MainWindow::on_edtSMPCpus_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSMPMaxCpus_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSMPSockets_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSMPDies_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSMPClusters_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSMPCores_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSMPThreads_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_chkBootmenu_stateChanged(int arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_cboLanguage_currentTextChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_cboAccel_currentTextChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtSystemName_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtCpuProp_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtAccelProp_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

void MainWindow::on_edtMachineName_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
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
