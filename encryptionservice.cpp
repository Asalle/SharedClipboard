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
    // get digest while we are still clear text
    const QByteArray digest = hash(data);

    byte enciv[CryptoPP::AES::BLOCKSIZE];
    rnd.GenerateBlock(enciv, CryptoPP::AES::BLOCKSIZE);
    CryptoPP::SecByteBlock keyblock = getKey();
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbenc(keyblock, keyblock.size(), enciv);
    cfbenc.ProcessData((byte*)data.data(), (byte*)data.data(), data.size());
    qDebug() << "encoded: " << data;

    //prepend digest, it's always the same size, in common.h SHA3_DIGEST_SIZE
    data.prepend((const char*)digest.constData(), SHA3_DIGEST_SIZE);

    // prepend salt
    data.prepend((const char*)enciv, CryptoPP::AES::BLOCKSIZE);

    qDebug() << "enc digest: " << digest;
    qDebug() << "enciv" << enciv;

    return data;
}

QByteArray & EncryptionService::decrypt(QByteArray & data)
{
    QDataStream out(&data, QIODevice::ReadOnly);

    // read and remove the iv
    byte deciv[CryptoPP::AES::BLOCKSIZE];
    out.readRawData((char*)deciv, CryptoPP::AES::BLOCKSIZE);
    data.remove(0, CryptoPP::AES::BLOCKSIZE);

    qDebug() << "encoded " << data;

    // read and remove digest
    QByteArray digest = QByteArray(SHA3_DIGEST_SIZE, 0);
    out.readRawData((char*)digest.data(), SHA3_DIGEST_SIZE);
    data.remove(0, SHA3_DIGEST_SIZE);

    CryptoPP::SecByteBlock keyblock = getKey();
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbdec(keyblock, keyblock.size(), deciv);
    cfbdec.ProcessData((byte*)data.data(), (byte*)data.data(), data.size());

    qDebug() << "dec digest: " << digest;
    qDebug() << "deciv" << deciv;
    qDebug() << "decoded " << data << "\n";

    if(digest != hash(data))
       throw "It's not our message!";

    return data;
}

const QByteArray EncryptionService::hash(QByteArray in)
{
    QByteArray digest = QByteArray(CryptoPP::SHA::DIGESTSIZE, 0);
    CryptoPP::SHA3_256().CalculateDigest((byte*)digest.data(), (const byte*)in.constData(), in.size());

    return digest;
}
