#include "sharedclipboard.h"
#include "ui_sharedclipboard.h"

SharedClipboard::SharedClipboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SharedClipboard)
{
    ui->setupUi(this);
}

SharedClipboard::~SharedClipboard()
{
    delete ui;
}
