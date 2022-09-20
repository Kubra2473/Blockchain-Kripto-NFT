#ifndef SIFRECOZ_H
#define SIFRECOZ_H
#include <iostream>
#include "hex.h"
#include "cryptlib.h"
#include "modes.h"
#include "osrng.h"
#include "aes.h"
#include "cryptopp/files.h"
#include <iostream>
#include <string>
#include <QWidget>
using namespace std;
using namespace CryptoPP;

struct Key
{
    string iv;
    string key;
};


class sifreCoz
{
public:
    sifreCoz();
    string convert_to_hex(string& input);
    string convert_from_hex(string& hex);
    Key generate_key();
    string decrypt_aes(Key& key, string& cipher);
    QString sifreCozme(QString& sifre);
};

#endif // SIFRECOZ_H
