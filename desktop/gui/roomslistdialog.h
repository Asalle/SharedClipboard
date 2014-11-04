#ifndef ROOMSLISTDIALOG_H
#define ROOMSLISTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class RoomsListDialog;
}

class RoomsListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomsListDialog(QWidget * parent = 0);
    ~RoomsListDialog();

    void setRoomsHash(QHash<QString, qint32> rooms);

signals:
    void roomChoosed(qint32 roomId, QString password);
    void newRoomCreated(QString name, QString password);

public slots:
    void accept();
    void addRoom(QString name, qint32 id);

private slots:
//    void listItemDoubleClicked();
    void onPasswordTyped(QString password);
    void onNewRoomButtonClicked();

private:
    QHash<QString, qint32> rooms;
    Ui::RoomsListDialog *ui;

};

#endif // ROOMSLISTDIALOG_H
