#include "connection.h"
#include "ui_connection.h"
#include "mainwindow.h"

MainWindow *mainw;

Connection::Connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    mainw = qobject_cast<MainWindow*>(parent);

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
    if(!ui->hostInput->text().isEmpty() && !ui->portInput->text().isEmpty())
    {
        host = ui->hostInput->text().toStdString();
        port = std::atoi(ui->portInput->text().toStdString().c_str());
        nickname = ui->nameInput->text().toStdString();
        mainw->client_start();
        close();
    }
}

void Connection::on_cancelBtn_clicked()
{
    QApplication::quit();
}
