#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QColorDialog>
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
    QString build_string(QString msg);
    void displayChatMessage(std::string name, std::string message);
    void print_msg(QString msg);

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

    // Formatted text tags
    QString fmt;

    QColor color;

};

#endif // CLIENT_H
