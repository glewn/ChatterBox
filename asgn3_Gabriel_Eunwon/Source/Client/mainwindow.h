#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QFile>
#include <QColorDialog>
#include <QFont>
#include <QFontDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <sstream>
#include <string>
#include "client.h"
#include "connection.h"

namespace Ui {
class MainWindow;
}

struct font {
    QString color;
    QFont style;
};

extern std::string host;
extern int port;
extern std::string nickname;
extern bool client;
extern QString imagePath;
extern bool isPicSet;
extern struct font usrFont;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void set_app_title(QString str);
    void set_style();
    void client_start();
    void closeEvent (QCloseEvent *event);

private slots:

    void on_actionProfile_Pic_triggered();

    void on_actionConnect_triggered();

    void on_actionFont_Color_triggered();

    void on_actionFont_Style_triggered();

private:
    Ui::MainWindow *ui;

    // Pointer to the widgets
    Client *clnt;
    QWidget *conn;
};

#endif // MAINWINDOW_H
