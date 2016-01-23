#ifndef SHAREDCLIPBOARD_H
#define SHAREDCLIPBOARD_H

#include <QMainWindow>

namespace Ui {
class SharedClipboard;
}

class SharedClipboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit SharedClipboard(QWidget *parent = 0);
    ~SharedClipboard();

private:
    Ui::SharedClipboard *ui;
};

#endif // SHAREDCLIPBOARD_H
