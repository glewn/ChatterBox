#include "connection.h"
#include "ui_connection.h"
#include "mainwindow.h" // ??? maybe ???
#include "client.h"

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);
    ui->hostInput->setText("127.0.0.1");
    ui->portInput->setText("7000");
    ui->nameInput->setText("popo");
}

Connection::~Connection()
{
    delete ui;
}


void Connection::on_okBtn_clicked()
{
    host = ui->hostInput->text().toStdString();
    port = std::atoi(ui->portInput->text().toStdString().c_str());
    nickname = ui->nameInput->text().toStdString();
    close();
}

void Connection::on_cancelBtn_clicked()
{
    close();
}
