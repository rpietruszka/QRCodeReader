#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QTextEdit>
#include <zbar.h>
#include "camera.h"

class updateThread : public QObject
{
    Q_OBJECT
public:
    explicit updateThread(QObject *parent = 0);
    // Bezpośredni dostęp do QLabel wświetlającego obraz
    void setCaptureQLabel (QLabel* ql);
    // Watości progów binaryzacji
    void setQSpinThresParams (QSpinBox** ql);
    QString scanImage(zbar::Image &image);

signals:
    void finished();
    void error(QString error);
    void newQRDecoded(QString s);
    void updateView(QImage img);

public slots:
    void process();
    void setActiveCams(int i);

private:
    QLabel *ql;
    QSpinBox **tresVals;
    Camera *camera;
    bool isScannerActive = true;
};

#endif // UPDATETHREAD_H
