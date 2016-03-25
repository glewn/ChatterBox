/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: client.cpp
--
-- PROGRAM: ChatterBox
--
-- FUNCTIONS:
-- explicit Client(QWidget *parent = 0);
-- ~Client();
-- void drawProfilePic();
-- QString build_string(QString msg);
-- void print_msg(QString msg);
-- void scroll_to_bottom();
-- void client_quit();
-- void update_list();
-- void on_msgInput_returnPressed();
-- void on_sendBtn_clicked();
-- void showEvent(QShowEvent * event);
-- void resizeEvent(QResizeEvent * event);
--
-- DATE: March 10, 2016
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- NOTES:
-- The client screen. Handles the GUI for all the client side. Including font styles, color, and profile pics.
----------------------------------------------------------------------------------------------------------------------*/
#include "client.h"
#include "ui_client.h"
#include "mainwindow.h"
#include "clientsrc.h"

clientSrc *csrc;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Client
--
-- DATE: March 10, 2016
--
-- REVISIONS: (Date and Description)
-- March 10, 2016 - Function created
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: Client::Client(QWidget *parent)
--
-- RETURNS: n/a
--
-- NOTES:
-- The constructor for the Client class.
----------------------------------------------------------------------------------------------------------------------*/
Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    if(isPicSet)
    {
        imageObject = new QImage();
        imageObject->load(imagePath);

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        ui->profilePic->setScene(scene);
        showEvent(NULL);
    }

    csrc = new clientSrc(this);//host, port);
    csrc->clientStart();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Client
--
-- DATE: March 10, 2016
--
-- REVISIONS: (Date and Description)
-- March 10, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: Client::~Client()
--
-- RETURNS: n/a
--
-- NOTES:
-- The Client class destructor.
----------------------------------------------------------------------------------------------------------------------*/
Client::~Client()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: drawProfilePic
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
-- INTERFACE: void Client::drawProfilePic()
--
-- RETURNS: void
--
-- NOTES:
-- Handles displaying the user's profile pic onto the screen. Uses the image path from the MainWindow.
----------------------------------------------------------------------------------------------------------------------*/
void Client::drawProfilePic() {
    imageObject = new QImage();
    imageObject->load(imagePath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    ui->profilePic->setScene(scene);
    showEvent(NULL);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: showEvent
--
-- DATE: March 22, 2016
--
-- REVISIONS: (Date and Description)
-- March 22, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::showEvent(QShowEvent * event)
--
-- RETURNS: void
--
-- NOTES:
-- Handles displaying the profile pic that fits inside the image screen. Keeps the image aspect.
----------------------------------------------------------------------------------------------------------------------*/
void Client::showEvent(QShowEvent * event)
{
    this->QWidget::showEvent(event);
    if(isPicSet) {
        ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: resizeEvent
--
-- DATE: March 22, 2016
--
-- REVISIONS: (Date and Description)
-- March 22, 2016 - Function created.
-- March 23, 2016 - Fix resizing to prevent crashes.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::resizeEvent(QResizeEvent * event)
--
-- RETURNS: void
--
-- NOTES:
-- Handles resize event. Resizes the image based on the image screen. Keeps the image aspect.
----------------------------------------------------------------------------------------------------------------------*/
void Client::resizeEvent(QResizeEvent * event)
{
    this->QWidget::resizeEvent(event);
    if(isPicSet) {
        ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_msgInput_returnPressed
--
-- DATE: March 11, 2016
--
-- REVISIONS: (Date and Description)
-- March 11, 2016 - Function created.
-- March 23, 2016 - Update to only handle append; move string building to separate function.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::on_msgInput_returnPressed()
--
-- RETURNS: void
--
-- NOTES:
-- Handles when the user presses the return key. Gets the input field text and displays it onto the message screen.
----------------------------------------------------------------------------------------------------------------------*/
void Client::on_msgInput_returnPressed()
{
    if(!ui->msgInput->text().isEmpty()) {
        ui->msgDisplay->append(this->build_string(ui->msgInput->text()));
        csrc->writeMsg(MSG_MESG, this->build_string(ui->msgInput->text()).toStdString());
        ui->msgInput->clear();
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_sendBtn_clicked
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
-- INTERFACE: void Client::on_sendBtn_clicked()
--
-- RETURNS: void
--
-- NOTES:
-- Handles when the user selects "Send" button. Has the same functionality as the returnPressed function.
----------------------------------------------------------------------------------------------------------------------*/
void Client::on_sendBtn_clicked()
{
    Client::on_msgInput_returnPressed();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: build_string
--
-- DATE: March 23, 2016
--
-- REVISIONS: (Date and Description)
-- March 23, 2016 - Function created.
-- March 23, 2016 - Updated to use structures.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: QString Client::build_string(QString msg)
--
-- RETURNS: QString - the formatted text string
--
-- NOTES:
-- Takes in a string and returns a formatted string based on the font color and style that is currently selected.
----------------------------------------------------------------------------------------------------------------------*/
QString Client::build_string(QString msg) {
    std::stringstream ss;
    time_t rawtime;
    struct tm *timeinfo;
    const char *ampm;

    // Get the local time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Determine whether it's AM or PM
    ampm = (timeinfo->tm_hour/12 > 1) ? "PM" : "AM";

    // Build formatted output for time and name (<- to be implemented)
    ss << "[" << timeinfo->tm_hour%12 << ":" << std::setw(2) << std::setfill('0') << timeinfo->tm_min
       << ":" << std::setw(2) << std::setfill('0') << timeinfo->tm_sec << ampm << "]"
       << ((QString::fromStdString(nickname).isEmpty()) ? ("You") : (nickname)) << ": ";

    ss << "<span style='color:" << usrFont.color.toStdString() << ";font-size:" << usrFont.style.pointSize()
       << "pt;font-weight:" << ((usrFont.style.bold()) ? "bold" : "normal") << ";text-decoration:"
       << ((usrFont.style.underline()) ? "underline" : "none") << ";font-style:" << ((usrFont.style.italic()) ? "italic" : "normal")
       << ";font-family:" << usrFont.style.family().toStdString() << "'>" << msg.toStdString() << "</span>";

    return QString::fromStdString(ss.str());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_msg
--
-- DATE: March 23, 2016
--
-- REVISIONS: (Date and Description)
-- March 23, 2016 - Function created
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::print_msg(QString msg)
--
-- RETURNS: void
--
-- NOTES:
-- Displays the message onto the message screen and scrolls to the bottom to visibly show the last message.
----------------------------------------------------------------------------------------------------------------------*/
void Client::print_msg(QString msg)
{
    ui->msgDisplay->append(msg);
    scroll_to_bottom();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: scroll_to_bottom
--
-- DATE: March 23, 2016
--
-- REVISIONS: (Date and Description)
-- March 23, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::scroll_to_bottom()
--
-- RETURNS: void
--
-- NOTES:
-- Moves the cursor to the end. Displays the last message appended to the screen.
----------------------------------------------------------------------------------------------------------------------*/
void Client::scroll_to_bottom()
{
    QTextCursor c = ui->msgDisplay->textCursor();
    c.movePosition(QTextCursor::End);
    ui->msgDisplay->setTextCursor(c);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: client_quit
--
-- DATE: March 23, 2016
--
-- REVISIONS: (Date and Description)
-- March 23, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::client_quit()
--
-- RETURNS: void
--
-- NOTES:
-- Write a message to the server to alert disconnect.
----------------------------------------------------------------------------------------------------------------------*/
void Client::client_quit() {
    csrc->writeMsg(MSG_QUIT, "Bye");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: update_list
--
-- DATE: March 23, 2016
--
-- REVISIONS: (Date and Description)
-- March 23, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void Client::update_list()
--
-- RETURNS: void
--
-- NOTES:
-- Update the list of users connected to the server.
----------------------------------------------------------------------------------------------------------------------*/
void Client::update_list() {
    int j = 0;
    ui->userList->clear();
    for(int i =0; i <MAXUSER; i++){
        if(list[i].sockNum >0) {
            QString s(list[i].name);
            ui->userList->insertItem(j++, s);
        }
    }
}
