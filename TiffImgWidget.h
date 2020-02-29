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
private:
    const int windowWidth = 600;
    const int windowHeight = 500;

    QString mFilePath;
    QImage *mQtImg;
    openslide_t *mImg;
    uchar *imgData;
    int mLevel;
    QLabel *mLab;


};

#endif // TIFFIMGWIDGET_H
