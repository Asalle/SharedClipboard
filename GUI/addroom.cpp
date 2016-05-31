#include "addroom.h"
#include "ui_addroom.h"

AddRoom::AddRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRoom)
{
    ui->setupUi(this);
}

AddRoom::~AddRoom()
{
    delete ui;
}

void AddRoom::on_buttonBox_accepted()
{
    if (
            ui->name->text() == "" ||
            ui->pass->text() == "" ||
            ui->pass2->text() == ""
       ){
        ui->errorLabel->setText("Error occured");
    } else if (ui->pass->text() != ui->pass2->text()){
        ui->errorLabel->setText("Error occured");
    } else {
        emit newRoom(ui->name->text(), ui->loginLineEdit->text());
        emit newPass(ui->pass->text());
    }
}
