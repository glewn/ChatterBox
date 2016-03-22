#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <sstream>
#include <string>
#include <time.h>
#include <fstream>
#include <iomanip>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
    void drawProfilePic();

protected:

private slots:
    void on_msgInput_returnPressed();

    void on_sendBtn_clicked();

private:
    void showEvent(QShowEvent * event);
    void resizeEvent(QResizeEvent * event);
    Ui::Client *ui;

    QImage  *imageObject;
    QPixmap image;
    QGraphicsScene *scene;
};

#endif // CLIENT_H
