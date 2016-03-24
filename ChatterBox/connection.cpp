/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: connection.cpp
--
-- PROGRAM: ChatterBox
--
-- FUNCTIONS:
--  explicit Connection(QWidget *parent = 0);
--  ~Connection();
--  void on_okBtn_clicked();
--  void on_cancelBtn_clicked();
--
-- DATE: March 11, 2016
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- NOTES:
-- The widget that handles getting connection information from the user. It starts the client code to connect and
-- interact with the server.
----------------------------------------------------------------------------------------------------------------------*/
#include "connection.h"
#include "ui_connection.h"
#include "mainwindow.h"

MainWindow *mainw;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Connection
--
-- DATE: March 11, 2016
--
-- REVISIONS: (Date and Description)
-- March 11, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: Connection::Connection(QWidget *parent)
--
-- RETURNS: n/a
--
-- NOTES:
-- The constructor for the Connection class.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Connection
--
-- DATE: March 11, 2016
--
-- REVISIONS: (Date and Description)
-- March 11, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: Connection::~Connection()
--
-- RETURNS: n/a
--
-- NOTES:
-- The destructor for the Connection class.
----------------------------------------------------------------------------------------------------------------------*/
Connection::~Connection()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_okBtn_clicked
--
-- DATE: March 24, 2016
--
-- REVISIONS: (Date and Description)
-- March 11, 2016 - Function created.
-- March 23, 2016 - Handle no input texts.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Connection::on_okBtn_clicked()
--
-- RETURNS: void
--
-- NOTES:
-- Handles when the user selects "OK". Gets the input data and starts the client code.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_cancelBtn_clicked
--
-- DATE: March 23, 2016
--
-- REVISIONS: (Date and Description)
-- March 24, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Connection::on_cancelBtn_clicked()
--
-- RETURNS: void
--
-- NOTES:
-- Handles when the user selects "Cancel". It closes the application.
----------------------------------------------------------------------------------------------------------------------*/
void Connection::on_cancelBtn_clicked()
{
    QApplication::quit();
}
