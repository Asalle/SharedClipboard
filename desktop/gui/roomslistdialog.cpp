#include "roomslistdialog.h"
#include "ui_roomslistdialog.h"
#include "passworddialog.h"
#include "createroomdialog.h"

RoomsListDialog::RoomsListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomsListDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNewRoomButtonClicked()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onListItemChaned()));
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListItemDoubleClicked(QModelIndex)));
}

RoomsListDialog::~RoomsListDialog()
{
    delete ui;
}

void RoomsListDialog::setRoomsHash(QHash<QString, qint32> rooms)
{
    QList<QString> roomsList = rooms.keys();
    QList<QString>::iterator i;
    for(i = roomsList.begin(); i != roomsList.end(); ++i)
    {
        ui->listWidget->addItem(*i);
    }
}

void RoomsListDialog::onPasswordTyped(QString password)
{
    QString name = ui->listWidget->selectedItems()[0]->text();
    emit roomChoosed(rooms[name], password);
    close();
}

void RoomsListDialog::onNewRoomButtonClicked()
{
    if(ui->listWidget->count() != 0 && ui->listWidget->selectedItems()[0]->text() == localServer)
    {
        emit deleteServerRoom();
        return;
    }

    CreateRoomDialog dialog;
    connect(&dialog, SIGNAL(createRoom(QString,QString)),
            this, SIGNAL(newRoomCreated(QString,QString)));

    dialog.exec();
}

void RoomsListDialog::onServerIsUp(QString serverName)
{
    localServer = serverName;
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(buttonView(QListWidgetItem*)));
}

void RoomsListDialog::onListItemDoubleClicked(QModelIndex index)
{
    accept();
}

void RoomsListDialog::onListItemChaned()
{
    if(ui->listWidget->selectedItems()[0]->text() == localServer)
    {
        ui->pushButton->setText("Delete");
    } else {
        ui->pushButton->setText("New");
    }
}

void RoomsListDialog::accept()
{
    if(ui->listWidget->selectedItems().isEmpty()) return;

    PasswordDialog dialog(ui->listWidget->selectedItems()[0]->text(), this);
    connect(&dialog, SIGNAL(passwordTyped(QString)), this, SLOT(onPasswordTyped(QString)));
    dialog.exec();
}

void RoomsListDialog::addRoom(QString name, qint32 id)
{
    rooms.insert(name, id);
    ui->listWidget->addItem(name);

}

void RoomsListDialog::deleteRoom(QString name)
{
    rooms.remove(name);

    QList <QListWidgetItem*> list = ui->listWidget->findItems(name, Qt::MatchFixedString);
    int row = ui->listWidget->row(list.first());
    QListWidgetItem* item = ui->listWidget->takeItem(row);

    disconnect(ui->listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(buttonView(QListWidgetItem*)));

    delete item;
}

