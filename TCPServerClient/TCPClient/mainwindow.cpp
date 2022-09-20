#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new TcpClient;
    connect(tcpClient,SIGNAL(alreadyRead()),this,SLOT(readSSMessage()));

    ui->lineEdit_ip->setText(tcpClient->returnIP());
    ui->lineEdit_port->setText(tcpClient->returnPort());
}

MainWindow::~MainWindow()
{
    delete ui;
}

string MainWindow::convert_to_hex(string &input)
{

    string output;
       CryptoPP::StringSource(input,true,
                              new CryptoPP::HexEncoder(
                                  new CryptoPP::StringSink(output)));
       return output;
}

Key MainWindow::generate_key()
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

string MainWindow::encrypt_aes(Key &key, string &plaintext)
{

    CBC_Mode< AES >::Encryption enc;
    const CryptoPP::byte* keydata = (const CryptoPP::byte*) &key.key[0];
    const CryptoPP::byte* ivdata = (const CryptoPP::byte*) &key.iv[0];
    enc.SetKeyWithIV(keydata,key.key.size(),ivdata);

    string cipher;
    StringSource(plaintext,true,
                 new StreamTransformationFilter(
                     enc,new HexEncoder(new StringSink(cipher))));
    return cipher;

}

void MainWindow::on_pushButton_clicked()
{
    QString metin=ui->lineEdit->text();
    //qDebug()<<"metin"<<metin;
    std::string utf8_text = metin.toUtf8().constData();
    Key key= generate_key();
    //cout<<"Key:"<<convert_to_hex(key.key)<<endl;
    string cipher=encrypt_aes(key,utf8_text);
    //cout<<"Encrypted:"<<convert_to_hex(cipher)<<endl;
    string a=convert_to_hex(cipher);
    //cout<<"a:"<<a;
    QString qstr = QString::fromStdString(a);

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG");
    QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());//Resim base64 Qstring dönüştürüldü
    std::string resim = iconBase64.toUtf8().constData();
    //cout<<"iconBase64"<<resim;
    string cipherResim=encrypt_aes(key,resim);
    string ab=convert_to_hex(cipherResim);
    //cout<<"ab:"<<ab;
    QString sifreliResim = QString::fromStdString(ab);
    QJsonObject mainObject;
    mainObject.insert("text", qstr);
    mainObject.insert("resim", sifreliResim);
    QJsonDocument doc(mainObject);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QString jsonString(data);
    //qDebug()<<"client veri:"<<jsonString;
    tcpClient->sendMessage(jsonString.toUtf8());
}

void MainWindow::readSSMessage()
{
    ui->label_3->setText(tcpClient->returnMessage());
}

void MainWindow::on_pushButton_update_clicked()
{
    tcpClient->setIP(ui->lineEdit_ip->text());
    tcpClient->setPort(ui->lineEdit_port->text().toInt());
    tcpClient->newConnect();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename= QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images(*.png *.jpg *.jpeg *.bmp *.gif)"));
    if(QString::compare(filename,QString()) !=0)
    {

        bool valid=image.load(filename);
        if(valid)
        {
            image=image.scaledToWidth(ui->label->width(),Qt::SmoothTransformation);
            ui->label->setPixmap(QPixmap::fromImage(image));//Resmi ekleme
        }

    }

}

