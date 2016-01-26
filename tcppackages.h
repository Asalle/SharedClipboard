#ifndef TCPPACKAGES
#define TCPPACKAGES

//! represents packages types
enum class TcpPackage : char
{
    TXT, /*!< plain text*/
    PNG, /*!< png image*/
    FILE_NOTIF, /*!< notification about avaliable file*/
    FILE_REQ, /*!< request of file*/
    FILE_RESP, /*!< file as response to file request*/
};



#endif // TCPPACKAGES

