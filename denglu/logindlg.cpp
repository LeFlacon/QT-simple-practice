#include "logindlg.h"
#include "ui_logindlg.h"
#include<QMessageBox>

loginDlg::loginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDlg)
{
    ui->setupUi(this);
}

loginDlg::~loginDlg()
{
    delete ui;
}

void loginDlg::on_loginbtn_clicked()
{
    if(ui->usrlinedit->text()==tr("666")&&ui->pwdlineedit->text()==tr("123456"))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("user name or password error!"),QMessageBox::Yes);
        ui->usrlinedit->clear();
        ui->pwdlineedit->clear();
        ui->usrlinedit->setFocus();
    }
}
