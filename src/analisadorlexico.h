#ifndef ANALISADORLEXICO_H
#define ANALISADORLEXICO_H

#include <QString>
#include <QVector>
#include <QStringList>
#include "token.h"

class AnalisadorLexico
{
public:
    explicit AnalisadorLexico(const QString& codigoFonte);

    bool analisar(QVector<Token>& tokens, QString& erro) const;

private:
    QString codigoFonte;
};

#endif // ANALISADORLEXICO_H
