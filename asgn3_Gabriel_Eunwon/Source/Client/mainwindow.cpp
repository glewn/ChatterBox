/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp
--
-- PROGRAM: ChatterBox
--
-- FUNCTIONS:
--  explicit MainWindow(QWidget *parent = 0);
--  ~MainWindow();
--  void set_app_title(QString str);
--  void set_style();
--  void client_start();
--  void closeEvent (QCloseEvent *event);
--  void on_actionProfile_Pic_triggered();
--  void on_actionConnect_triggered();
--  void on_actionFont_Color_triggered();
--
-- DATE: March 10, 2016
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- NOTES:
-- The main window of the program. Manages all the QWidgets inside the central widget. Handles the menu bar and the
-- status bar. Responsible for displaying all GUI.
----------------------------------------------------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientsrc.h"

std::string host = "";
int port = DEFAULT_PORT;
std::string nickname = "";
QString imagePath = NULL;
bool isPicSet = false;
font usrFont;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MainWindow
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
-- INTERFACE: MainWindow::MainWindow(QWidget *parent)
--
-- RETURNS: n/a
--
-- NOTES:
-- The constructor for the MainWindow class.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    usrFont.color = "#333333";
    usrFont.style = QFont("Arial", 12);

    conn = new Connection(this);
    setCentralWidget(conn);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~MainWindow
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
-- INTERFACE: MainWindow::~MainWindow()
--
-- RETURNS: n/a
--
-- NOTES:
-- The destructor for the MainWindow class.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: set_app_title
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
-- INTERFACE: void MainWindow::set_app_title(QString str)
--
-- RETURNS: void
--
-- NOTES:
-- Used to set the title of the application; the window title.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::set_app_title(QString str)
{
    setWindowTitle(str);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: set_style()
--
-- DATE: March 21, 2016
--
-- REVISIONS: (Date and Description)
-- March 21, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void MainWindow::set_style()
--
-- RETURNS: void
--
-- NOTES:
-- Used the set the style(theme) of the GUI.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::set_style()
{
    QFile File("style/test.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    qApp->setStyleSheet(StyleSheet);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_actionProfile_Pic_triggered
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
-- INTERFACE: void MainWindow::on_actionProfile_Pic_triggered()
--
-- RETURNS: void
--
-- NOTES:
-- Handles the profile pic event. Opens a file dialog to allow image. Sets the path of the image to be used in the
-- client widget.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_actionProfile_Pic_triggered()
{
    imagePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    isPicSet = true;
    clnt->drawProfilePic();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_actionConnect_triggered
--
-- DATE: March 21, 2016
--
-- REVISIONS: (Date and Description)
-- March 21, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: void MainWindow::on_actionConnect_triggered()
--
-- RETURNS: void
--
-- NOTES:
-- Handles the connect event to bring the user back to the connection screen.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_actionConnect_triggered()
{
    conn = new Connection(this);
    setCentralWidget(conn);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_actionFont_Color_triggered
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
-- INTERFACE: void MainWindow::on_actionFont_Color_triggered()
--
-- RETURNS: void
--
-- NOTES:
-- Handles the font color event. Brings up a color dialog to allow the user to select a color for the text.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_actionFont_Color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Text Color",  QColorDialog::DontUseNativeDialog);
    if(color.isValid()) {
        usrFont.color = color.name();
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_actionFont_Style_triggered
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
-- INTERFACE: void MainWindow::on_actionFont_Style_triggered()
--
-- RETURNS: void
--
-- NOTES:
-- Handles the font style event. Brings up a font dialog to allow the user to select a style for the text.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_actionFont_Style_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Arial", 12), this);
    if (ok) {
        usrFont.style = font;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: client_start
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
-- INTERFACE: void MainWindow::client_start()
--
-- RETURNS: void
--
-- NOTES:
-- Sets the central widget to be the client widget. Redirects the user from the connection setting to the client screen.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::client_start()
{
    clnt = new Client(this);
    setCentralWidget(clnt);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: closeEvent
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
-- INTERFACE: void MainWindow::closeEvent (QCloseEvent *event)
--
-- RETURNS: void
--
-- NOTES:
-- Overriden function to handle application close event. Performs cleanup before exiting the program.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::closeEvent (QCloseEvent *event)
{
    clnt->client_quit();
}
