#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <string>

namespace Ui {
class Connection;
}

class Connection : public QWidget
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();

private slots:

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::Connection *ui;
};

#endif // CONNECTION_H
