#include "client.h"
#include "ui_client.h"
#include "mainwindow.h"

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

    color = QColorDialog::getColor(Qt::black, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    if(color.isValid()) {
        fmt.append(color.name());
    }
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
    // Make sure to move this -----------------------------------------------------------------
    // fmt = "<span style='display:block;color:#863453'>";
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
    ss << "[" << timeinfo->tm_hour%12 << ":" << std::setw(2) << std::setfill('0') << timeinfo->tm_min
       << ":" << std::setw(2) << std::setfill('0') << timeinfo->tm_sec << ampm << "]"
       << ": " << "<span style='color:" << fmt.toStdString() << "'>" << ui->msgInput->text().toStdString() << "</span>";

    ui->msgDisplay->append(QString::fromStdString(ss.str()));

    ui->msgInput->clear();
}

void Client::on_sendBtn_clicked()
{
    Client::on_msgInput_returnPressed();
}
