#include "widget.h"
#include "windowdocker.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QDir>
#include <QTimerEvent>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QFileInfo>
#include <QLabel>
#include <QPixmap>
#include <QCursor>

Widget::Widget(QWidget *parent )
    : QWidget(parent)
{
    transParent= new QWidget();

    transParent->setWindowFlag(Qt::FramelessWindowHint);
    transParent->setAttribute(Qt::WA_TranslucentBackground,true);
    transParent->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    transParent->showMaximized();
    QPoint inipos = transParent->geometry().center();

    wdocker = new windowdocker(transParent);
    setParent(wdocker);
    wdocker->w = this;
    wdocker->setGeometry(inipos.x()-width/2,inipos.y()-height/2,width,height);
    wdocker->show();
    wdocker->activateWindow();

    m_timer = new QTimer(this);
    cltimer = new QTimer(this);
    readImgTimer = new QTimer(this);
    thrd = new wthread(this);

    connect(m_timer,&QTimer::timeout,this,&Widget::timerCountLife);
    connect(cltimer,&QTimer::timeout,this,&Widget::cltimeup);
    connect(readImgTimer,&QTimer::timeout,this,&Widget::readThrd);

    this->setFocus();
    setFocusPolicy(Qt::ClickFocus);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

}

QPoint Widget::centerpos()
{
    return wdocker->geometry().center();
}

void Widget::cltimeup()
{
    cltimer_on = 0;
    cltimer->stop();
}

void Widget::changetime(int i)
{
    waittime=i;
    timerstart(waittime);
}

void Widget::timerCountLife()
{
    timerlife-=100;
    qDebug()<<"timerlife:"<<timerlife<<"**";

    if(timerlife<=0)
    {
        if(waittime!=0)
            nextFile();
        else
            m_timer->stop();

    }
}

void Widget::timerstart(int tm)
{
    waittime = tm;
    qDebug()<<"waittime:"<<waittime;

    if(tm>0)
    {
        timerlife = tm;
        qDebug()<<"timerlife:"<<timerlife;
        m_timer->start(100);
    }
    else
    {
        m_timer->stop();
    }
}

void Widget::timerSuspend()
{
    waittime = -waittime;
    timerlife=-timerlife;
    qDebug()<<"timerlife:"<<timerlife;
    if(timerlife >0)
        m_timer->start(100);
    else
        m_timer->stop();
}


void Widget::setimg(int next)
{
    qDebug()<<"flag"<<next;

    if(curpix)
    {
        delete curpix;
    }
    if(next==1)
    {
        if(nextpix)
        {
            curpix = nextpix;
            nextpix = 0;
        }
        else curpix = new QImage(filepath);
    }
    else if(next==-1)
    {
        if(fronpix)
        {
            curpix = fronpix;
            fronpix = 0;
        }
        else curpix = new QImage(filepath);
    }
    else if(next == 0)
    {
        curpix = new QImage(filepath);
    }
    wdocker->resizew();
    qDebug()<<curfile<<filepath;

    if(runok)
    {
        readImgTimer->start(100);
    }


}

qreal Widget::imgratioWH()
{
    if(curpix)
    {
        if(!curpix->isNull())
        {
            return qreal(curpix->width())/curpix->height();
        }
    }
    return -1;
}


void Widget::nextFile(int i)
{
    if(timerlife>=0)
        timerlife = qAbs(waittime);
    else
        timerlife = -qAbs(waittime);
    if(curpix)
    {
        if(i)
        {
            if(curfile < m_filelist->count() - 1)
                curfile++;
            else curfile = 0;
            filepath = m_filelist->at(curfile).filePath();
            setimg(1);
        }
        else
        {

            if(curfile == 0)
                curfile = m_filelist->count() - 1;
            else curfile --;
            filepath = m_filelist->at(curfile).filePath();
            setimg(-1);
        }
    }
}

void Widget::openfile(QString *str)
{
    if(str){filepath = *str;}
    else
    {
        QFileDialog dialog;
        filepath = dialog.getOpenFileName(this, tr("Open File"),
                                          filepath,
                                          tr("Images (*.png *.bmp *.jpg *.jpeg)"));
    }
    if(!filepath.isEmpty())
    {
        QFileInfo fileinfo(filepath);
        filedir = new QDir(fileinfo.path());
        QStringList filter;
        filter<<"*.jpg"<<"*.png"<<"*.jpeg"<<"*.bmp";
        m_filelist = new QList<QFileInfo>(filedir->entryInfoList(filter,QDir::Files,QDir::Name));

        for(int i=0;i<m_filelist->count();i++)
        {
            if(m_filelist->at(i).filePath()==filepath)
            {
                curfile = i;
                break;
            }
        }
        setimg(0);
        timerstart();
    }
}

void Widget::readThrd()
{
    readImgTimer->stop();
    if(thrd)
        thrd->run();
}

void wthread::run()
{
    wid->runok = 0;
    int i=0;
    QString path;
    if(wid->nextpix)
        delete wid->nextpix;

    if(wid->curfile < wid->m_filelist->count() - 1)
        i = wid->curfile+1;
    else i = 0;
    path = wid->m_filelist->at(i).filePath();
    wid->nextpix = new QImage(path);
    qDebug()<<"next at"<<i<<path;
    if(1)
    {
        if(wid->fronpix)
            delete wid->fronpix;
        if(wid->curfile == 0)
            i = wid->m_filelist->count() - 1;
        else i = wid->curfile -1;
        path = wid->m_filelist->at(i).filePath();
        wid->fronpix = new QImage(path);
        qDebug()<<"front at"<<i<<path;
    }
    wid->runok = 1;
    terminate();
    wait();
}


void Widget::wheelEvent(QWheelEvent *ev)
{
    ev->ignore();
//    qreal delta;
//    if(ev->angleDelta().y() > 0)
//    {
//        delta = 0.91;
//    }
//    else
//    {
//        delta = 1.1;
//    }

//    int ow = wdocker->rect().width();
//    int oh = wdocker->rect().height();
//    wdocker->setGeometry(centerpos().x()-ow*delta/2,centerpos().y()-oh*delta/2,ow*delta,oh*delta);
}

void Widget::keyPressEvent(QKeyEvent *ev)
{
    {
        if(ev->key() == Qt::Key_1)
        {
            changetime(1000);
        }
        if(ev->key() == Qt::Key_2)
        {
            changetime(2000);
        }
        if(ev->key() == Qt::Key_3)
        {
            changetime(3000);
        }
        if(ev->key() == Qt::Key_4)
        {
            changetime(4000);
        }
        if(ev->key() == Qt::Key_5)
        {
            changetime(5000);
        }
        if(ev->key() == Qt::Key_6)
        {
            changetime(6000);
        }
        if(ev->key() == Qt::Key_7)
        {
            changetime(7000);
        }
        if(ev->key() == Qt::Key_8)
        {
            changetime(8000);
        }
        if(ev->key() == Qt::Key_9)
        {
            changetime(9000);
        }
        if(ev->key() == Qt::Key_0)
        {
            changetime(10000);
        }
        if(ev->key() == Qt::Key_Q)
        {
            changetime(15000);
        }
        if(ev->key() == Qt::Key_W)
        {
            changetime(20000);
        }
        if(ev->key() == Qt::Key_E)
        {
            changetime(25000);
        }
        if(ev->key() == Qt::Key_R)
        {
            changetime(30000);
        }
        if(ev->key() == Qt::Key_T)
        {
            changetime(40000);
        }



    }
    if(ev->key() == Qt::Key_Escape)
    {
        this->close();
    }
    if(ev->key() == Qt::Key_Minus)
    {
        qreal delta = 0.8;
        int ow = wdocker->rect().width();
        int oh = wdocker->rect().height();
        wdocker->setGeometry(centerpos().x()-ow*delta/2,centerpos().y()-oh*delta/2,ow*delta,oh*delta);

    }
    if(ev->key() == Qt::Key_Equal)
    {
        qreal delta = 1.25;
        int ow = wdocker->rect().width();
        int oh = wdocker->rect().height();
        wdocker->setGeometry(centerpos().x()-ow*delta/2,centerpos().y()-oh*delta/2,ow*delta,oh*delta);

    }
    if(ev->key() == Qt::Key_Left)
    {
        nextFile(0);
    }
    if(ev->key() == Qt::Key_Right)
    {
        nextFile(1);
    }
    if(ev->key() == Qt::Key_Return)//Qt::Key_Enter little
    {
        timerSuspend();
        openfile();
    }
    if(ev->key() == Qt::Key_Enter)
    {
        timerSuspend();
        openfile();
    }
    if(ev->key() == Qt::Key_Space)
    {
        timerSuspend();
    }
    if(ev->key() == Qt::Key_F11)
    {
        wdocker->setGeometry(-wdocker->border,
                             -wdocker->border,
                             transParent->width()+2*wdocker->border,
                             transParent->height()+2*wdocker->border);
    }
}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    m_timer->stop();
    cltimer->start(200);
    cltimer_on = 1;
    mousedragglobal = ev->globalPos();
    windowtl =wdocker->geometry().topLeft();


}

void Widget::mouseMoveEvent(QMouseEvent *ev)
{

    wdocker->move(windowtl+ ev->globalPos()-mousedragglobal);
}

void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(timerlife >0)
        m_timer->start(100);

    if(ev->button() == Qt::LeftButton)
    {
        if(cltimer_on == 1)
        {
            cltimer_on = 0;
            nextFile(1);
        }
    }
    if(ev->button() == Qt::RightButton)
    {
        if(cltimer_on == 1)
        {
            cltimer_on = 0;
            nextFile(0);

        }
    }


}

void Widget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        openfile();
    }
}

void Widget::closeEvent(QCloseEvent *)
{
    transParent->close();

}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(curpix)
    {
        if(!curpix->isNull())
        {
            if(smooth)
            {
                painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            }
            painter.drawImage(rect(),*curpix);
        }
        else
        {
            painter.fillRect(this->rect(),QColor(100,200,200,70));

        }
    }
    else
    {
        painter.fillRect(this->rect(),QColor(100,200,200,70));
    }

}



Widget::~Widget()
{
}
