#ifndef CONSOLEINTERFACE_H
#define CONSOLEINTERFACE_H

/* ----------------------------------------------------------------------
 * Class ConsoleInterface
 * ----------------------------------------------------------------------
 * Provides functions to output messages, help text and results.
 */

#include "columnwidth.h"
#include "persistence.h"
#include <QTextStream>

typedef QHash<QString, QVariant> Account;

class ConsoleInterface
{
public:
    ConsoleInterface();

    void printError(const QString &errorMsg);
    void printSingleAccount(const Account &account);
    void printHelp(const QStringList &help);
    void printSuccessMsg(const QString &message);
    void printAccountList(const QList<Account> &accountList);
    void printOptionTable(const QHash<char, QVariant> optionTable);

private:
    QTextStream outStream;
    QStringList m_printOrderList;
    // const static
    static const QString m_colorRed;
    static const QString m_colorGreen;
    static const QString m_colorLBlue;
    static const QString m_colorStandard;
    static const char printOrder[];

    // Methods
    QStringList getPrintOrderForColumns();
    void printTableHeader(const Account &account, const ColumnWidth &columnWidth);
    void printAccount(const Account &account, const ColumnWidth &columnWidth);
    ColumnWidth getTableLayout(const QList<Account> &accountList);
};

#endif // CONSOLEINTERFACE_H
