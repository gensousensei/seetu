#ifndef WINDOWDOCKER_H
#define WINDOWDOCKER_H

#include <QWidget>

class Widget;
class windowdocker : public QWidget
{
    Q_OBJECT
public:
    explicit windowdocker(QWidget *parent = nullptr);

    void resizew();

    void resizeEvent(QResizeEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);
    qreal dockratioWH();
    Widget *w = 0;

    QColor *colorborder = 0;
    QColor highlight = QColor(100,200,200,150);
    QColor normal = QColor(100,200,220,75);
    QColor backcolor = QColor(100,200,200,0);

    int border = 20;
    int dragdown = 0;
    int dragright = 0;
    int x0 = 0;
    int y0 = 0;
    int u0 = 0;
    int l0 = 0;
    int w0 = 0;
    int h0 = 0;


signals:

};

#endif // WINDOWDOCKER_H
