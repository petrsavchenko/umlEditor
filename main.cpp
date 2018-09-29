#include <QtGui/QApplication>
#include <QtGui>
#include "mainwindow.h"
#include"QTextCodec"
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
