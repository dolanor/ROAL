#ifndef FIRSTRUN_H
#define FIRSTRUN_H

#include <QWidget>

namespace Ui {
    class FirstRun;
}

class FirstRun : public QWidget
{
        Q_OBJECT
        
    public:
        explicit FirstRun(QWidget *parent = 0);
        ~FirstRun();
        
    private slots:

        void on_qpTorrent_clicked();
        void on_qbHTTP_clicked();

    signals:
        void downloadMethodSelected(bool _method);


    private:
        Ui::FirstRun *ui;
};

#endif // FIRSTRUN_H
