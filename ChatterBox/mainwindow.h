#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sstream>
#include <string>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

extern std::string host;
extern int port;
extern std::string nickname;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_msgInput_returnPressed();

    void on_sendBtn_clicked();

    void on_actionListWidget_triggered();

    void on_actionProfilePic_triggered();

    void on_listUsers_itemDoubleClicked(QListWidgetItem *item);

    void on_actionConnection_triggered();

    void on_actionConnection_Print_triggered();

private:
    Ui::MainWindow *ui;

    // Profile pic variables
    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
