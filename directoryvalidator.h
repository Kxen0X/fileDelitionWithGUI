#ifndef DIRECTORYVALIDATOR_H
#define DIRECTORYVALIDATOR_H
#include <QValidator>

class DirectoryValidator: public QValidator
{
public:
    DirectoryValidator(QObject* parent = nullptr);
    State validate(QString&, int&) const override;
};

#endif // DIRECTORYVALIDATOR_H
