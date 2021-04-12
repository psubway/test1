#include "setting_form.h"


setting_form::setting_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::setting_form)
{
    ui->setupUi(this);
}

setting_form::~setting_form()
{
    delete ui;
}
