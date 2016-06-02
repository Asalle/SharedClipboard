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
    LoginPassDialog * checkPassDlg = new LoginPassDialog(NULL);;
    QObject::connect(roomChoose, &RoomChoose::roomChosen,
                     [&](QString name){
                        checkPassDlg->show();
                     });
    tcpService = QSharedPointer<TcpService>(new TcpService());
    QObject::connect(checkPassDlg, &LoginPassDialog::credentials,
                     roomService.data(), &RoomService::checkPass);
    QObject::connect(roomService.data(), &RoomService::checkPassByTcp,
                     tcpService.data(), &TcpService::checkPass);
    udpService = QSharedPointer<UdpService>(new UdpService("testroom", NULL));

    QObject::connect(udpService.data(), &UdpService::newMember,
         roomService.data(), &RoomService::addMember);
    QObject::connect(udpService.data(), &UdpService::newMember,
        [&](QString name, QString room, QList<QHostAddress>){
            roomChoose->updateRoomList(room);
        });

    // start sniffing for other members over udp
    udpService->start();

    QObject::connect(roomChoose, &RoomChoose::newRoom,
                     udpService.data(), &UdpService::setRoomAndLogin);
    QObject::connect(roomChoose, &RoomChoose::newRoom,
                     roomService.data(), &RoomService::setRoomNameAndLogin);

    QObject::connect(roomChoose, &RoomChoose::newPass,
                     [&](QString pass){
                        encService = QSharedPointer<EncryptionService>(new EncryptionService(pass));
                        tcpService->setEncService(encService);
                     });
//    QObject::connect(tcpService.data(), &TcpService::pwdAC,
//                     roomService.data(), &RoomService::PwdAC);

    QObject::connect(roomService.data(), &RoomService::PwdAC,
                    [&](QString login, QString pass){
                       QObject::connect(roomService.data(), &RoomService::refreshMembers,
                                        tcpService.data(), &TcpService::setRoomMembers);
                       tcpService->createServer();

                       clipboardService = QSharedPointer<ClipboardService>(new ClipboardService());
                       QObject::connect(tcpService.data(), &TcpService::gotData,
                                        clipboardService.data(), &ClipboardService::updateClipboard);
                       QObject::connect(clipboardService.data(), &ClipboardService::clipboardChanged,
                                        tcpService.data(), &TcpService::send);
                       });

    QObject::connect(roomService.data(), &RoomService::PwdFailed,
                     []{
                        QMessageBox msg;
                        msg.setText("Wrong password");
                        msg.show();
                     });
    roomChoose->show();

    return app.exec();
}
