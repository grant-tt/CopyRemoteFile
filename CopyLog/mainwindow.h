#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/private/qzipwriter_p.h>
#include <QtGui/private/qzipreader_p.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void ConnectToServer(QString IP, int Port, QString UserName, QString PassWord);

    bool isFolderEmpty(const QString &folderPath);

    QString getFolderName(const QString &folderPath);

    QStringList datetimeRangeFilter(const QString &filename, const QDateTime &startDateTime, const QDateTime &endDateTime);

    QStringList traverseDirectory(const QString &directoryPath);

    QStringList findFilesWithDatetimeRange(const QString &folderPath, const QDateTime &startDateTime, const QDateTime &endDateTime);

    void downloadFile(QString remotePath, QString localPath, QString makdirPath);

    //    void zipFolder(QString source_dir, QString output_file);

    // 参数1:源文件夹 例：D:/develop/Projects/pycharmProject/copyLog/TT
    // 参数2：打包后文件夹，例 D:/develop/Projects/pycharmProject/copyLog/TT.zip
    bool zipFolder(const QString &folderPath, const QString &zipFilePath);

    void addFileToZip(QZipWriter &zipWriter, const QString &filePath, const QString &zipPath);

    void addDirToZip(QZipWriter &zipWriter, const QString &dirPath, const QString &zipPath);

private slots:
    // 更改IP
    void on_ledIP_editingFinished();

    // 更改Port
    void on_ledPort_editingFinished();

    void on_ledUserName_editingFinished();

    void on_ledPassword_editingFinished();

    void on_btnConnect_clicked();

    void on_btnCopy_clicked();

    void on_btnSelectPath_clicked();

    void on_btnSave_clicked();

private:
    Ui::MainWindow* ui;
    QString m_strIp;
    QString m_strUserName;
    QString m_strPassWord;
    int m_nPort;
    QString m_strSource_pathDefault = "C:/App_Data/Log";
    QString m_strSource_path = "C:/App_Data/Log";
    QString m_strDestination_path;
    QProcess process;
};
#endif // MAINWINDOW_H
