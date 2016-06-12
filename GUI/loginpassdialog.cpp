#include "loginpassdialog.h"
#include "ui_loginpassdialog.h"

LoginPassDialog::LoginPassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPassDialog)
{
    ui->setupUi(this);
    ui->infolabel->setWordWrap(true);

    QRegExp textMidlen = QRegExp("([a-z]|[A-Z]|[0-9]){1,25}");
    QValidator * textTo25Symb = new QRegExpValidator(textMidlen);

    ui->loginLE->setValidator(textTo25Symb);
    ui->PassLE->setValidator(textTo25Symb);
}

LoginPassDialog::~LoginPassDialog()
{
    delete ui;
}

void LoginPassDialog::on_buttonBox_rejected()
{
    this->close();
}

void LoginPassDialog::on_buttonBox_accepted()
{
    emit credentials(ui->loginLE->text(), ui->PassLE->text());
}
