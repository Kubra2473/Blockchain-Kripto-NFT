#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include "sifrecoz.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSever = new TcpSever;
    QString str;
    str = "network ip:"+tcpSever->returnOutIP()+" port:1414";
    str =  str + "\n\nlocal ip:"+tcpSever->getIPAddress();
    ui->label_3->setText(str);
    if(!tcpSever->listen(QHostAddress::Any,1414)){
        this->close();
    }
    connect(tcpSever,SIGNAL(alreadyRead()),this,SLOT(readMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tcpSever;
}

void MainWindow::readMessage()
{
    sifreCoz deneme;
    QString veri=tcpSever->returnReadSomething();
    //qDebug()<<"Gelen Veri:"<<veri;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(QByteArray(veri.toUtf8()));
     //qDebug()<<"jsonnnn:"<<jsonDocument;
    QJsonObject object = jsonDocument.object();
   //qDebug()<<"Json Object:"<<object;
    //Text
     QJsonValue textV = object.value("text");
    //qDebug()<<"text:"<<textV;
     QString decodedText = "";
     decodedText.append(textV.toString());
     //qDebug()<<"Text Metin:"<<decodedText;
     QString textCozuldu = deneme.sifreCozme(decodedText);
     ui->label_2->setText(textCozuldu);

     //Resim
      QJsonValue resim = object.value("resim");
     //qDebug()<<"resim:"<<resim;
      QString decodedResim = "";
      decodedResim.append(resim.toString());
      //qDebug()<<"Resim"<<decodedResim;
      QString Cozuldu = deneme.sifreCozme(decodedResim);
      QByteArray brResim = Cozuldu.toUtf8();
      QString filename = "output.png";
      QImage image;
      image.loadFromData(QByteArray::fromBase64(brResim), "PNG");
      image=image.scaledToWidth(ui->label->width(),Qt::SmoothTransformation);
      ui->label->setPixmap(QPixmap::fromImage(image));//Resmi ekleme
      image.save(filename, "PNG");


}

void MainWindow::on_pushButton_clicked()
{
    tcpSever->setSendMessage(ui->lineEdit->text());
}

void MainWindow::on_pushButton_2_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString Str = tcpSever->returnOutIP();
    clipboard->setText(Str);
}
