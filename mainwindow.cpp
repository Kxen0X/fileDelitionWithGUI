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


int deleteProcess(QVector<QString>, QProgressBar*);
int toDelete(QString&, QString&, QString&, QLabel*, QProgressBar*);


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
    toDelete(path, fileName, userName, ui->label_4, ui->progressBar);

    }
}


int toDelete(QString& path, QString& fileName, QString& userName, QLabel* info, QProgressBar* progressBar){
    info->setText("Поиск объектов");
    QVector<QString> objectsToDelete;

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

                    objectsToDelete.append(CurrentFilePath);
                }
            }

        }

    }
    catch(const std::exception& exc){
        qDebug() << "Exception: " << exc.what();
        return 1;
    }

    info->setText("Удаление объектов");

    int count = deleteProcess(objectsToDelete, progressBar);

    objectsToDelete.clear();

    info->setText("");

    return 0;

}


int deleteProcess(QVector<QString> objectsToDelete, QProgressBar* progressBar){
    int countOfUndeleted = 0;
    int sz = objectsToDelete.size();
    int countOfDeleted = 0;
    foreach(QString path, objectsToDelete){

        QApplication::processEvents();
        QFileInfo fileInfo(path);
        if(fileInfo.isFile()){
            if(QFile::remove(path)){
                countOfDeleted++;
                progressBar->setValue((double)countOfDeleted/sz*100);
            }else countOfUndeleted++;
        }
        if(fileInfo.isDir()){
            QDir dir(path);
            if(dir.rmdir(path)){
                countOfDeleted++;
                progressBar->setValue((double)countOfDeleted/sz*100);
            }else countOfUndeleted++;

        }
    }

    return countOfUndeleted;
}
