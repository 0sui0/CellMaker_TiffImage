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

    bool OpenImage(const QString &filePath);
    void SetMagnify(int m);
    void ShowFullImg();
    void clickPartImg(int x,int y);

protected:
    void mousePressEvent(QMouseEvent *e);

public slots:
    void reMagnify(int m);
    void backToEntirety();

private:
    //the size of window
    const int windowWidth = 960;
    const int windowHeight = 640;

    const int margin = 40;
    //img
    QString mFilePath;
    QImage *mQtImg;
    openslide_t *mImg;
    uchar *imgData;

    int mMagnify;
    QLabel *mLab;

    void DivideLine();
    void ShowPartImg(int x,int y,int dx, int dy);


    bool mIsLarge;
};

#endif // TIFFIMGWIDGET_H
