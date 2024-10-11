#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ledIP_editingFinished()
{
    m_strIp = ui->ledIP->text();
    qDebug() << "IP changed" << m_strIp;
}


void MainWindow::on_ledPort_editingFinished()
{
    m_nPort = ui->ledPort->text().toInt();
    qDebug() << "Port changed" << m_nPort;
}


void MainWindow::on_ledUserName_editingFinished()
{
    m_strUserName = ui->ledUserName->text();
    qDebug() << "UserName changed" << m_strUserName;
}


void MainWindow::on_ledPassword_editingFinished()
{
    m_strPassWord = ui->ledPassword->text();
    qDebug() << "PassWord changed" << m_strPassWord;
}

bool MainWindow::isFolderEmpty(const QString &folderPath)
{
    QDir dir(folderPath);
    if (!dir.exists())
    {
        // 文件夹不存在
        return false;
    }
    //    // 检查文件夹中的文件和子文件夹
    //    return dir.entryList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs).isEmpty();

    // 获取文件夹中所有条目的列表，包括文件和子文件夹
    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    return entries.isEmpty();
}

QString MainWindow::getFolderName(const QString &folderPath)
{
    QString folderName = QDir(folderPath).dirName();
    return folderName;
}


QStringList MainWindow::datetimeRangeFilter(const QString &filename, const QDateTime &startDateTime, const QDateTime &endDateTime)
{
    QStringList filters;
    filters << startDateTime.toString("yyyyMMdd_HHmm") << endDateTime.toString("yyyyMMdd_HHmm");
    return filters;
}


QStringList MainWindow::traverseDirectory(const QString &directoryPath)
{
    //    QDir dir(directoryPath);
    //    QStringList listDirectorys;
    //    QStringList listFiles;
    //    QStringList listDirectorysAndFiles;
    //    // 确保目录存在
    //    //    if (!dir.exists())
    //    //    {
    //    //        qDebug() << "Directory does not exist:" << directoryPath;
    //    //        return;
    //    //    }

    //    // 遍历目录中的所有条目
    //    const QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    //    for (const QFileInfo &entry : entries)
    //    {
    //        //        qDebug() << "entry" << entry;
    //        if (entry.isDir() && !isFolderEmpty(entry.absoluteFilePath()))
    //        {
    //            // 打印目录路径
    //            //            qDebug() << "Directory:" << entry.absoluteFilePath();
    //            //            if (isFolderEmpty(entry.absoluteFilePath()))
    //            //            {
    //            //                qDebug() << "Directory:" << entry.absoluteFilePath();
    //            //                listDirectorysAndFiles.append(entry.absoluteFilePath());
    //            //            }
    //            // 递归遍历子目录
    //            traverseDirectory(entry.absoluteFilePath());
    //        }
    //        else
    //        {
    //            // 打印文件路径
    //            //            qDebug() << "File:" << entry.absoluteFilePath();
    //            listDirectorysAndFiles.append(entry.absoluteFilePath());
    //        }
    //        listDirectorysAndFiles.append(entry.absoluteFilePath());
    //    }
    //    return listDirectorysAndFiles;


    //    QStringList fileList;
    //    QDir dir(directoryPath);

    //    if (!dir.exists())
    //    {
    //        qDebug() << "Directory does not exist:" << directoryPath;
    //        return fileList;
    //    }

    //    const QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    //    for (const QFileInfo &entry : entries)
    //    {
    //        QString fullPath = entry.absoluteFilePath();
    //        fileList.append(fullPath);
    //        if (entry.isDir())
    //        {
    //            // 递归遍历子文件夹
    //            QStringList subFiles = traverseDirectory(fullPath);
    //            fileList.append(subFiles);
    //        }
    //    }

    //    return fileList;


    // 考虑空文件夹
    QStringList fileList;
    QDir dir(directoryPath);

    if (!dir.exists())
    {
        qDebug() << "Directory does not exist:" << directoryPath;
        return fileList;
    }

    const QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &entry : entries)
    {
        if (entry.isDir())
        {
            // 检查是否为空文件夹
            QDir subDir(entry.absoluteFilePath());
            if (subDir.isEmpty())
            {
                fileList.append(entry.absoluteFilePath());
            }
            // 递归遍历子文件夹
            QStringList subFiles = traverseDirectory(entry.absoluteFilePath());
            fileList.append(subFiles);
        }
        else
        {
            // 添加文件
            fileList.append(entry.absoluteFilePath());
        }
    }

    return fileList;

}

QStringList MainWindow::findFilesWithDatetimeRange(const QString &folderPath, const QDateTime &startDateTime, const QDateTime &endDateTime)
{
    QStringList matchingFiles;
    QDir dir(folderPath);
    if (!dir.exists())
    {
        qDebug() << "Directory does not exist:" << folderPath;
        return matchingFiles;
    }

    QStringList matchingFilesALL =  traverseDirectory(folderPath);
    //    const QStringList entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &filename : matchingFilesALL)
    {
        // qDebug() << filename;
        QFileInfo fileInfo(dir.absoluteFilePath(filename));


        //        if (curre0 >= startDateTime && curre0 <= endDateTime)
        //    {
        //        qDebug() << "23333333333";
        //    }

        //        if (startDateTime <= fileInfo.baseName() <= endDateTime)
        //        {
        //            qDebug() << fileInfo.baseName();
        //            matchingFiles.append(fileInfo.absoluteFilePath());
        //        }
        QDateTime fileDateTime = QDateTime::fromString(fileInfo.baseName(), "yyyy-MM-dd_hh-mm-ss"); // Adjust the format as needed

        if (fileDateTime.isValid() && fileDateTime >= startDateTime && fileDateTime <= endDateTime)
        {
            matchingFiles.append(fileInfo.absoluteFilePath());
        }
    }
    return matchingFiles;
}

void MainWindow::downloadFile(QString remotePath, QString localPath, QString makdirPath)
{
    //    QNetworkAccessManager* manager = new QNetworkAccessManager();
    //    QNetworkRequest request(ftpUrl);

    //    // 发起FTP下载请求
    //    QNetworkReply* reply = manager->get(request);

    //    // 等待请求完成
    //    QObject::connect(reply, &QNetworkReply::finished, [reply, localFilePath]()
    //    {
    //        if (reply->error() == QNetworkReply::NoError)
    //        {
    //            // 确保目标路径存在，如果不存在则创建
    //            QDir dir = QFileInfo(localFilePath).absoluteDir();
    //            if (!dir.exists())
    //            {
    //                dir.mkpath(".");
    //            }

    //            // 将下载的内容写入文件
    //            QFile file(localFilePath);
    //            if (file.open(QIODevice::WriteOnly))
    //            {
    //                file.write(reply->readAll());
    //                file.close();
    //                qDebug() << "文件下载成功，保存路径：" << localFilePath;
    //            }
    //            else
    //            {
    //                qDebug() << "无法打开文件进行写入：" << localFilePath;
    //            }
    //        }
    //        else
    //        {
    //            qDebug() << "下载失败：" << reply->errorString();
    //        }
    //        reply->deleteLater();
    //    });

    //    // 连接下载进度信号
    //    QObject::connect(reply, &QNetworkReply::downloadProgress, [](qint64 bytesReceived, qint64 bytesTotal)
    //    {
    //        qDebug() << "下载进度：" << bytesReceived << "of" << bytesTotal;
    //    });


    // 创建QNetworkAccessManager对象
    //    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // 设置FTP服务器的URL

    // FTP服务器信息
    //    QString ftpServer = "\\10.10.40.197\\App_Data";
    //    QString ftpPath = "C:/App_Data/file.txt";
    //    QString username = "Luffy";
    //    QString password = "000000";

    //    QProcess* process = new QProcess();
    //    QDir exeDir = QDir::current();

    // 远程共享文件夹路径
    //    QString remotePath = "\\\\RemoteComputerName\\SharedFolder\\*";
    //    QString remotePath = ftpServer +  "\\testShare\\**";

    // 本地目标路径
    //    QString localPath = "C:\\LocalFolderPath\\";

    //    qDebug() << "+++++" << ftpServer << password << username << remotePath << " to " << m_strDestination_path;
    //    QString commandnet = QString("net use %1 %2 /user:%3").arg(ftpServer, password, username);
    //    process->start("cmd.exe", QStringList() << "/c" << commandnet);
    //    qDebug() << commandnet;
    // xcopy命令
    //    QString Destination_path =  "\"m_strDestination_path\"";
    //"\"+ m_strDestination_path. +"\";
    //    process.start("cmd.exe", QStringList() << "/c" << "\"C:/Program Files/HaoZip/HaoZip.exe\"");
    //    QString command = QString("xcopy %1 %2 /y /e").arg("\\10.10.40.197\\App_Data\\testShare\\**", "D:\\develop\\test1");
    //    qDebug() << command;
    //    QProcess process;
    //    process->start("cmd", QStringList() << "/c" << "dir");



    //    QProcess process2;
    //    process2.start("cmd.exe"); // 启动cmd进程
    //    process2.waitForStarted(); // 等待启动

    //    QString commandd = "xcopy \\10.10.40.197\\App_Data\\testShare\\** D:\\develop\\test /y /e"; // 这里可以替换为你想执行的CMD命令
    //    process2.write(commandd.toStdString().c_str());
    //    process2.write("\n");

    //    while (process2.canReadLine())
    //    {
    //        QString output = QString::fromLocal8Bit(process2.readLine());
    //        qDebug() << output; // 输出命令执行结果
    //    }

    //    QProcess p(0);
    //    p.start("cmd", QStringList() << "/c" << "net use \\10.10.40.197\App_Data 000000 /user:Luffy");
    //    p.start("cmd", QStringList() << "/c" << "xcopy \\10.10.40.197\\App_Data\\testShare\\** D:\\develop\\test /y /e");
    //    p.start("cmd", QStringList() << "/c" << "xcopy \\10.10.40.197\App_Data\\testShare\\** D:\\develop\\test /y /e \n");



    //    QString comm = "xcopy D:\develop\test\IstarApp\AppOne\** D:\develop\test /y /e";
    //    p.start("cmd");
    //    qDebug() << comm.toLatin1();
    //    p.write(comm.toLatin1());
    //    p.waitForStarted();
    //    p.waitForFinished();

    //    QProcess process;
    // 设置bat脚本的路径
    //    QString batFilePath = "D:\\develop\\test\\Copy.bat";
    // 设置要传递给bat脚本的参数
    //    QStringList arguments;
    //    arguments << "10.10.40.197" << "App_Data\\20test\\Alarm\\Released\\2024-09-09\\2024-09-09_15-06-14" << "D:\\develop\test";

    // 启动bat脚本并传递参数
    //    process.start("batFilePath", arguments);

    //    process.start("cmd", QStringList() << "/c" << "dir");

    // 远程共享文件夹路径
    //    QString remotePath = "\\\\10.10.40.197\\App_Data\\testShare";
    //    remotePath = "\\\\10.10.40.197\\App_Data\\testShare\\2024-09-09\\2024-09-09_15-06-14";
    //    remotePath = "\\\\10.10.40.197\\App_Data\\20test\\Alarm\\Reported\\2024-09-09\\2024-09-09_16-06-14";
    //  "\\\\10.10.40.197\\App_Data\\20test\\Alarm\\Reported\\2024-09-09\\2024-09-09_16-06-14" "D:\\develop\\test" "D:\\develop\\test\\Alarm\\Reported\\2024-09-09"
    // 本地目标路径
    //    localPath = "D:\\develop\\test";
    //    QString localPath = "D:\\develop\\test";
    //    makdirPath = "D:\\develop\\test\\testShare\\2024-09-09";
    QString commandmk =  QString("mkdir %1").arg(makdirPath) + "\n";
    //    QString commandmk =  QString("mkdir %1").arg(makdirPath);
    //    qDebug() << commandmk;
    //    process.start("cmd", QStringList() << "/c" << commandmk);
    // xcopy命令
    QString command = QString("xcopy %1 %2 /y /e").arg(remotePath, makdirPath) + "\n";

    //    process.start("cmd", QStringList() << "/c" << command);

    process.start("cmd.exe");
    if (process.waitForStarted())
    {
        process.write(commandmk.toLatin1());
        process.write(command.toLatin1());
        process.write("exit\n");
    }
    process.waitForStarted();
    process.waitForFinished();
    //    QString strTemp = QString::fromLocal8Bit(process.readAllStandardOutput());

    //    QMessageBox testMassage;
    //    testMassage.setText(strTemp);
    //    testMassage.exec();

}

void MainWindow::addFileToZip(QZipWriter &zipWriter, const QString &filePath, const QString &zipPath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        zipWriter.addFile(zipPath, file.readAll());
        file.close();
        qDebug() << "Added file to zip:" << zipPath;
    }
    else
    {
        qWarning() << "Failed to open file for reading:" << filePath;
    }
}

void MainWindow::addDirToZip(QZipWriter &zipWriter, const QString &dirPath, const QString &zipPath)
{
    //    QDir dir(dirPath);
    //    if (!dir.exists())
    //    {
    //        qWarning() << "Directory does not exist:" << dirPath;
    //        return;
    //    }
    //    QStringList entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    //    for (const QString &entry : entries)
    //    {
    //        QString newZipPath = zipPath.isEmpty() ? entry : zipPath + "/" + entry;
    //        if (dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot).contains(entry))
    //        {
    //            zipWriter.addDirectory(newZipPath);
    //            addDirToZip(zipWriter, dir.absoluteFilePath(entry), newZipPath);
    //        }
    //        else
    //        {
    //            addFileToZip(zipWriter, dir.absoluteFilePath(entry), newZipPath);
    //        }
    //    }

    QDir dir(dirPath);
    if (!dir.exists())
    {
        qWarning() << "Directory does not exist:" << dirPath;
        return;
    }
    // Add the directory to zip, even if it's empty
    if (zipPath.endsWith("/"))
    {
        zipWriter.addDirectory(zipPath);
    }
    else
    {
        zipWriter.addDirectory(zipPath + "/");
    }

    QStringList entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &entry : entries)
    {
        QString newZipPath = zipPath.isEmpty() ? entry : zipPath + "/" + entry;
        QString localFilePath = dir.absoluteFilePath(entry);
        if (QDir(localFilePath).exists())
        {
            addDirToZip(zipWriter, localFilePath, newZipPath);
        }
        else
        {
            addFileToZip(zipWriter, localFilePath, newZipPath);
        }
    }

}

bool MainWindow::zipFolder(const QString &folderPath, const QString &zipFilePath)
{
    //    QString sourceDirPath = "/path/to/source/folder"; // 替换为你的源文件夹路径
    //    QString zipFilePath = "/path/to/target/zipfile.zip"; // 替换为你的目标ZIP文件路径

    QFile zipFile(zipFilePath);
    if (!zipFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Cannot open zip file for writing:" << zipFilePath;
        return -1;
    }

    QZipWriter zipWriter(&zipFile);
    zipWriter.setCompressionPolicy(QZipWriter::AutoCompress); // 设置压缩策略
    addDirToZip(zipWriter, folderPath, ""); // 从根目录开始添加
    zipWriter.close();
    zipFile.close();

    qDebug() << "Folder successfully compressed to" << zipFilePath;
    QMessageBox::warning(this, "提示", "Folder successfully compressed to " + zipFilePath + "!");
    return true;
}

void MainWindow::ConnectToServer(QString IP, int Port, QString UserName, QString PassWord)
{
    qDebug() << "ConnectToServer";
}

// 连接SSh
void MainWindow::on_btnConnect_clicked()
{
    if (ui->ledIP->text().isEmpty() || ui->ledPort->text().isEmpty() || ui->ledUserName->text().isEmpty() || ui->ledPassword->text().isEmpty())
    {
        qDebug() << "isEmpty()";
        QMessageBox::warning(this, "提示", "请输入IP、端口、用户名和密码!");
    }
    else
    {
        // 连接命令
        QString strIp = "\\\\" + ui->ledIP->text();
        QString command = QString("net use %1 %2 /user:%3").arg("\\\\" + ui->ledIP->text(), ui->ledPassword->text(), ui->ledUserName->text());
        qDebug() << command;
        process.start("cmd", QStringList() << "/c" << command);
        process.waitForStarted();
        process.waitForFinished();

        m_strSource_pathDefault.replace("/", "\\");
        m_strSource_pathDefault.remove(0, 2);
        m_strSource_path = "\\\\" + ui->ledIP->text() + m_strSource_pathDefault;

        //        qDebug() << "m_strSource_path" << m_strSource_path;

        QString strTemp = QString::fromLocal8Bit(process.readAllStandardOutput());

        qDebug() << strTemp;

        if (strTemp.contains("成功"))
        {
            QMessageBox::warning(this, "提示", "连接成功!");
        }
        else
        {
            QMessageBox::warning(this, "提示", "连接失败!");
        }
    }
}

void MainWindow::on_btnCopy_clicked()
{
    if (!m_strDestination_path.isEmpty() && !m_strSource_path.isEmpty())
    {
        QDateTime startDateTime = ui->dateTimeBegin->dateTime();
        QDateTime endDateTime = ui->dateTimeEnd->dateTime();
        QStringList filesWithDatetimeRange = findFilesWithDatetimeRange(m_strSource_path, startDateTime, endDateTime);


        for (QString &file : filesWithDatetimeRange)
        {
            QString tempPath = file;
            QString strMkdirPath;
            QString strRemotePath;
            QString strLocalPath;

            QFileInfo fileInfo(tempPath);
            QString folder_path = fileInfo.absoluteDir().absolutePath();
            int index = 0;
            while ((index = folder_path.indexOf(m_strSource_path, index)) != -1)
            {
                folder_path.remove(index, m_strSource_path.length());
            }

            file.replace("/", "\\");
            folder_path.replace("/", "\\");

            strRemotePath = file;
            QString tempspath = m_strDestination_path;
            tempspath.replace("/", "\\");
            strLocalPath = tempspath;
            strMkdirPath = strLocalPath + folder_path;

            // 远程共享文件夹路径
            //    QString remotePath = "\\\\10.10.40.197\\App_Data\\testShare";
            //    QString remotePath = "\\\\10.10.40.197\\App_Data\\testShare\\2024-09-09\\2024-09-09_15-06-14";

            // 本地目标路径
            //    QString localPath = "D:\\develop\\test";
            //    QString makdirPath = "D:\\develop\\test\\testShare\\2024-09-09";

            //            qDebug() << "++++++" << strRemotePath << strLocalPath << strMkdirPath;
            qDebug() << "Copy" << strRemotePath << " to " << strLocalPath;
            downloadFile(strRemotePath, strLocalPath, strMkdirPath);
        }


        //        QMessageBox testMassage;
        //        testMassage.setText("已完成!");
        //    testMassage.setText(strTemp);
        //        testMassage.exec();
        QMessageBox::warning(this, "提示", "已完成!");
    }
    else
    {
        QMessageBox::warning(this, "提示", "请先连接服务器并选择保存路径!");
    }

}


void MainWindow::on_btnSelectPath_clicked()
{
    QString folder_path = QFileDialog().getExistingDirectory(this, "选择保存路径");
    if (!folder_path.isEmpty())
    {
        // 用户选择了路径，将路径显示在标签上
        ui->ledSavePath->clear();
        ui->ledSavePath->setText(folder_path);
        m_strDestination_path = ui->ledSavePath->text();
    }
    else
    {
        QMessageBox::warning(this, "提示", "您未选择任何路径!");
    }
}


void MainWindow::on_btnSave_clicked()
{
    QString folder_name = getFolderName(m_strDestination_path);
    qDebug() << "getFolderName(m_strDestination_path)" << getFolderName(m_strDestination_path);
    QFileInfo fileInfo(m_strDestination_path);
    QString folder_path = fileInfo.absoluteDir().absolutePath();
    qDebug() << "parent" << fileInfo.absoluteDir().absolutePath();

    if (!m_strDestination_path.isEmpty() && !folder_path.isEmpty())
    {
        // 压缩文件
        //        QString folderPath = "/path/to/source/folder"; // 要打包的文件夹路径
        //        QString zipFilePath = "/path/to/destination.zip"; // 输出的.zip文件路径
        QString zipFilePath =  folder_path + "/" + folder_name + ".zip";
        //        QString zipFilePath =  folder_path;

        qDebug() << "m_strDestination_path" << m_strDestination_path;
        qDebug() << "zipFilePath" << zipFilePath;
        // 参数1:源文件夹 例：D:/develop/Projects/pycharmProject/copyLog/TT
        // 参数2：打包后文件夹，例 D:/develop/Projects/pycharmProject/copyLog/TT.zip
        zipFolder(m_strDestination_path, zipFilePath);
        qDebug() << "文件夹 " + m_strDestination_path + " 已打包到 " + folder_path + "/" + folder_name + ".zip " + "(路径:" + folder_path + ")";
    }
    else
    {
        QMessageBox::warning(this, "提示", "保存失败!");
    }
}

