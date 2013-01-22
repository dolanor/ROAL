#ifndef FILEVALIDATIONTHREAD_H
#define FILEVALIDATIONTHREAD_H

#include <QThread>
#include <QStringList>
#include <QFile>
#include <QCryptographicHash>

class FileValidationThread : public QThread
{
    public:
        void setData(QString _installationPath, QStringList *_files, QStringList *_hashes, QStringList *_sizes);
        void run();
        bool getResult();

    private:
        bool result;
        QString installationPath;
        QStringList *files, *hashes, *sizes;
};

#endif // FILEVALIDATIONTHREAD_H
