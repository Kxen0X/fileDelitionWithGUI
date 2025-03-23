#include "directoryvalidator.h"
#include <QDir>
#include <QRegularExpression>

DirectoryValidator::DirectoryValidator(QObject* parent): QValidator(parent) {}
QValidator::State DirectoryValidator::validate(QString& input, int& pos) const{

    if (input.isEmpty()) {
        return Intermediate;
    }

    QRegularExpression diskRegex("^[A-Za-z]:\\\\");

    if (!diskRegex.match(input).hasMatch()) {
        return Invalid;
    }

    QDir dir(input);
    if (dir.exists()) {
        return Acceptable;
    } else {

        if (input.endsWith("\\")) {

            return Intermediate;
        }

        QString parentPath = input.left(input.lastIndexOf("\\"));
        QDir parentDir(parentPath);
        if(parentDir.exists()){
            return Intermediate;
        }

        return Invalid;
    }
}
