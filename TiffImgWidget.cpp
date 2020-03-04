#include "TiffImgWidget.h"
#include <QPluginLoader>
#include <QDir>
#include <QtWidgets/QApplication>
#include <iostream>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

TiffImgWidget::TiffImgWidget()
{
    this->resize(windowWidth, windowHeight);
    this->setMinimumSize(windowWidth, windowHeight);
    mLab = new QLabel(this);
    mMagnify = 8;
    mQtImg = NULL;
    imgData = NULL;
    mIsLarge = 0;
}

TiffImgWidget::TiffImgWidget(QWidget *parent):QWidget(parent)
{
    this->resize(windowWidth, windowHeight);
    this->setMinimumSize(windowWidth, windowHeight);
    mLab = new QLabel(this);
    mMagnify = 8;
    mQtImg = NULL;
    imgData = NULL;
    mIsLarge = 0;
}

bool TiffImgWidget::OpenImage(const QString &filePath)
{
    //file path
    QByteArray ba = filePath->toLatin1();
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
void TiffImgWidget::ShowPartImg(int x,int y,int dx, int dy)
{
    int64_t w,h;
    openslide_get_level0_dimensions(mImg,&w,&h);
    int level = openslide_get_best_level_for_downsample(mImg,std::max((double)w/dx,(double)h/dy));
    int64_t lw, lh;
    openslide_get_layer_dimensions(mImg,level,&lw,&lh);
    dx = dx*lw/w;
    dy = dy*lh/h;

    if(imgData) delete imgData;
    if(mQtImg) delete mQtImg;

    imgData = new uchar[dx*dy*4];
    openslide_read_region(mImg,(uint32_t*)imgData,x,y,level,dx,dy);
    mQtImg = new QImage(imgData, dx, dy, QImage::Format_ARGB32);
    *mQtImg = mQtImg->scaled(windowWidth,windowHeight,Qt::KeepAspectRatio);
    mLab->setPixmap(QPixmap::fromImage(*mQtImg));
}
void TiffImgWidget::mousePressEvent(QMouseEvent *event)
{
    if(mIsLarge) return ;
    mIsLarge = 1;
    clickPartImg(event->x(), event->y());
}
//(x,y) position of mouse click
void TiffImgWidget::clickPartImg(int x,int y)
{
    x /= mQtImg->width()/mMagnify;
    y /= mQtImg->height()/mMagnify;

    int64_t w,h;
    openslide_get_level0_dimensions(mImg,&w,&h);
    int dx=w/mMagnify, dy=h/mMagnify;
    x*=dx;
    y*=dy;

    dx+=margin*2;
    dy+=margin*2;
    x = std::max(x-margin,0);
    y = std::max(y-margin,0);
    ShowPartImg(x,y,dx,dy);
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
    case 0:SetMagnify(8);
        break;
    case 1:SetMagnify(16);
        break;
    case 2:SetMagnify(20);
        break;
    default:
        break;
    }
}
void TiffImgWidget::backToEntirety()
{
    if(mIsLarge){
        mIsLarge = 0;
        ShowFullImg();
    }
}
