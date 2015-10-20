#include "consoleinterface.h"


const QString ConsoleInterface::m_colorRed = "\e[0,31m";
const QString ConsoleInterface::m_colorGreen = "\e[0,32m";
const QString ConsoleInterface::m_colorLBlue = "\e[0,34m";
const QString ConsoleInterface::m_colorStandard = "\e[0,0m";
const char ConsoleInterface::printOrder[] = {'i', 'p', 'u', 'k', 'l', 's', 'q', 'r', 't', 0};


// Constructor
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
    foreach (const QString line, help) {
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
void ConsoleInterface::printAccountList(const QList<Account> &accountList)
{
    if (accountList.isEmpty()) {
        outStream << 'n';
        return;
    }
    ColumnWidth tableLayout = getTableLayout(accountList);
    printTableHeader(accountList[0], tableLayout);
    foreach (const Account account, accountList) {
        printAccount(account, tableLayout);
    }
}

/**
 * Print parsed options.
 * @param optionTable
 */
void ConsoleInterface::printOptionTable(const QHash<char, QVariant> optionTable)
{
    Account account;
    QList<char> keyList = optionTable.keys();
    foreach (const char key, keyList) {
        QString keyName = Persistence::columnNameOfOption(key);
        QVariant value = optionTable.value(key);
        account.insert(keyName, value);
    }
    printSingleAccount(account);
}

/**
 * @brief ConsoleInterface::writeToFile
 * @param accountList
 * @param filepath
 */
void ConsoleInterface::writeToFile(const QList<Account> &accountList, const QString &filepath)
{
    QFile file(filepath);
    if (! file.open(QFile::ReadWrite)) {
        outStream << m_colorRed << "Could not open file !" << m_colorStandard << "\n";
        return;
    }
    QDataStream fileStream(&file);
    foreach (Account account, accountList) {
        QString record;
        foreach (QString column, m_printOrderList) {
            if (account.contains(column)) {
                record.append(column).append('=').append(account.value(column).toString()).append("  |  ");
            }
        }
        record .append('\n');
        fileStream << record;
    }
    file.close();

    printSuccessMsg(QString("File written.\n"));
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
    int index = 0;
    while (printOrder[index] != 0) {
        QString columnName = Persistence::columnNameOfOption(printOrder[index]);
        columnList << columnName;
        ++index;
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
void ConsoleInterface::printAccount(const Account &account, const ColumnWidth &columnWidth)
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
ColumnWidth ConsoleInterface::getTableLayout(const QList<Account> &accountList)
{
    ColumnWidth tableLayout;
    foreach (const Account account, accountList) {
        foreach (const QString column, account.keys()) {
            tableLayout.insertWidthValue(column, account.value(column));
        }
    }

    return tableLayout;
}
