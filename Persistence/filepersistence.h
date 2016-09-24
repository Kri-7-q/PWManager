#ifndef FILEPERSISTENCE_H
#define FILEPERSISTENCE_H

#include <QFile>
#include <QTextStream>

class FilePersistence
{
public:
    FilePersistence();

    QString error() const;
    bool persistReadableFile(const QString& filePath, const QList<QVariantMap> &accountList);

private:
    QFileDevice::FileError m_errorCode;
};

#endif // FILEPERSISTENCE_H
