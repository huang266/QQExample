#include "registr.h"
#include "ui_registr.h"

Registr::Registr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registr)
{
    ui->setupUi(this);
}

registr::~registr()
{
    delete ui;
}
void registr::paintEvent(QPaintEvent *enent){

}
