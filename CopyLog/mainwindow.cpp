#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QTimer>

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setStyleSheet("QProgressBar { border-radius: 10px; height: 25px; text-align: center; color: #0000FF; background-color: #FFFFFF; border: 1px solid #000000;} QProgressBar::chunk { border-radius: 10px; background-color: #05B8EE;}");
    connect(this, &MainWindow::FileNums, this, [&](int FileNums)
    {
        if (m_nTotalFile != 0)
        {
            ui->progressBar->setValue((double)FileNums / m_nTotalFile * 100); // 设置进度条的当前值
            ui->progressBar->show();
        }
    });
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

QString MainWindow::getFolderName(const QString &folderPath)
{
    QString folderName = QDir(folderPath).dirName();
    return folderName;
}

QStringList MainWindow::traverseDirectory(const QString &directoryPath)
{
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
    QString commandmk =  QString("mkdir %1").arg(makdirPath) + "\n";
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

    QString strTemp = QString::fromLocal8Bit(process.readAllStandardOutput());

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

        m_strRemoteSource_pathDefault.replace("/", "\\");
        m_strRemoteSource_pathDefault.remove(0, 2);
        m_strSource_path = "\\\\" + ui->ledIP->text() + m_strRemoteSource_pathDefault;

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

        m_nTotalFile = filesWithDatetimeRange.size();
        int FileNum = 0;
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
            FileNum++;
            emit FileNums(FileNum);
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
        QMessageBox::warning(this, "提示", "压缩失败!");
    }
}

