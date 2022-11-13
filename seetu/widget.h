#ifndef WIDGET_H
#define WIDGET_H

#include <QDir>
#include <QWidget>
#include <QList>
#include <QFileInfo>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QThread>
class QTimer;
class QDir;
class QPixmap;
class QImage;
class QLabel;
class wthread;
class windowdocker;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    void changetime(int i);
    void timerstart(int tm=0);
    void timerSuspend();
    void timerCountLife();

    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void closeEvent(QCloseEvent *);

    void setimg(int next = 1);

    void cltimeup();
    void readThrd();

    void nextFile(int i = 1);
    void openfile(QString *str = 0);

    QPoint centerpos();

    int width = 500;
    int height =500;


    qreal disrat =1;
    qreal imgratioWH();

    int curfile = -1;
    int runok = 1;
    int waittime =0;
    int timerlife = 0;

    int open = 0;
    int cltimer_on = 0;
    int smooth = 0;

    QWidget *transParent = 0;
    windowdocker *wdocker = 0;

    wthread *thrd =0;

    QList<QFileInfo> *m_filelist;

    QDir *filedir = 0;

    QString filepath = "E:/picture/Test";

    QTimer *m_timer =0;
    QTimer *cltimer = 0;
    QTimer *readImgTimer = 0;

    QPoint mousedragglobal=QPoint();
    QPoint windowtl=QPoint();

    QImage *curpix =0;
    QImage *fronpix = 0;
    QImage *nextpix = 0;





};

class wthread : public QThread
{
    Q_OBJECT
public:
    Widget *wid;
    wthread(Widget *w){wid = w;}
    void run();
};



#endif // WIDGET_H
