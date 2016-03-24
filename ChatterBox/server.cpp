#include "server.h"
#include "ui_server.h"
#include "serversrc.h"
#include "mainwindow.h"
Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}

void Server::on_pushButton_clicked()
{
    port = ui->lineEdit->text().toInt();
    ServerSrc *ssrc = new ServerSrc();
    ssrc->start();
}
