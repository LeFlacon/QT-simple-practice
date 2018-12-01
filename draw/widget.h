#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <vector>
#include <QPushButton>

using namespace std;
namespace Ui {
class Widget;
}

typedef struct myLine
{
    QPoint startp;
    QPoint endp;

}myLine;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    QPoint startp;
    QPoint endp;
    bool isPressed;

    int scale;//放大倍数
    QPushButton *btn;

    vector<myLine*> lines;

private:
    Ui::Widget *ui;

private slots:
    void fangda();

};

#endif // WIDGET_H
