#ifndef TIFFIMGWIDGET_H
#define TIFFIMGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QString>
#include "openslide.h"


class TiffImgWidget: public QWidget
{

    Q_OBJECT
public:
    TiffImgWidget();
    TiffImgWidget(QWidget *parent);

    bool OpenImage(QString filePath);
    void SetMagnify(int m);
    void ShowFullImg();

public slots:
    void reMagnify(int m);

private:
    //the size of window
    const int windowWidth = 960;
    const int windowHeight = 640;
    //img
    QString mFilePath;
    QImage *mQtImg;
    openslide_t *mImg;
    uchar *imgData;

    int mMagnify;
    QLabel *mLab;

    void DivideLine();

};

#endif // TIFFIMGWIDGET_H
