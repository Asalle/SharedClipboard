#include "encryptionservice.h"

const CryptoPP::SecByteBlock EncryptionService::getKey()
{
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbdec(randomkeyblock,
                                                         randomkeyblock.size(),
                                                         randomKeyBlockSalt
                                                         );
    CryptoPP::SecByteBlock out;
    cfbdec.ProcessData((byte*)out.data(), (byte*)keyblockhash.data(), keyblockhash.size());
    return out;
}

EncryptionService::EncryptionService(QString & value)
{
    //keyblock = CryptoPP::SecByteBlock(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    randomkeyblock = CryptoPP::SecByteBlock(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(randomKeyBlockSalt, CryptoPP::AES::BLOCKSIZE);
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbenc(randomkeyblock, randomkeyblock.size(), iv);
    cfbenc.ProcessData((byte*)value.constData(), (byte*)randomkeyblock.data(), value.size());
}

QByteArray & EncryptionService::encode(QByteArray & data)
{
    rnd.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);
    CryptoPP::SecByteBlock keyblock = getKey();
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbenc(keyblock, keyblock.size(), iv);
    cfbenc.ProcessData((byte*)data.constData(), (byte*)data.data(), data.size());
    data.prepend((const char*)iv, CryptoPP::AES::BLOCKSIZE);
    qDebug() << "encoded with " << data;


    return data;
}

QByteArray & EncryptionService::decode(QByteArray & data)
{
    QDataStream out(&data, QIODevice::ReadOnly);
    out.readRawData((char*)iv, CryptoPP::AES::BLOCKSIZE);

    data.remove(0, CryptoPP::AES::BLOCKSIZE);

    CryptoPP::SecByteBlock keyblock = getKey();
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbdec(keyblock, keyblock.size(), iv);
    cfbdec.ProcessData((byte*)data.constData(), (byte*)data.data(), data.size());
    qDebug() << "encoded " << data;

    return data;
}
