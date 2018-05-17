#include "qrcodereader.h"
#include <QApplication>
#include <iostream>
#include <unistd.h>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("QRCodeReader");
    QApplication::setApplicationVersion("v1.0");

    ImageViewer w;
    //w.showMaximized();
    w.showFullScreen();
    return a.exec();
}
