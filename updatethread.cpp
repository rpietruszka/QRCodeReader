#include "updatethread.h"
#include "qrcodereader.h"
#include "camera.h"

#include <QLabel>
#include <QString>
#include <QPixmap>
#include <iostream>
#include <QImageReader>
#include <QImage>
#include <raspicam/raspicam.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <zbar.h>
#include <zbar/ImageScanner.h>
#include <unistd.h>


typedef unsigned char uchar;

// Wykorzystywana rozdzielczość
const int W = 1920;
const int H = 1080;

updateThread::updateThread(QObject *parent) : QObject(parent)
{
}

void updateThread::process(){

    camera = new Camera();
    camera->setWidth(W);
    camera->setHeight(H);
    camera->setRotation(180);

    // Struktury OpenCV reprezentujące matryce pikseli
    cv::Mat thresholdFrame;
    cv::Mat fullFrame;
    // String (w Qt) wykorzstywany do przekazywania rozczytanych kodów
    QString s;

    // tablica do której trafiają dane z kamer (pojedyncza kamera później obraz jest konkatenowny) 1B * W * H
    uchar *data=new uchar[camera->getDataSize()];

    while(1==1)
    {

//      cv::Mat capture(camera->getHeight(), camera->getWidth(), CV_8UC1, data);
        // Pobranie obrazu z kolejnych kamer
        for(int i=0;i<camera->getConnectedCamN(); i++)
        {
            // Otwarcie kamer można ponawiać - wtedy nic się nie dzieje
            // Jednak podczas zmiany liczby kamer nowe trzeba otworzyć ponownie,
            // aby ustawić odpwiednie wartości w rejetrach peryferyjnych np. rozdzielczość
            camera->open();

            // Przechwycenie obrazu
            *camera >> data;
            camera->selectNextCam();

            cv::Mat capture(camera->getHeight(), camera->getWidth(), CV_8UC1, data);
            
            /* Binaryzacja z modułu OpenCv tu wykorzystywan jest zwykła z globalnym progiem
             * próby wykorzystania adaptacyjnej w większści przypadków kończyły się niepowodzeniem,
             * ponieważ były zbyt agresywnie - np 1 kwadrat koloru czanergo otoczony białym prakycznie zawsze był
             * 'wybielany' nawet dla małych
             */
            cv::threshold(capture, thresholdFrame, tresVals[i]->value(), 255, cv::THRESH_BINARY);

            if(i == 0)
            {
                fullFrame.release();
                thresholdFrame.copyTo(fullFrame);
            }
            else
            {
                // łącznie obrazu
                cv::hconcat(fullFrame, thresholdFrame, fullFrame);        
            }
        }
        
        // Konwersja do struktury rozpoznawalnej przez skaner kodów QR dostarczany przez ZBar
        zbar::Image image(fullFrame.size().width, fullFrame.size().height,\
                             "GREY", (void*)fullFrame.data,fullFrame.size().width*fullFrame.size().height );


        if(isScannerActive)
        {
            // Istnieje możliwość przeniesienia procesu skanowania w poszukiwaniu kodów do osobnego wątku
            // np. za pomocą biblotek pthread / boost, jednak nie gwarantuje to znaczącej poprawy wydajności
            // patrz dokumentacja.
            s = scanImage(image);
            //Wyzowlenie sygnału zbindowanego z QTextExit wyświetlającym rozpoznane QR
            this->newQRDecoded(s);
        }
        
        // Znów konwersja tym razem do struktury obrazu rozpoznawalnej przez Qt
        QImage newImage(fullFrame.data, fullFrame.size().width, fullFrame.size().height, QImage::Format_Indexed8);

        //Skalowanie połącznego obrazu kamer do odpowieniego rozmiaru z zachowaniem proprcji obrazu
        QImage scaledImg = newImage.scaled(ql->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // Tutaj trzeba bezpośrenio się odwołać do Widgetu wyświtaljącego obraz, ponieważ może być zbyt duzy dla mechanizmu sygnałów Qt
        // Innym rozwiązniem dla sygnałóœ jest przekazanie przez wskaźnika co jednak wymaga synchronizacji lub przekaznia kopii.
        ql->setPixmap(QPixmap::fromImage(scaledImg));

        s.clear();
        //camera->getCamera()->release();
    }
}

// klasa qrcodereader nie ma bezpośredniego dostępu do kamer należących do updateThread dlatego
// udostępniany jest slot pozwalający na aktualizacją wartości
void updateThread::setActiveCams(int i)
{
    camera->setConnectedCamN(i);
}

// Zawatość funkcji może być łatwo wykorzystana prze pthread -> zmiana typu parametru na void* oraz rzutowanie wskaźnika na zbar::Image
QString updateThread::scanImage(zbar::Image &image)
{

    zbar::ImageScanner scanner;
    // Blokowanie odczytywania innych kodów (EAN-13/CODE-128/Code-39) = Znacząca poprawa wydajności
    // Dokładnie mozliwosci w 'man zbarimg' sekcja SYMBOLOGY
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 0);
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
    QString s;

    scanner.scan(image);
    // Rezultaty (zdekodowane QR) można otrzymać przez odpowieni iterator
    for(zbar::Image::SymbolIterator symbol = image.symbol_begin();
              symbol != image.symbol_end(); ++symbol)
    {
        // Wypisanie odczytanych kodów
        s.append(symbol->get_data().c_str());
        s.append('\n');
        std::cout << symbol->get_data() << std::endl;
    }
    return s;
}

void updateThread::setCaptureQLabel (QLabel* ql){
    this->ql = ql;
}

void updateThread::setQSpinThresParams (QSpinBox** ql){
    this->tresVals = ql;
}
