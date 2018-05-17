#ifndef CAMERA_H
#define CAMERA_H

#include <raspicam/raspicam.h>


typedef unsigned char uchar;

/*
 *    Warper dla RaspiCam udostępniający podstawowe operacje
 */

class Camera
{

    // Przechwytujemy obraz w skali szarości 1B (8b) / piksel
private:
    // W celu zapewnienia kompatybilności wykorzystujemy rozdzielcość dla kamer do 5Mpx
    // Wedługo dokumentacji wykorzystywanego huba nie są obsługiwanie kamery 8Mpx i wzwyż
    const int maxW = 2592;
    const int maxH = 1944;

    int W = 1920;
    int H = 1080;

    int currentUsingCam = 0;
    
    raspicam::RaspiCam *camera;

    int camPinout[4][3][2] = {
      /* Reprezentacja konfiguracji pinów dla wyboru kamery
       * konwencja {pinID, HIGH/LOW}
       * WinringPi posisada własną numerację pinów różną od trybów BCM / BOARD
       * znanych z biblioteki GPIO tutaj WiringPi_pin ->  BOARD_pin
       * 7 -> 7, 12 -> 1, 11 -> 0
       */
      { {7,1}, {1,1}, {0,0} }, // CamB
      { {7,0}, {1,1}, {0,0} }, // CamA
      { {7,0}, {1,0}, {0,1} }, // CamC
      { {7,1}, {1,0}, {0,1} } // CamD
    };

public:
    Camera();
    //Liczba podłączonych kamer aktualizowana przez updateThread
    int connectedCams = 1;

    // Ustawienie rozmiarów zdjęcia:
    // Biblioteka RaspiCam posiada ograniczenia stosunku szerokości do wysokości zdjęcia!
    // Ustawienie tych wartości MUSI nastąpić przed wywołaniem open()!
    void setWidth(int width);
    void setHeight(int height);
    void setRotation(int angle);

    unsigned int getWidth();
    unsigned int getHeight();
    // Zwraca rozmiar tablicy potrzebny do przechwycenia obrazu dla ustawionej rozdzielczości
    size_t getDataSize();

    int getConnectedCamN();
    void setConnectedCamN(int N);

    raspicam::RaspiCam* getCamera();

    // Otwarcie urządzenia
    int open();

    // Funkcje odpwiadające za przełącznie kamer - ustawiają stany wyjść GPIO
    void selectNextCam();
    void selectPrevCam();
    // Wykorzystywana przez 2 poprzenie ustawia konfigurację GPIO na podstawie currentUsingCam
    void updatePinConfiguration();

    // Przeciążony operatory wykorzystywany do pobrania przechwyconego obrazu
    friend void operator>>(Camera& cam, uchar* data);
};



#endif // CAMERA_H
