#include "qdebug.h"
#include "widget.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    Widget w;
    QString str1;
    if(argc>1)
    {
        str1 = qApp->arguments().at(1).split("\\").join("/");
        w.openfile(&str1);
    }

    w.show();




    return a.exec();
}


