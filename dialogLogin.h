#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class DlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLogin(QWidget *parent = nullptr);
    ~DlgLogin();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
