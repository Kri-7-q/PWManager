#include "consoleinterface.h"

ConsoleInterface::ConsoleInterface() :
    outStream(stdout)
{
    m_printOrderList = getPrintOrderForColumns();
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
 * Print a single Account object to console.
 * Calculates the width for each column to print
 * and the tables total width.
 * Draws a header and the Account object.
 * @param account
 */
void ConsoleInterface::printSingleAccount(const Account &account)
{
    ColumnWidth columnWidth;
    QStringList keyList = account.keys();
    for (QString key : keyList) {
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
    for (QString line : help) {
        outStream << line;
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
void ConsoleInterface::printAccountList(const QList<Account> &accountList, const ColumnWidth &columnWidth)
{
    if (accountList.isEmpty()) {
        outStream << 'n';
        return;
    }
    printTableHeader(accountList[0], columnWidth);
    for (Account account : accountList) {
        printAccount(account, columnWidth);
    }
}

/**
 * Print parsed options.
 * @param optionTable
 */
void ConsoleInterface::printOptionTable(const QHash<QString, QString> optionTable, const int columnWidth)
{
    QString space;
    QStringList keyList = optionTable.keys();
    outStream << '\n';
    for (QString key : keyList) {
        int needSpace = columnWidth - key.length();
        if (needSpace > 0) {
            space = QString(needSpace, ' ');
        } else {
            space = "";
        }
        outStream << key << space << " : " << optionTable.value(key) << '\n';
    }
    outStream << '\n';
}

/**
 * Get the print order of columns.
 * The option character are translated to column names.
 * Those column names will be stored in a list and in
 * the order to print.
 * @return
 */
QStringList ConsoleInterface::getPrintOrderForColumns()
{
    QStringList columnList;
    int length = strlen(printOrder);
    for (int i=0; i<length; ++i) {
        QString column = Account::databaseNameOfOption(printOrder[i]);
        columnList << column;
    }

    return columnList;
}

/**
 * Print an output header.
 * @param account
 * @param columnWidth
 */
void ConsoleInterface::printTableHeader(const Account &account, const ColumnWidth &columnWidth)
{
    QChar line('-');
    QChar space(' ');
    QString horzLine = QString(columnWidth.totalWidth(), line).append('\n');
    outStream << '\n';
    outStream << horzLine;
    outStream << "|";
    for (QString column : m_printOrderList) {
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
void ConsoleInterface::printAccount(const Account &account, const ColumnWidth &columnWidth)
{
    QChar line('-');
    QChar space(' ');
    QString horzLine = QString(columnWidth.totalWidth(), line).append('\n');
    outStream << '|';
    for (QString column : m_printOrderList) {
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
