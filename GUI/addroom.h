#ifndef ADDROOM_H
#define ADDROOM_H

#include <QDialog>

namespace Ui {
class AddRoom;
}

class AddRoom : public QDialog
{
    Q_OBJECT

public:
    explicit AddRoom(QWidget *parent = 0);
    ~AddRoom();
signals:
    void newRoom(QString name);
    void newPass(QString pass);

private slots:
    void on_addbtn_clicked();

    void on_cancelbtn_clicked();

private:
    Ui::AddRoom *ui;
};

#endif // ADDROOM_H
