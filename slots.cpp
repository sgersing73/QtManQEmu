#include "mainwindow.h"
#include <QString>

void MainWindow::on_edtMemPath_textChanged(const QString &arg1)
{
    this->buildSystemParameter();
}

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

void MainWindow::on_chkMemPreallocActive_stateChanged(int arg1)
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
