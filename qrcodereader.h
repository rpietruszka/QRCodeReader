#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H


#include <iostream>
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QTextEdit>
#include <QSpinBox>
#include "updatethread.h"

// struktura automatycznie generowania przez Qt odzwierciedlająca formularz xml generowany przez QtDesigner
// do wywołań Qt dla C++
namespace Ui {
    class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    // Makro Qt inicjalizujące stan QtWigdet(QMainWindow)
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);

    ~ImageViewer();

    // Solot-y wykorzytywane do reakcji na zmianę parametrów podawanych przez użytkownika
public slots:
    // Ustawia pole tekstowe zdekodowanych QR
    void setText(QString s);
    // Ilość podłączonych kamer to max 4 (1-layer stacking hub)
    void setActiveCams(int i);


private:
    Ui::ImageViewer *ui;
    // Wątek aktualizujący widok przetwarzanego obrazu
    updateThread *updatethread;
    // tablica progów binaryzacji (w postaci wigetów Qt) przekazywania to ww wątku
    // nie wymaga synchronizacji i jest szybszym rozwiązaniem od bindowania SIGNAL->SLOT
    QSpinBox** ThresVals;
};

#endif // IMAGEVIEWER_H
