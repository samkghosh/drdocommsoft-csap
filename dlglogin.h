#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>

namespace Ui {
class DlgLogin;
}

class DlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLogin(QWidget *parent = nullptr);
    ~DlgLogin();

private slots:
    void on_btnLoginOK_clicked();

private:
    Ui::DlgLogin *ui;
};

#endif // DLGLOGIN_H
