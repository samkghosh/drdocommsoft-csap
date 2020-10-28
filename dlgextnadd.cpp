#include "dlgextnadd.h"
#include "ui_dlgextnadd.h"

DlgExtnAdd::DlgExtnAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExtnAdd)
{
    ui->setupUi(this);
    setWindowTitle("Extenion Add/Modify From");
}

DlgExtnAdd::~DlgExtnAdd()
{
    delete ui;
}
