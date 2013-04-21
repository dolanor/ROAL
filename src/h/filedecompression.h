#ifndef FILE_DECOMPRESSION_H
#define FILE_DECOMPRESSION_H

#include <QThread>

#ifdef Q_OS_WIN
#include <direct.h>
#endif

#include <archive.h>
#include <archive_entry.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

class FileDecompression : public QThread
{
    public:
        /**
         * @brief Decompressing archives
         * @param _file The path with the file name
         */
        FileDecompression();

        /**
         * @brief Start decompressing
         */
        void run();

        void setFile(QString _file, QString _installPath);

    private:

        QString file;
        QString installPath;
        std::ofstream log;

        int copy_data(struct archive *_ar, struct archive *_aw);

        void extract();
};

#endif // FILE_DECOMPRESSION_H
