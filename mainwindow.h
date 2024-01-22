#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>
#include <QUuid>
#include <QHostInfo>
#include <QStorageInfo>
#include <QDir>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fillCboSystems();
    void fillCboConfigName();

    void createActions();

    void buildSystemParameter();
    void buildDiskParameter();
    void buildCdromParameter();
    void buildFloppyParameter();

    void about();
    void license();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:

    void processOutput();
    void processError();

    void on_cmdHelp_clicked();
    void on_cmdVersion_clicked();
    void on_cmdProgramDir_clicked();
    void on_cmdStart_clicked();

    void on_edtMemSize_textChanged(const QString &arg1);
    void on_edtSlots_textChanged(const QString &arg1);
    void on_edtMaxMem_textChanged(const QString &arg1);
    void on_chkMemPreallocActive_stateChanged(int arg1);
    void on_edtMemPath_textChanged(const QString &arg1);

    void on_tbuCreateImageFileName_clicked();
    void on_cmdCreateImage_clicked();


    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();

    void on_edtCdrom_0_textChanged(const QString &arg1);
    void on_edtCdrom_1_textChanged(const QString &arg1);

    void on_edtDisk_0_textChanged(const QString &arg1);
    void on_edtDisk_1_textChanged(const QString &arg1);
    void on_edtDisk_2_textChanged(const QString &arg1);
    void on_edtDisk_3_textChanged(const QString &arg1);

    void on_edtFloppy_0_textChanged(const QString &arg1);
    void on_edtFloppy_1_textChanged(const QString &arg1);

    void on_cboModulInfo_currentTextChanged(const QString &arg1);

    void on_actionExit_triggered();

    void on_chkBootmenu_stateChanged(int arg1);

    void on_cboLanguage_currentTextChanged(const QString &arg1);

    void on_cboAccel_currentTextChanged(const QString &arg1);

    void on_cboConfigName_currentIndexChanged(int index);

    void on_actionSave_triggered();

    void on_actionDelete_triggered();

    void on_cboSystems_currentTextChanged(const QString &arg1);

    void on_edtSMPCpus_textChanged(const QString &arg1);

    void on_edtSMPMaxCpus_textChanged(const QString &arg1);

    void on_edtSMPSockets_textChanged(const QString &arg1);

    void on_edtSMPDies_textChanged(const QString &arg1);

    void on_edtSMPClusters_textChanged(const QString &arg1);

    void on_edtSMPCores_textChanged(const QString &arg1);

    void on_edtSMPThreads_textChanged(const QString &arg1);

    void on_cmdUUID_clicked();

    void on_edtUUID_textChanged(const QString &arg1);

    void on_edtExtra_textChanged(const QString &arg1);

    void on_actionStylsheet_triggered();

    void on_edtCpuModel_textChanged(const QString &arg1);

    void on_edtSystemName_textChanged(const QString &arg1);

    void on_edtCpuProp_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QProcess       *mProcess;    
    QSettings      *mSettings;
    QString        m_SettingsFile;
};
#endif // MAINWINDOW_H
