#include "../h/firstrun.h"
#include "ui_firstrun.h"

FirstRun::FirstRun(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirstRun)
{
    ui->setupUi(this);
}

FirstRun::~FirstRun()
{
    delete ui;
}

void FirstRun::on_qpTorrent_clicked()
{
    emit downloadMethodSelected(false);
}

void FirstRun::on_qbHTTP_clicked()
{
    emit downloadMethodSelected(true);
}
