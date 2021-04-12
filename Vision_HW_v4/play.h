#ifndef PLAY_H
#define PLAY_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "QSettings"
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
namespace Ui {
class PLAY;

}
class PLAY : public QDialog
{
    Q_OBJECT

public:
    explicit PLAY(QWidget *parent = nullptr);
    ~PLAY();

    QString LOGO_DIR;
    int LOT_NUM;
    QString LOT_TEMP;
    QString Key_input;
    QString Key_input_temp;

signals:

      void CHECK_END(void);

private slots:
    void keyin_timeout();
    void LOT_END_SIG();
    void OK();
private:
    QString DATE_TEMP;
    Ui::PLAY *ui;
    QTimer *key_input_timeout;
    bool NO_BT;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // PLAY_H
