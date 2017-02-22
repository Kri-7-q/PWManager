#include "consoleinterface.h"


const QString ConsoleInterface::m_colorRed = "\e[0,31m";
const QString ConsoleInterface::m_colorGreen = "\e[0,32m";
const QString ConsoleInterface::m_colorLBlue = "\e[0,34m";
const QString ConsoleInterface::m_colorStandard = "\e[0,0m";
const QString ConsoleInterface::m_colorBraun = "\e[0,33m";


// Constructor
ConsoleInterface::ConsoleInterface() :
    outStream(stdout)
{
    m_printOrderList << QString("id") << QString("provider") << QString("username");
}

// Setter
void ConsoleInterface::setPrintOrderList(const QStringList &list)
{
    m_printOrderList = list;
}

/**
 * Print an error message to console.
 * Print in text color red and adds a
 * new line character.
 */
void ConsoleInterface::printError(const QString &errorMsg)
{
    QString coloredMsg = m_colorRed + errorMsg + m_colorStandard;
    outStream << coloredMsg << '\n';
}

/**
 * Print warnings.
 * @param warnings
 */
void ConsoleInterface::printWarnings(const QString &warnings)
{
    QString coloredMsg = m_colorBraun + warnings + m_colorStandard;
    outStream << coloredMsg << '\n';
}

/**
 * Print a single Account object to console.
 * Calculates the width for each column to print
 * and the tables total width.
 * Draws a header and the Account object.
 * @param account
 */
void ConsoleInterface::printSingleAccount(const QVariantMap &account)
{
    if (account.isEmpty()) {
        return;
    }
    ColumnWidth columnWidth;
    QStringList keyList = account.keys();
    foreach (const QString key, keyList) {
        QVariant valueVariant = account.value(key);
        QString value = valueVariant.toString();
        columnWidth.insertWidthValue(key, value.length());
    }
    printTableHeader(account, columnWidth);
    printAccount(account, columnWidth);
    outStream << '\n';
}

/**
 * Print help text.
 * @param help
 */
void ConsoleInterface::printHelp(const QStringList &help)
{
    outStream << '\n';
    outStream << m_colorLBlue << help[0] << m_colorStandard;
    for(int index=1; index<help.size(); ++index) {
        outStream << help[index];
    }
    outStream << '\n';
}

/**
 * Print a success message in color green.
 * @param message
 */
void ConsoleInterface::printSuccessMsg(const QString &message)
{
    QString msg = QString(message).prepend(m_colorGreen).append(m_colorStandard);
    outStream << msg;
}

/**
 * Print a list of Account objects to console
 * including a header.
 * @param accountList
 * @param columnWidth
 */
void ConsoleInterface::printAccountList(const QList<QVariantMap> &accountList)
{
    if (accountList.isEmpty()) {
        outStream << m_colorGreen << "Nothing was found.\n" << m_colorStandard;
        return;
    }
    ColumnWidth tableLayout = getTableLayout(accountList);
    printTableHeader(accountList[0], tableLayout);
    foreach (const QVariantMap account, accountList) {
        printAccount(account, tableLayout);
    }
}

/**
 * Print an output header.
 * @param account
 * @param columnWidth
 */
void ConsoleInterface::printTableHeader(const QVariantMap &account, const ColumnWidth &columnWidth)
{
    QChar line('-');
    QChar space(' ');
    QString horzLine = QString(columnWidth.totalWidth(), line).append('\n');
    outStream << '\n';
    outStream << horzLine;
    outStream << "|";
    foreach (const QString column, m_printOrderList) {
        if (account.contains(column)) {
            QString fillSpace = QString(columnWidth.spaceToFillColumn(column, column), space);
            outStream << ' ' << m_colorLBlue << column << m_colorStandard << fillSpace << '|';
        }
    }
    outStream << '\n';
    outStream << horzLine;
}

/**
 * Print an account.
 * @param account
 * @param columnWidth
 */
void ConsoleInterface::printAccount(const QVariantMap &account, const ColumnWidth &columnWidth)
{
    QChar line('-');
    QChar space(' ');
    QString horzLine = QString(columnWidth.totalWidth(), line).append('\n');
    outStream << '|';
    foreach (const QString column, m_printOrderList) {
        if (account.contains(column)) {
            QVariant value = account.value(column);
            QString valueString = value.toString();
            QString fillSpace = QString(columnWidth.spaceToFillColumn(column, valueString), space);
            outStream << ' ' << valueString << fillSpace << '|';
        }
    }
    outStream << '\n';
    outStream << horzLine;
}

/**
 * Calculate column width of table.
 * Find longest entry to fit column width.
 * Minimum size is width of column header name.
 * @param accountList
 * @return
 */
ColumnWidth ConsoleInterface::getTableLayout(const QList<QVariantMap> &accountList)
{
    ColumnWidth tableLayout;
    foreach (const QVariantMap account, accountList) {
        foreach (const QString column, account.keys()) {
            tableLayout.insertWidthValue(column, account.value(column));
        }
    }

    return tableLayout;
}
