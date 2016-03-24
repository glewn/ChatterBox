#include "client.h"
#include "ui_client.h"
#include "mainwindow.h"
#include "clientsrc.h"

clientSrc *csrc;

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

    csrc = new clientSrc(this, parent);//host, port);
    csrc->clientStart();
}

Client::~Client()
{
    delete ui;
}

void Client::drawProfilePic() {
    imageObject = new QImage();
    imageObject->load(imagePath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    ui->profilePic->setScene(scene);
    showEvent(NULL);
}

void Client::showEvent(QShowEvent * event)
{
    this->QWidget::showEvent(event);
    if(isPicSet) {
        ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
}

void Client::resizeEvent(QResizeEvent * event)
{
    this->QWidget::resizeEvent(event);
    if(isPicSet) {
        ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
}

void Client::on_msgInput_returnPressed()
{
    if(!ui->msgInput->text().isEmpty()) {
        csrc->writeMsg(this->build_string(ui->msgInput->text()).toStdString());
        ui->msgInput->clear();
    }
}

void Client::on_sendBtn_clicked()
{
    Client::on_msgInput_returnPressed();
}

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

void Client::print_msg(QString msg)
{
    ui->msgDisplay->append(msg);
    scroll_to_bottom();
}

void Client::scroll_to_bottom()
{
    QTextCursor c = ui->msgDisplay->textCursor();
    c.movePosition(QTextCursor::End);
    ui->msgDisplay->setTextCursor(c);
}
