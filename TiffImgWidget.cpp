#include "TiffImgWidget.h"
#include <QPluginLoader>
#include <QDir>
#include <QtWidgets/QApplication>
#include <iostream>
#include <QDebug>

TiffImgWidget::TiffImgWidget()
{
    this->resize(windowWidth, windowHeight);
    mLab = new QLabel(this);
}

TiffImgWidget::TiffImgWidget(QWidget *parent):QWidget(parent)
{
    this->resize(windowWidth, windowHeight);
    mLab = new QLabel(this);
}

bool TiffImgWidget::OpenImage(QString filePath)
{
    QByteArray ba = filePath.toLatin1();
    const char *cFilePath = ba.data();
    mFilePath = filePath;
    if(openslide_can_open(cFilePath)){
        mImg = openslide_open(cFilePath);
        int64_t w,h;
        openslide_get_level0_dimensions(mImg,&w,&h);
        qDebug()<<"w:"<<w<<" h:"<<h<<endl;
        mLevel = openslide_get_best_level_for_downsample(mImg,std::max((double)w/windowWidth,(double)h/windowHeight));
        qDebug()<<mLevel;
        openslide_get_layer_dimensions(mImg,mLevel,&w,&h);

        imgData = new uchar[w*h*4];
        openslide_read_region(mImg,(uint32_t*)imgData,0,0,mLevel,w,h);
        mQtImg = new QImage(imgData, w, h, QImage::Format_ARGB32);
        *mQtImg = mQtImg->scaled(windowWidth,windowHeight,Qt::KeepAspectRatio);
        mLab->setPixmap(QPixmap::fromImage(*mQtImg));

        //openslide_t tmpImg;
       // openslide_get_le

    } else {        //open filed
        return 0;
    }

}
