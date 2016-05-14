#include <QApplication>
#include <QSharedPointer>
#include <QMessageBox>


#include "sessionmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    QSharedPointer<RoomService> roomService;
    QSharedPointer<UdpService> udpService;
    QSharedPointer<TcpService> tcpService;
    QSharedPointer<ClipboardService> clipboardService;
    QSharedPointer<FileService> fileService;
    QSharedPointer<EncryptionService> encService;

    roomService = QSharedPointer<RoomService>(new RoomService(NULL));
    QList<QString> room_list = roomService->getRooms();
    RoomChoose * roomChoose = new RoomChoose(room_list);
    LoginPassDialog * checkPassDlg = new LoginPassDialog(NULL);

    QObject::connect(roomChoose, &RoomChoose::roomChosen,
                     [](QString name){
                        checkPassDlg->show();
                     });

    QObject::connect(roomService, &RoomService::PwdAC,
                    [&](QString l, QString p){

                       udpService = QSharedPointer<UdpService>(new UdpService(testLogin, NULL));

                       QObject::connect(udpService.data(), &UdpService::newMember,
                            roomService.data(), &RoomService::addMember);

                       // start sniffing for other members over udp
                       udpService->start();

                       tcpService = QSharedPointer<TcpService>(new TcpService());

                       QObject::connect(roomService.data(), &RoomService::refreshMembers,
                                        tcpService.data(), &TcpService::setRoomMembers);
                       tcpService->createServer();

                       encService = QSharedPointer<EncryptionService>(new EncryptionService(pass));
                       clipboardService = QSharedPointer<ClipboardService>(new ClipboardService());
                       QObject::connect(tcpService.data(), &TcpService::gotData,
                                        clipboardService.data(), &ClipboardService::updateClipboard);
                       QObject::connect(clipboardService.data(), &ClipboardService::clipboardChanged,
                                        tcpService.data(), &TcpService::send);
                       });

    QObject::connect(roomService, &RoomService::PwdFailed,
                     []{
                        QMessageBox msg;
                        msg.setText("Wrong password");
                        msg.show();
                     });

    roomChoose->show();
    return app.exec();
}
