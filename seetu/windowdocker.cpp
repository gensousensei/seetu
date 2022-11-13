#include "windowdocker.h"
#include "widget.h"
#include <QResizeEvent>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
windowdocker::windowdocker(QWidget *parent)
    : QWidget{parent}
{
    colorborder = &normal;
    setWindowFlag(Qt::FramelessWindowHint);
    update();
}

void windowdocker::resizew()
{
    qreal imgratio = w->imgratioWH();
    if(imgratio>0)
    {
        int w1 = width()-2*border;
        int h1 = height()-2*border;
        if(imgratio>dockratioWH())
        {
            int w2 = w1;
            int h2 = w1/imgratio;
            int dh = (h1-h2)/2;
            w->setGeometry(border,border+dh,w2,h2);
            w->update();

        }
        else
        {
            int h2 = h1;
            int w2 = h2*imgratio;
            int dw = (w1-w2)/2;
            w->setGeometry(border+dw,border,w2,h2);
            w->update();

        }
    }
    else
    {
        int w1 = width()-2*border;
        int h1 = height()-2*border;
        int h2 = h1/2;
        int w2 = w1/2;
        int dw = (w1-w2)/2;
        int dh = (h1-h2)/2;
        w->setGeometry(border+dw,border+dh,w2,h2);
        w->update();
    }

}


qreal windowdocker::dockratioWH()
{
    return qreal((width()-2*border))/(height()-2*border);
}

void windowdocker::mousePressEvent(QMouseEvent *ev)
{
    int px = ev->pos().x();
    int py = ev->pos().y();

    if(px>0&&px<border)
    {
        dragright = -1;

    }
    else if(px>width()-border&&px<width())
    {
        dragright = 1;

    }
    if(py>0&&py<border)
    {
        dragdown = -1;

    }
    else if(py>height()-border&&py<height())
    {
        dragdown = 1;
    }
    l0 = geometry().x();
    u0 = geometry().y();

    x0 = ev->globalX();
    y0 = ev->globalY();

    w0 = geometry().width();
    h0 = geometry().height();

}

void windowdocker::mouseMoveEvent(QMouseEvent *ev)
{


        int l1 = l0,
                u1=u0,
                w1=w0,
                h1=h0;
        int y1 = ev->globalY();
        int x1 =ev->globalX();
        if(dragdown>0)
        {
            h1=y1-y0+h0;
        }
        else if(dragdown<0)
        {
            u1 =y1-y0+u0;
            h1=y0-y1+h0;
        }

        if(dragright<0)
        {
            l1 =x1-x0+l0;
            w1 =x0-x1+w0;

        }
        else if(dragright>0)
        {
            w1 =x1-x0+w0;

        }
        setGeometry(l1,u1,w1,h1);




}

void windowdocker::mouseReleaseEvent(QMouseEvent *ev)
{
    dragright = 0;
    dragdown = 0;
}

void windowdocker::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    if(this->isActiveWindow())
    {
        QRegion thisrect = QRegion(this->rect());
        QRect drect = QRect(border,border,width()-2*border,height()-2*border);
        QRegion dockerrect = QRegion(drect);
        QPainterPath path;
        path.addRegion(thisrect-dockerrect);
        p.fillPath(path,*colorborder);
        p.fillRect(drect,backcolor);
    }

    else p.fillRect(this->rect(),QColor(0,0,0,0));

}

void windowdocker::focusOutEvent(QFocusEvent *ev)
{
    update();
}

void windowdocker::resizeEvent(QResizeEvent *ev)
{
    if(ev->size().width()>100&&ev->size().height()>100)
    {
        resizew();
    }
    else resize(110,110);
}

void windowdocker::wheelEvent(QWheelEvent *ev)
{
    qreal delta;
    if(ev->angleDelta().y() > 0)
    {
        delta = 0.91;
    }
    else
    {
        delta = 1.1;
    }

    int ow =rect().width();
    int oh =rect().height();

    setGeometry(pos().x()+(1-delta)*ev->position().x(),pos().y()+(1-delta)*ev->position().y(),ow*delta,oh*delta);
}
