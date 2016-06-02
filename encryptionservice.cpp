#include "encryptionservice.h"

const CryptoPP::SecByteBlock EncryptionService::getKey()
{
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbdec(randomkeyblock,
                                                         randomkeyblock.size(),
                                                         randomKeyBlockSalt
                                                         );
    CryptoPP::SecByteBlock out = CryptoPP::SecByteBlock(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    cfbdec.ProcessData(out, randomkeyblock, randomkeyblock.size());
    assert(out.size() != 0);
    return out;
}

EncryptionService::EncryptionService(QString & value)
{
    randomkeyblock = CryptoPP::SecByteBlock(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(randomKeyBlockSalt, CryptoPP::AES::BLOCKSIZE);
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbenc(randomkeyblock, randomkeyblock.size(), iv);
    cfbenc.ProcessData((byte*)randomkeyblock.data(), (byte*)value.constData(), value.size());
}

QByteArray & EncryptionService::encrypt(QByteArray & data)
{
    rnd.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);
    //memset((byte*)iv, 'B', CryptoPP::AES::BLOCKSIZE);
    CryptoPP::SecByteBlock keyblock = getKey();
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbenc(keyblock, keyblock.size(), iv);
    cfbenc.ProcessData((byte*)data.data(), (byte*)data.data(), data.size());
    data.prepend((const char*)iv, CryptoPP::AES::BLOCKSIZE);
    qDebug() << "encoded with " << data;

    return data;
}

QByteArray & EncryptionService::decrypt(QByteArray & data)
{
    QDataStream out(&data, QIODevice::ReadOnly);
    out.readRawData((char*)iv, CryptoPP::AES::BLOCKSIZE);

    // remove the read iv
    data.remove(0, CryptoPP::AES::BLOCKSIZE);

    CryptoPP::SecByteBlock keyblock = getKey();
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbdec(keyblock, keyblock.size(), iv);
    cfbdec.ProcessData((byte*)data.data(), (byte*)data.data(), data.size());
    qDebug() << "decoded " << data;

    return data;
}
