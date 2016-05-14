#ifndef ENCODER_H
#define ENCODER_H

#include <QByteArray>
#include <QString>
#include <QDataStream>
#include <QDebug>


#include "common.h"
#include "osrng.h"
#include "rijndael.h"

#include "modes.h"
#include "secblock.h"
#include "filters.h"
#include "cryptlib.h"

//! A class, that encrypts and decrypts sharedclipboard packages
//! Crypto++ was taken as a third-party lib, statically binded
//! Sources of Crypto++ are in cryptopp folder
//! AES cfb was taken as default encryption

class EncryptionService
{
    //QString keyhash; /*!< The given key's hash (password of the room)*/
    CryptoPP::AutoSeededRandomPool rnd; /*!< the randomizer, generates random initialization vectors*/
    byte iv[CryptoPP::AES::BLOCKSIZE]; /*!< default holder for initialization vector*/
    CryptoPP::SecByteBlock keyblockhash; /*!< processed key's hash to work with */

    CryptoPP::SecByteBlock randomkeyblock;
    byte randomKeyBlockSalt[CryptoPP::AES::BLOCKSIZE];


    CryptoPP::SecByteBlock const getKey();
public:
    EncryptionService(QString&);
    QByteArray & encode(QByteArray&); /*!< encodes bytearray*/
    QByteArray & decode(QByteArray&); /*!< decyphers bytearray*/

    QByteArray encodeFile(QString &, SharedFile & sf); /*!< encodes file*/
    QByteArray decodeFile(); /*!< decyphers file*/
};

#endif // ENCODER_H

// here you got some Bill Cypher

//                                  @B@B@
//                                  @@B@B
//                                  @B@B@
//                                  B@B@@
//                                  @@@B@
//                                  B@B@B
//                                  @@@@@
//                                  B@B@@
//                                  @B@B@
//                                  B@@@B
//                                  @@@B@
//                                  B@@@B
//                                  @B@B@
//                                  B@B@B
//                                  @@@B@
//                                B@B@B@B@B,
//                                MqJ7i7YPM.

//                                   7@:
//                                  r@B@:
//                                 r@@@B@:
//                                ;@B@B@B@:
//                               r@B@B@B@@@:
//                              ;@@@B@B@B@B@:
//                             i@B@@@B@B@B@B@:
//                            i@B@B@B@B@B@@@B@:
//                           i@B@B@B@@@B@B@B@B@:
//                          i@B@B@B@B@B@B@@@B@B@:
//                         i@B@B@B@u. , .u@B@@@B@:
//                        i@B@B@Br   .@    7@@B@@@:
//                       i@B@B@u     u@.     0@B@B@:
//                      i@@@B@       XB:      .@B@B@:
//                     :@B@B@M       F@:       B@B@@@,
//                    :@B@B@B@Bv     LB      5B@B@B@B@,
//                   :@B@B@B@B@@@:    @    :@B@B@B@B@@@,
//                  :@B@B@B@B@@@@@Bv     v@@B@B@B@B@B@B@,
//                 :@B@@@B@B@B@B@B@B@B@B@@@B@B@@@B@B@B@B@,
//                :@B@B@B@B@B@Br7@@@@@B@B@BB:5B@@@B@B@B@B@,
//               :@B@B@B@B@B@B@    j@B@BBr   J@B@@@B@B@B@B@,
//              :@B@B@B@B@B@B@B:      r      S@@B@@@B@B@B@B@,
//             ,@B@B@B@B@B@@@B@.    7G@Pi    U@B@B@B@B@B@@@B@.
//            ,@B@B@B@B@@@B@B@B. YB@B@B@B@B; LB@B@@@@@B@@@@@@@.
//           ,@@@@@B@@@B@B@B@B@B@B@@@@@B@@@B@@@B@@@@@B@B@B@B@B@.
//          ,@B@B@B@@@B@B@@@B@@@B@B@B@B@@@B@B@B@B@B@@@B@B@B@B@B@.
//         .@@@B@B@B@B@@@B@B@B@B@B@B@B@B@@@B@B@B@B@B@@@@@B@B@B@B@.
//        ,@B@@@B@B@@@B@@@B@B@B@B@B@B@B@@@B@@@B@@@B@@@B@B@B@B@B@B@.
//       .@B@B@B@B@B@B@B@B@B@B@B@B@B@B@B@B@@@B@B@@@B@B@B@B@B@@@B@B@.
//      ,@B@B@B@B@B@B@@@B@B@B@@@B@B@B@B@B@B@@@B@B@B@B@B@B@@@B@B@B@B@.
//     :@B@@@B@B@B@B@B@B@@@B@@@B@B@B@B@B@B@B@B@B@B@B@B@B@@@B@B@@@B@@@.
//    i@B@B@BBB@MBB@B@B@B@MMB@O@B@B@B@B@B@B@B@BBM@MBB@BMOM8GBMZOOMMBB@
//            .5            Yu                   B          k
//            rP            15                   @          P7
//            v0            LB    @i             B          FU
//            7G             @   MBi            U@          Uq
//            vE             v@YM @i           J@           vG
//            vG               :  Bi       MMXE2            rO
//            LN                  @v       B7               rB
//            i8                  i:       @j               :@
//            .B                           BN               ,B
//             @                           @q               iM
//             B7                          Bi          r    u5
//             iB  .v                                  78 0 B:
//              @r iL Z                               8iuqO,B
//               Mi@kO:                                ::uO7


