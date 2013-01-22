#include "../h/filevalidationthread.h"

void FileValidationThread::setData(QString _installationPath, QStringList *_files, QStringList *_hashes, QStringList *_sizes)
{
    files = _files;
    hashes = _hashes;
    sizes = _sizes;
    installationPath = _installationPath;
}

void FileValidationThread::run()
{
    QStringList cFiles, cHashes, cSizes;

    for(int i = 0; i < files->size(); i++)
    {
        QFile file(installationPath + files->at(i));

        if(file.open(QIODevice::ReadOnly))
        {
            // Get bytes
            QByteArray fileData = file.readAll();

            // Get hash
            QByteArray hash = QCryptographicHash::hash(fileData, QCryptographicHash::Sha256);

            // Compare
            if(QString(hash.toHex()) != hashes->at(i))
            {
                cFiles.append(files->at(i));
                cHashes.append(hashes->at(i));
                cSizes.append(sizes->at(i));
            }
        }
        else
        {
            cFiles.append(files->at(i));
            cHashes.append(hashes->at(i));
            cSizes.append(sizes->at(i));
        }
    }

    files->clear();
    hashes->clear();
    sizes->clear();

    for(int i = 0; i < cFiles.size(); i++)
    {
        files->append(cFiles.at(i));
        hashes->append(cHashes.at(i));
        sizes->append(cSizes.at(i));
    }

}

