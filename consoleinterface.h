#ifndef CONSOLEINTERFACE_H
#define CONSOLEINTERFACE_H

#include "account.h"
#include "columnwidth.h"
#include "persistence.h"
#include <QTextStream>

class ConsoleInterface
{
public:
    ConsoleInterface();

    void printError(const QString &errorMsg);
    void printSingleAccount(const Account &account);
    void printHelp(const QStringList &help);
    void printSuccessMsg(const QString &message);
    void printAccountList(const QList<Account> &accountList, const ColumnWidth &columnWidth);
    void printOptionTable(const QHash<QString, QString> optionTable, const int columnWidth);

private:
    QTextStream outStream;
    const QString m_colorRed = "\e[0,31m";
    const QString m_colorGreen = "\e[0,32m";
    const QString m_colorLBlue = "\e[0,34m";
    const QString m_colorStandard = "\e[0,0m";
    const char *printOrder = "ipuklsqrt";
    QStringList m_printOrderList;

    // Methods
    QStringList getPrintOrderForColumns();
    void printTableHeader(const Account &account, const ColumnWidth &columnWidth);
    void printAccount(const Account &account, const ColumnWidth &columnWidth);
};

#endif // CONSOLEINTERFACE_H
