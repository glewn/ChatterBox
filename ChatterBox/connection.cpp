#include "connection.h"
#include "ui_connection.h"
#include "mainwindow.h"

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);
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
