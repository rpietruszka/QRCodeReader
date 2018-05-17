#include "camera.h"

#include <iostream>
#include <raspicam/raspicam.h>
#include <wiringPi.h>
#include <unistd.h>


typedef unsigned char uchar;

Camera::Camera()
{
    this->camera = new raspicam::RaspiCam();

    camera->setFormat(raspicam::RASPICAM_FORMAT_GRAY);

    std::cout << "Camera res: " << this->W << "x" << this->H\
              <<  "\nBefore open camera u can change it.\n" << std::endl;

    wiringPiSetup();
    pinMode(7, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(0, OUTPUT);

    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(6, OUTPUT);

    digitalWrite(1, 1);
    digitalWrite(0, 1);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    digitalWrite(13, 1);
    digitalWrite(6, 1);

    updatePinConfiguration();
    std::cout << "Constr end\n";
}

void Camera::setWidth(int width)
{
    if(width > 0 && width < maxW)
    {
        this->W = width;
    }
    else
    {
        throw ("Width must be possitive and less "+maxW);
    }
}

void Camera::setHeight(int height)
{
    if(height > 0 && height < maxW)
    {
        this->H = height;
    }
    else
    {
        throw ("Height must be possitive and less "+maxW);
    }
}

unsigned int Camera::getWidth()
{
    return this->W;
}

unsigned int Camera::getHeight()
{
    return this->H;
}

raspicam::RaspiCam* Camera::getCamera()
{
    return camera;
}

size_t Camera::getDataSize()
{
    return W*H;
}

int Camera::open()
{
    camera->setCaptureSize(W, H);
    return camera->open();
}

void Camera::setRotation(int angle)
{
    camera->setRotation(angle);
}

void Camera::selectNextCam()
{
  currentUsingCam = (currentUsingCam + 1) % connectedCams;
  updatePinConfiguration();
}

void Camera::selectPrevCam()
{
  currentUsingCam = currentUsingCam - 1;
  if(currentUsingCam == -1)
  {
      currentUsingCam = connectedCams - 1;
  }
  updatePinConfiguration();
}

void Camera::updatePinConfiguration()
{
    for(int i=0; i<3; i++)
    {
        digitalWrite(camPinout[currentUsingCam][i][0], camPinout[currentUsingCam][i][1]);
    }
    // Opóźnienie wynikająca konieczności mutipleksacji podłącznych kamer
    usleep(100000);
}

void operator>>(Camera& c, uchar* data)
{
    c.camera->grab();
    c.camera->retrieve(data, raspicam::RASPICAM_FORMAT_IGNORE);
}

int Camera::getConnectedCamN()
{
    return connectedCams;
}
void Camera::setConnectedCamN(int N)
{
    if(N > 0 && N < 5)
    {
        this->connectedCams = N;
        currentUsingCam = 0;
    }
    else
    {
        std::cerr << "Invalid camera nubmer, leaving without changes" << std::endl;
    }
}

