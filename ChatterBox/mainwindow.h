#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QFile>
#include <QColorDialog>
#include "client.h"
#include "server.h"
#include "connection.h"

namespace Ui {
class MainWindow;
}

extern std::string host;
extern int port;
extern std::string nickname;
extern bool client;
extern QString imagePath;
extern bool isPicSet;

struct font {
    QString color;
    QString size;
    bool bold;
    bool italic;
    bool underline;
    QString fontStyle;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void set_status(QString str);
    void set_app_title(QString str);
    void set_style();

private slots:
    void on_actionClient_triggered();

    void on_actionServer_triggered();

    void on_actionProfile_Pic_triggered();

    void on_actionConnect_triggered();

private:
    Ui::MainWindow *ui;

    // Pointer to the widgets
    Client *clnt;
    Server *svr;
    QWidget *conn;
};

#endif // MAINWINDOW_H
