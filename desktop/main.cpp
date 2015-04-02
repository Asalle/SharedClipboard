#include <QApplication>

#include "controller.h"
#include "gui/mainwindow.h"
#include "gui/roomslistdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow * window = new MainWindow();
    Controller controller(window);
    window->show();

    return a.exec();
}

//TODO complete refactor of text/image sending

