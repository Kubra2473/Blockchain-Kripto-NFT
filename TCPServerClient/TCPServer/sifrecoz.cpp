#include "sifrecoz.h"

sifreCoz::sifreCoz()
{

}

string sifreCoz::convert_to_hex(string &input)
{
    string output;
    CryptoPP::StringSource(input,true,
                           new CryptoPP::HexEncoder(
                               new CryptoPP::StringSink(output)));
    return output;

}

string sifreCoz::convert_from_hex(string &hex)
{
    string output;
    CryptoPP::StringSource(hex,true,
                           new CryptoPP::HexDecoder(
                               new CryptoPP::StringSink(output)));
    return output;

}

Key sifreCoz::generate_key()
{
    std::string str = "11111111111111111111111111111111";
    std::string str2 ="0000000000000000";
    SecByteBlock key(reinterpret_cast<const CryptoPP::byte*>(&str[0]),str.size());
    SecByteBlock iv(reinterpret_cast<const CryptoPP::byte*>(&str2[0]),str.size());

    string keystr((const char*) key.BytePtr(), key.size());
    string ivstr((const char*) iv.BytePtr(), iv.size());

    Key keyout;
    keyout.iv=ivstr;
    keyout.key=keystr;
    return keyout;

}

string sifreCoz::decrypt_aes(Key &key, string &cipher)
{
    CBC_Mode< AES >::Decryption dec;
    const CryptoPP::byte* keydata = (const CryptoPP::byte*) &key.key[0];
    const CryptoPP::byte* ivdata = (const CryptoPP::byte*) &key.iv[0];
    dec.SetKeyWithIV(keydata,key.key.size(),ivdata);

    string plaintext;
    StringSource(cipher,true,
               new HexDecoder(
                     new StreamTransformationFilter(
                         dec,new StringSink(plaintext))));
    return plaintext;

}

QString sifreCoz::sifreCozme(QString &sifre)
{
    string str= sifre. toStdString();
    Key key= generate_key();
    //cout<<"Key:"<<convert_to_hex(key.key)<<endl;
    string b=convert_from_hex(str);
    string decipher=decrypt_aes(key,b);
    //cout<<"Decrypted:"<<decipher<<endl;
    QString qstr = QString::fromStdString(decipher);
    return qstr;

}
