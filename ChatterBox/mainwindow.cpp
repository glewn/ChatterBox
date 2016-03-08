#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_msgInput_returnPressed()
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

void MainWindow::on_sendBtn_clicked()
{
    MainWindow::on_msgInput_returnPressed();
}

void MainWindow::on_actionListWidget_triggered()
{
    ui->listUsers->addItem("User1\nVancouver, Canada");
}

void MainWindow::on_actionGraphicsView_triggered()
{

}
