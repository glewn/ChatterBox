#include "mainwindow.h"
#include "ui_mainwindow.h"

std::string host = "";
int port = 0;
std::string nickname = "";
QString imagePath = NULL;
bool isPicSet = false;

// UNUSEDDDDDD
struct font font;

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

void MainWindow::set_status(QString str)
{
    ui->statusBar->showMessage(str);
}

void MainWindow::set_app_title(QString str)
{
    setWindowTitle(str);
}

void MainWindow::set_style()
{
    QFile File("style/test.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    qApp->setStyleSheet(StyleSheet);
}

/*
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
    // Display message; same functionality as return pressed
    MainWindow::on_msgInput_returnPressed();
}

// Test function
void MainWindow::on_actionListWidget_triggered()
{
    // Temp to show functionality
    ui->listUsers->addItem("User1\nVancouver, Canada");
}

// Test function
void MainWindow::on_actionProfilePic_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    imageObject = new QImage();
    imageObject->load(imagePath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    //scene->setSceneRect(image.rect());
    ui->profilePic->setScene(scene);
    ui->profilePic->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

// Test function
void MainWindow::on_listUsers_itemDoubleClicked(QListWidgetItem *item)
{
    QWidget *wdg = new QWidget;
    Ui::Profile ui;
    ui.setupUi(wdg);
    ui.label->setText(item->text());
    wdg->show();
    //ui->msgDisplay->append(item->text());
}

// Test function
void MainWindow::on_actionConnection_triggered()
{
    QWidget *wdg = new Connection;
    wdg->show();
}

// Test function
void MainWindow::on_actionConnection_Print_triggered()
{
    //std::string tmp = MainWindow::host;
    ui->msgDisplay->append("shit");
    ui->msgDisplay->append(QString::fromStdString(host));
}
*/

void MainWindow::on_actionClient_triggered()
{
    clnt = new Client;
    setCentralWidget(clnt);
}

void MainWindow::on_actionServer_triggered()
{
    svr = new Server;
    setCentralWidget(svr);
    //testing
    /*
    set_app_title("whatevs");
    set_style();
    */
}

void MainWindow::on_actionProfile_Pic_triggered()
{
    imagePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    isPicSet = true;
    clnt->drawProfilePic();
}

void MainWindow::on_actionConnect_triggered()
{
    conn = new Connection;
    conn->show();
}
/*
void MainWindow::on_actionColor_Dialog_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    if(color.isValid()) {
        ui->widget->setPalette(color);
        ui->widget->show();
    }
}
*/
