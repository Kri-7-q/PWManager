#include "filepersistence.h"

/**
 * @brief FilePersistence::FilePersistence
 */
FilePersistence::FilePersistence() :
    m_errorCode(QFileDevice::FileError::NoError)
{

}

/**
 * @brief FilePersistence::error
 * @return
 */
QString FilePersistence::error() const
{
    switch (m_errorCode) {
    case QFileDevice::FileError::NoError:
        return QString("No error occurd.");
        break;
    case QFileDevice::FileError::ReadError:
        return QString("An error occurred when reading from the file.");
        break;
    case QFileDevice::FileError::WriteError:
        return QString("An error occurred when writing to the file.");
        break;
    case QFileDevice::FileError::FatalError:
        return QString("A fatal error occurred.");
        break;
    case QFileDevice::FileError::ResourceError:
        return QString("Out of resources (e.g., too many open files, out of memory, etc.)");
        break;
    case QFileDevice::FileError::OpenError:
        return QString("The file could not be opened.");
        break;
    case QFileDevice::FileError::AbortError:
        return QString("The operation was aborted.");
        break;
    case QFileDevice::FileError::TimeOutError:
        return QString("A timeout occurred.");
        break;
    case QFileDevice::FileError::UnspecifiedError:
        return QString("An unspecified error occurred.");
        break;
    case QFileDevice::FileError::RemoveError:
        return QString("The file could not be removed.");
        break;
    case QFileDevice::FileError::RenameError:
        return QString("The file could not be renamed.");
        break;
    case QFileDevice::FileError::PositionError:
        return QString("The position in the file could not be changed.");
        break;
    case QFileDevice::FileError::ResizeError:
        return QString("The file could not be resized.");
        break;
    case QFileDevice::FileError::PermissionsError:
        return QString("The file could not be accessed.");
        break;
    case QFileDevice::FileError::CopyError:
        return QString("The file could not be copied.");
        break;
    }
}

/**
 * @brief FilePersistence::persistReadableFile
 * @param filePath
 * @return
 */
bool FilePersistence::persistReadableFile(const QString &filePath, const QList<QVariantMap>& accountList)
{
    QFile file(filePath);
    if (! file.open(QIODevice::WriteOnly)) {
        m_errorCode = file.error();
        return false;
    }
    QTextStream outStream(&file);
    for (int index=0; index<accountList.size(); ++index) {
        const QVariantMap& account = accountList.at(index);
        QStringList keyList = account.keys();
        for (int keyIndex=0; keyIndex<keyList.size(); ++keyIndex) {
            const QVariant value = account.value(keyList[keyIndex]);
            if (! value.isValid()) {
                continue;
            }
            QString line = keyList[keyIndex] + ": " + value.toString();
            outStream << line << endl;
        }
        outStream << QString("-----------------------------------------") << endl;
    }

    return true;
}
