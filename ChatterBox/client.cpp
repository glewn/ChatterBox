#include "client.h"
#include "ui_client.h"
#include "mainwindow.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    if(!imagePath.isNull())
    {
        imageObject = new QImage();
        imageObject->load(imagePath);

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        ui->profilePic->setScene(scene);
        showEvent();
    }
}

Client::~Client()
{
    delete ui;
}

/*
void Client::resizeEvent(QResizeEvent *)
{
    ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}
*/

void Client::showEvent()
{
    ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void Client::on_msgInput_returnPressed()
{
    std::stringstream ss;
    time_t rawtime;
    struct tm *timeinfo;
    const char *ampm;

    // Get the local time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Determine whether it's AM or PM
    ampm = (timeinfo->tm_hour/12 > 1) ? "PM" : "AM";

    // Build formatted output
    ss << timeinfo->tm_hour%12 << ":" << std::setw(2) << std::setfill('0') << timeinfo->tm_min
       << ":" << std::setw(2) << std::setfill('0') << timeinfo->tm_sec << ampm
       << ": " << ui->msgInput->text().toStdString();

    ui->msgDisplay->append(QString::fromStdString(ss.str()));

    ui->msgInput->clear();
}

void Client::on_sendBtn_clicked()
{
    Client::on_msgInput_returnPressed();
}
