#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFileDialog>
#include <QDir>
#include <QVector>
#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QProcess>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(0);
    ui->progressBar->setMaximum(100);


    QRegularExpression regex("^[a-zA-Z]:\\\\([^\\\\/:*?\"<>|]+\\\\)*$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit->setValidator(validator);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, open_button);
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, open_button);
    connect(ui->lineEdit_3, &QLineEdit::textChanged, this, open_button);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::open_button(){
    if(!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty() && !ui->lineEdit_3->text().isEmpty() && QDir(ui->lineEdit->text()).exists()){
        ui->pushButton_2->setEnabled(1);
    }else ui->pushButton_2->setEnabled(0);
}

void MainWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        ui->lineEdit->setText(dir);
    }

}


void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this, "Information", "Удаляет файлы, которые СОДЕРЖАТ в имени определенную последовательность символов.");
}



int deleteProcess(QVector<QString>&, QVector<QString>&, QString&, QProgressBar*);
int toDeleteComplAccurate(QString&, QString&, QString&, QLabel*, QProgressBar*);
int toDeleteIncomplAccurate(QString&, QString&, QString&, QLabel*, QProgressBar*);

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox question(this);
    question.setWindowTitle("Confirmation");
    question.setText("Продолжить удаление объектов?");
    question.setIcon(QMessageBox::Question);
    question.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    question.setDefaultButton(QMessageBox::No);

    if(question.exec() == QMessageBox::Yes){

        QString path = ui->lineEdit->text();
        QString fileName = ui->lineEdit_2->text();
        QString userName = ui->lineEdit_3->text();
        Qt::CheckState isAccurate = ui->checkBox->checkState();
        int count = 0;
        if(isAccurate == Qt::Unchecked){
            count = toDeleteComplAccurate(path, fileName, userName, ui->label_4, ui->progressBar);
        }else{
            count = toDeleteIncomplAccurate(path, fileName, userName, ui->label_4, ui->progressBar);
        }


        QMessageBox ans(this);
        ans.setWindowTitle("Undeleted Files");
        ans.setText("Количество неудаленных объектов: " + QString::number(count));
        ans.exec();
    }
}


int toDeleteComplAccurate(QString& path, QString& fileName, QString& userName, QLabel* info, QProgressBar* progressBar){
    info->setText("Поиск объектов");
    QVector<QString> foldersToDelete;
    QVector<QString> filesToDelete;

    try{


        QDirIterator it(path, QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);

        while(it.hasNext()){
            QApplication::processEvents();
            it.next();
            QFileInfo fileInfo(it.filePath());

            if(fileInfo.isFile() || fileInfo.isDir()){

                QString nameOfCurrentFile = fileInfo.baseName();
                QString CurrentFilePath = fileInfo.filePath();

                if(QFileInfo(CurrentFilePath).absolutePath().contains("\\Windows", Qt::CaseSensitive)) continue;

                if(nameOfCurrentFile == "Program Files" || nameOfCurrentFile == "Program Files (x86)" ||
                    nameOfCurrentFile == "PerfLogs" || nameOfCurrentFile == userName) continue;

                if(nameOfCurrentFile == fileName){
                    if(fileInfo.isDir()){
                        foldersToDelete.append(CurrentFilePath);
                    }
                    if(fileInfo.isFile()){
                        filesToDelete.append(CurrentFilePath);
                    }
                }
            }

        }

    }
    catch(const std::exception& exc){
        qDebug() << "Exception: " << exc.what();
        return 1;
    }

    info->setText("Удаление объектов");

    int count = deleteProcess(filesToDelete, foldersToDelete, userName, progressBar);

    filesToDelete.clear();
    foldersToDelete.clear();


    info->setText("");
    progressBar->setValue(0);
    progressBar->setVisible(0);
    return count;

}

int toDeleteIncomplAccurate(QString& path, QString& fileName, QString& userName, QLabel* info, QProgressBar* progressBar){
    info->setText("Поиск объектов");
    QVector<QString> foldersToDelete;
    QVector<QString> filesToDelete;

    try{


        QDirIterator it(path, QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);

        while(it.hasNext()){
            QApplication::processEvents();
            it.next();
            QFileInfo fileInfo(it.filePath());

            if(fileInfo.isFile() || fileInfo.isDir()){

                QString nameOfCurrentFile = fileInfo.baseName();
                QString CurrentFilePath = fileInfo.filePath();

                if(QFileInfo(CurrentFilePath).absolutePath().contains("\\Windows", Qt::CaseSensitive)) continue;

                if(nameOfCurrentFile == "Program Files" || nameOfCurrentFile == "Program Files (x86)" ||
                    nameOfCurrentFile == "PerfLogs" || nameOfCurrentFile == userName) continue;

                if(nameOfCurrentFile.contains(fileName)){
                    if(fileInfo.isDir()){
                        foldersToDelete.append(CurrentFilePath);
                    }
                    if(fileInfo.isFile()){
                        filesToDelete.append(CurrentFilePath);
                    }
                }
            }

        }

    }
    catch(const std::exception& exc){
        qDebug() << "Exception: " << exc.what();
        return 1;
    }

    info->setText("Удаление объектов");

    int count = deleteProcess(filesToDelete, foldersToDelete, userName, progressBar);

    filesToDelete.clear();
    foldersToDelete.clear();


    info->setText("");
    progressBar->setValue(0);
    progressBar->setVisible(0);
    return count;

}


int deleteProcess(QVector<QString>& filesToDelete, QVector<QString>& foldersToDelete, QString& userName, QProgressBar* progressBar){
    QApplication::processEvents();
    progressBar->setVisible(1);
    int countOfUndeleted = 0;
    int countOfDeleted = 0;
    int filesCount = filesToDelete.size();
    int dirsCount = foldersToDelete.size();

    QString currentBachPath = QCoreApplication::applicationDirPath() + "/toAdmin.bat";


    for(QString path: foldersToDelete){
        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);

        process.startCommand(currentBachPath + " \"" + path.replace("/", "\\") + "\" " + userName);
        if(!process.waitForFinished()){
            countOfUndeleted++;
        } else {
            int exitCode = process.exitCode();
            QString output = process.readAllStandardOutput();

            if (exitCode != 0) {
                countOfUndeleted++;
                qDebug() << "Failed to delete file:" << path << "Exit code:" << exitCode;
            } else {
                countOfDeleted++;
                qDebug() << "Successfully deleted file:" << path;
            }

            qDebug() << "Output:" << output;
        }
        progressBar->setValue((double)(countOfDeleted+countOfUndeleted) / (filesCount+dirsCount) * 100);
        QApplication::processEvents();
    }
    for(QString path: filesToDelete){
        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.startCommand(currentBachPath + " \"" + path.replace("/", "\\") + "\" " + userName);
        if(!process.waitForFinished()){
            countOfUndeleted++;
        } else {
            int exitCode = process.exitCode();
            QString output = process.readAllStandardOutput();

            if (exitCode != 0) {
                countOfUndeleted++;
                qDebug() << "Failed to delete file:" << path << "Exit code:" << exitCode << '\n';
            } else {
                countOfDeleted++;
                qDebug() << "Successfully deleted file:" << path << '\n';
            }

            qDebug() << "Output:" << output << '\n';
        }
        progressBar->setValue((double)(countOfDeleted+countOfUndeleted) / (filesCount+dirsCount) * 100);
        QApplication::processEvents();
    }

    return countOfUndeleted;
}





