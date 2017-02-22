#include "commandprocessor.h"
#include "Persistence/filepersistence.h"
#include "SearchAccount/matchstring.h"
#include "Utility/sortlist.h"
#include "SearchAccount/matchobject.h"

/**
 * @brief CommandProcessor::CommandProcessor
 * @param iface
 * @param m_pDatabase
 */
CommandProcessor::CommandProcessor(ConsoleInterface &iface, Persistence *database) :
    m_userInterface(iface),
    m_pDatabase(database)
{

}

/**
 * @brief CommandProcessor::process
 * @param command
 * @param optionTable
 */
void CommandProcessor::process(AppCommand::Command command, OptionTable &optionTable)
{
    switch (command) {
    case AppCommand::New: {
        if (!optionTable.contains('k')) {
            PwGenerator pwGenerator;
            int passwordLength = optionTable.value('l').toInt();
            QString characterDefinition = optionTable.value('s').toString();
            QString password = pwGenerator.passwordFromDefinition(static_cast<ushort>(passwordLength), characterDefinition);
            if (pwGenerator.hasError()) {
                m_userInterface.printError(pwGenerator.errorMessage());
                return;
            }
            optionTable.insert('k', QVariant(password));
        }
        optionTable.insert('t', QVariant(QDateTime::currentDateTime()));
        bool result = m_pDatabase->persistAccountObject(optionTable);
        if (result) {
            m_userInterface.printSuccessMsg("Account successfully persisted.\n");
            QVariantMap account = m_pDatabase->findAccount(optionTable);
            m_userInterface.printSingleAccount(account);
        } else {
            m_userInterface.printError("Could not store new Account !");
            m_userInterface.printError(m_pDatabase->error());
        }
        break;
    }
    case AppCommand::Show: {
        QList<QVariantMap> list = m_pDatabase->findAccountsLike(optionTable);
        if (m_pDatabase->hasError()) {
            m_userInterface.printError(m_pDatabase->error());
            break;
        }
        m_userInterface.printAccountList(list);
        break;
    }
    case AppCommand::Remove: {
        int rowsRemoved = m_pDatabase->deleteAccountObject(optionTable);
        if (m_pDatabase->hasError()) {
            m_userInterface.printError(m_pDatabase->error());
        } else {
            QString msg = QString("%1 accounts removed from m_pDatabase.\n").arg(rowsRemoved);
            m_userInterface.printSuccessMsg(msg);
        }
        break;
    }
    case AppCommand::Modify: {
        optionTable.insert('t', QDateTime::currentDateTime());
        if (m_pDatabase->modifyAccountObject(optionTable)) {
            m_userInterface.printSuccessMsg("Account object successfully updated.\n");
            QVariantMap account = m_pDatabase->findAccount(optionTable);
            m_userInterface.printSingleAccount(account);
        } else {
            m_userInterface.printError("Account could not be updated !\n");
            m_userInterface.printError(m_pDatabase->error());
        }
        break;
    }
    case AppCommand::GeneratePW: {
        // If not have new password definition then get it from m_pDatabase.
        if (! optionTable.contains('l') || ! optionTable.contains('s')) {
            OptionTable searchObj(optionTable);
            searchObj.insert('l', QVariant());
            searchObj.insert('s', QVariant());
            QVariantMap pwDefinition = m_pDatabase->findAccount(searchObj);
            if (pwDefinition.isEmpty()) {
                m_userInterface.printError("Could not read password definition.\n");
                return;
            }
            if (! optionTable.contains('l')) {
                optionTable.insert('l', pwDefinition.value(m_pDatabase->optionToRealName('l')));
            }
            if (! optionTable.contains('s')) {
                optionTable.insert('s', pwDefinition.value(m_pDatabase->optionToRealName('s')));
            }
        }
        int length = optionTable.value('l').toInt();
        QString definition = optionTable.value('s').toString();
        PwGenerator generator;
        QString password = generator.passwordFromDefinition(length, definition);
        if (generator.hasError()) {
            m_userInterface.printError(generator.errorMessage());
            return;
        }
        optionTable.insert('k', password);
        optionTable.insert('t', QDateTime::currentDateTime());
        if (m_pDatabase->modifyAccountObject(optionTable)) {
            m_userInterface.printSuccessMsg("New password generated and stored into m_pDatabase.\n");
        } else {
            m_userInterface.printError("Could not store new password into m_pDatabase !\n");
            m_userInterface.printError(m_pDatabase->error());
        }
        break;
    }
    case AppCommand::File:
        // Store data to file.
        if (optionTable.contains('o')) {
            if (optionTable.contains('v')) {
                // Human readable file.
                QList<QVariantMap> accountList = m_pDatabase->allPersistedAccounts();
                FilePersistence filePersist;
                filePersist.persistReadableFile(optionTable.value('f').toString(), accountList);
            } else {
                // Store data NOT human readable.
            }
        }
        // Read data from file.
        if (optionTable.contains('g')) {
            // ...
        }
        break;
    case AppCommand::Find: {
        QStringList list = optionTable.value('?').toStringList();
        QString searchMask = (list.isEmpty()) ? QString() : list[0];
        if (searchMask.length() < 3) {
            m_userInterface.printError("Search mask must have at least three symbols !");
            break;
        }
        OptionTable readProvider;
        readProvider.insert('i', QVariant());
        readProvider.insert('p', QVariant());
        QList<QVariantMap> providerList = m_pDatabase->findAccountsLike(readProvider);
        MatchString match(searchMask.toLower());
        SortList<MatchObject> sortList;
        for (int index=0; index<providerList.size(); ++index) {
            QString key = m_pDatabase->optionToRealName('p');
            QString text = providerList[index].value(key, QVariant()).toString();
            int result = match.matchText(text.toLower());
            if (result > 0) {
                sortList.insert(MatchObject(index, result));
            }
            match.reset();
        }
        QList<QVariantMap> matchList;
        if (!sortList.isEmpty()) {
            SortListIterator<MatchObject> iterator = sortList.end();
            do {
                int index = iterator.data().index();
                matchList << providerList[index];
            } while(iterator.previous());
        }
        m_userInterface.printAccountList(matchList);

        break;
    }
    default:
        break;
    }
}
