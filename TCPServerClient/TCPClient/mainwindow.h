#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpclient.h"
#include <iostream>
#include "hex.h"
#include "cryptlib.h"
#include "modes.h"
#include "osrng.h"
#include "aes.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFileDialog>
#include <QBuffer>
using namespace std;
using namespace CryptoPP;
struct Key
{
    string iv;
    string key;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    string convert_to_hex(string& input);
    Key generate_key();
    string encrypt_aes(Key& key, string& plaintext);
    QImage image;

private slots:
    void on_pushButton_clicked();
    void readSSMessage();

    void on_pushButton_update_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    TcpClient *tcpClient;
};

#endif // MAINWINDOW_H
