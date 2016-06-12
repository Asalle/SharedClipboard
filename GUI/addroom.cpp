#include "addroom.h"
#include "ui_addroom.h"

AddRoom::AddRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRoom)
{
    ui->setupUi(this);
    ui->addbtn->setDefault(true);

    QRegExp textMidlen = QRegExp("([a-z]|[A-Z]|[0-9]){1,25}");
    QValidator * textTo25Symb = new QRegExpValidator(textMidlen);

    ui->name->setValidator(textTo25Symb);
    ui->pass->setValidator(textTo25Symb);
    ui->pass2->setValidator(textTo25Symb);
}

AddRoom::~AddRoom()
{
    delete ui;
}

void AddRoom::on_addbtn_clicked()
{
    ui->errorLabel->setText("");
    if (
            ui->name->text() == "" ||
            ui->pass->text() == "" ||
            ui->pass2->text() == ""
       ){
        ui->errorLabel->setText("Error occured");
    } else if (ui->pass->text() != ui->pass2->text()){
        ui->errorLabel->setText("Error occured");
    } else {
        emit newRoom(ui->name->text());
        emit newPass(ui->pass->text());
        this->close();
    }
}

void AddRoom::on_cancelbtn_clicked()
{
    this->close();
}
