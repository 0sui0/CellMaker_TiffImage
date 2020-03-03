#include "TiffImgWidget.h"
#include <QPluginLoader>
#include <QDir>
#include <QtWidgets/QApplication>
#include <iostream>
#include <QDebug>
#include <QPainter>

TiffImgWidget::TiffImgWidget()
{
    this->resize(windowWidth, windowHeight);
    this->setMinimumSize(windowWidth, windowHeight);
    mLab = new QLabel(this);
    mMagnify = 5;
    mQtImg = NULL;
    imgData = NULL;
}

TiffImgWidget::TiffImgWidget(QWidget *parent):QWidget(parent)
{
    this->resize(windowWidth, windowHeight);
    this->setMinimumSize(windowWidth, windowHeight);
    mLab = new QLabel(this);
    mMagnify = 5;
    mQtImg = NULL;
    imgData = NULL;
}

bool TiffImgWidget::OpenImage(QString filePath)
{
    //file path
    QByteArray ba = filePath.toLatin1();
    const char *cFilePath = ba.data();
    mFilePath = filePath;

    //open img
    if(openslide_can_open(cFilePath)){
        mImg = openslide_open(cFilePath);
        this->ShowFullImg();
        return 1;
    } else {        //open filed
        return 0;
    }
}

void TiffImgWidget::ShowFullImg()
{
    int64_t w,h;
    openslide_get_level0_dimensions(mImg,&w,&h);
    qDebug()<<"w:"<<w<<" h:"<<h<<endl;
    int level = openslide_get_best_level_for_downsample(mImg,std::max((double)w/windowWidth,(double)h/windowHeight));
    qDebug()<<level;
    openslide_get_layer_dimensions(mImg,level,&w,&h);

    if(imgData) delete imgData;
    if(mQtImg) delete mQtImg;

    imgData = new uchar[w*h*4];
    openslide_read_region(mImg,(uint32_t*)imgData,0,0,level,w,h);
    mQtImg = new QImage(imgData, w, h, QImage::Format_ARGB32);
    *mQtImg = mQtImg->scaled(windowWidth,windowHeight,Qt::KeepAspectRatio);
    this->DivideLine();
    mLab->setPixmap(QPixmap::fromImage(*mQtImg));
}

void TiffImgWidget::SetMagnify(int m){
    this->mMagnify = m;
    this->ShowFullImg();
}

void TiffImgWidget::DivideLine(){
    QPainter p(mQtImg);
    //pen style
    p.setRenderHint(QPainter::Antialiasing, true);

    //draw line
    int w = mQtImg->width();
    int h = mQtImg->height();
    for(int t=w/mMagnify,i=t; i+mMagnify<w; i+=t)
        p.drawLine(QPoint(i,0), QPoint(i,h));
    for(int t=h/mMagnify,i=t; i+mMagnify<h; i+=t)
        p.drawLine(QPoint(0,i),QPoint(w,i));
}

void TiffImgWidget::reMagnify(int m){
    switch (m) {
    case 0:SetMagnify(4);
        break;
    case 1:SetMagnify(5);
        break;
    case 2:SetMagnify(8);
        break;
    default:
        break;
    }
}
