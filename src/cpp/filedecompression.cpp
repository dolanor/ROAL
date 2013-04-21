#include "../h/filedecompression.h"
#include <QDebug>

FileDecompression::FileDecompression()
{
    log.open("launcher.log");
}

void FileDecompression::run()
{
    // Change dir
#ifdef Q_OS_WIN
    int chdir = _chdir(installPath.toStdString().c_str());
    ////QMessageBox::warning(NULL,"",QString::number(chdir));
    if(chdir == 0)
#else
    if(chdir(installPath.toStdString().c_str()) == 0)
#endif
    {
        //////QMessageBox::warning(NULL,"",QString::number(chdir));
        // Extract
        extract();
    }
}

void FileDecompression::setFile(QString _file, QString _installPath)
{
#ifdef Q_OS_WIN
    file = _file;
    installPath = _installPath.replace("/","\\\\") + "game\\\\data\\\\";

    log << file.toStdString().c_str() << "\n";
#else
    file = _file;
    installPath = _installPath + "game/data/";
#endif
}

int FileDecompression::copy_data(struct archive *_ar, struct archive *_aw)
{
    int r;
    const void *buff;
    size_t size;

#ifdef Q_OS_WIN
    long long offset;
#else
    off_t offset;
#endif

    //QMessageBox::warning(NULL,"","Extracting");

    for (;;) {
        r = archive_read_data_block(_ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
        {
            //QMessageBox::warning(NULL,"","GOOD");
            return (ARCHIVE_OK);
        }
        if (r != ARCHIVE_OK)
        {
            //QMessageBox::warning(NULL,"","Bad");
            return (r);
        }
        r = archive_write_data_block(_aw, buff, size, offset);
        if (r != ARCHIVE_OK)
        {
            //QMessageBox::warning(NULL,"","BAD BAD");
            fprintf(stderr, "%s\n", archive_error_string(_aw));
            return (r);
        }
    }
}

void FileDecompression::extract()
{
    log << "Start" << "\n";

    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_filename(a, file.toStdString().c_str(), 10240)))
        exit(1);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
        {
            log << "EOF" << "\n";
            break;
        }
        if (r != ARCHIVE_OK)
        {
            log << archive_error_string(a) << "\n";
            fprintf(stderr, "%s\n", archive_error_string(a));
        }
        if (r < ARCHIVE_WARN)
            exit(1);
        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK)
        {
            log << archive_error_string(ext) << "\n";
            fprintf(stderr, "%s\n", archive_error_string(ext));
        }
        else if (archive_entry_size(entry) > 0)
        {
            log << "COPY" << "\n";
            copy_data(a, ext);
            if (r != ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            exit(1);
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

    // Change dir
#ifdef Q_OS_WIN
#else
    if(chdir(QString(installPath + "launcher/bin/").toStdString().c_str()))
#endif
    {
    }
}
