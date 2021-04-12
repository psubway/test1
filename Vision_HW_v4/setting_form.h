#ifndef SETTING_FORM_H
#define SETTING_FORM_H

#include <QMainWindow>
#include "ui_setting_form.h"
namespace Ui {
class setting_form;
}

class setting_form : public QMainWindow
{
    Q_OBJECT

public:
    explicit setting_form(QWidget *parent = nullptr);
    ~setting_form();

private:
    Ui::setting_form *ui;
};

#endif // SETTING_FORM_H
