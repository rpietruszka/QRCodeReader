#include <QtWidgets>
#include <QMessageBox>
#include <QDir>
#include <QImageReader>
#include <QLabel>
#include <QSize>
#include <QString>
#include <iostream>
#include <QTextEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <unistd.h>


#include "updatethread.h"
#include "qrcodereader.h"
#include "ui_qrcodereader.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    centralWidget()->setLayout(ui->grid);

    ThresVals = new QSpinBox*[4];
    ThresVals[0] = ui->CamAThresVal;
    ThresVals[1] = ui->CamBThresVal;
    ThresVals[2] = ui->CamCThresVal;
    ThresVals[3] = ui->CamDThresVal;

    // Tworzenie wątku odpowiedzialnego za aktualizcję captureImage - wyświtalnego obrazu
    // wymaga dostępu go aktualizowania pól, ilości kamer, progów binaryzacji
    QThread *thread = new QThread();
    updatethread = new updateThread();
    updatethread->setCaptureQLabel(ui->captureImage);
    updatethread->setQSpinThresParams(ThresVals);
    updatethread->moveToThread(thread);
    // Podłącznie sygnałów do sterowanie wątkiem
    QObject::connect(thread, SIGNAL(started()), updatethread, SLOT(process()));
    QObject::connect(updatethread, SIGNAL(newQRDecoded(QString)), this, SLOT(setText(QString)));
    QObject::connect(updatethread, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(updatethread, SIGNAL(finished()), updatethread, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    // Podłącznie sygnałów do wyboru ilości podłączonych kamer oraz zakończnia programu
    QObject::connect(ui->ExitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(ui->CamNo, SIGNAL(valueChanged(int)), this, SLOT(setActiveCams(int)));

    // Rozciągnięcie pola wyświetlającego przechwycony obraz
    ui->captureImage->resize(ui->captureImage->maximumSize());
    // Start wątku
    thread->start();
}

// Slot wykorzystywany do aktualizacji QEditText
// argument to QString zawierający rozpoznane kody
void ImageViewer::setText(QString s)
{
    this->ui->decodedQR->setPlainText(s);
}

// Slot odpowiadający za aktywację odpowiednich pól progów binaryzacji
void ImageViewer::setActiveCams(int i){
    for(int j = 1; j < 4; j++)
    {
        ThresVals[j]->setEnabled((j+1 > i) ? false : true);
    }
    updatethread->setActiveCams(i);
	std::cout << i << std::endl;
}

ImageViewer::~ImageViewer()
{
    delete[] ThresVals;
    delete ui;
}
