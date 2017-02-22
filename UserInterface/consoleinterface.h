#ifndef CONSOLEINTERFACE_H
#define CONSOLEINTERFACE_H

/* ----------------------------------------------------------------------
 * Class ConsoleInterface
 * ----------------------------------------------------------------------
 * Provides functions to output messages, help text and results.
 */

#include "columnwidth.h"
#include "Utility/sortlist.h"
#include "SearchAccount/matchobject.h"
#include <QTextStream>

class ConsoleInterface
{
public:
    ConsoleInterface();

    void setPrintOrderList(const QStringList& list);

    void printError(const QString &errorMsg);
    void printWarnings(const QString& warnings);
    void printSingleAccount(const QVariantMap &account);
    void printHelp(const QStringList &help);
    void printSuccessMsg(const QString &message);
    void printAccountList(const QList<QVariantMap> &accountList);
    void printSearchMatches(const SortList<MatchObject>& sortedList);

private:
    QTextStream outStream;
    QStringList m_printOrderList;
    // const static
    static const QString m_colorRed;
    static const QString m_colorGreen;
    static const QString m_colorLBlue;
    static const QString m_colorStandard;
    static const QString m_colorBraun;

    // Methods
    void printTableHeader(const QVariantMap &account, const ColumnWidth &columnWidth);
    void printAccount(const QVariantMap &account, const ColumnWidth &columnWidth);
    ColumnWidth getTableLayout(const QList<QVariantMap> &accountList);
};

#endif // CONSOLEINTERFACE_H
