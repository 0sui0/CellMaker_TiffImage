
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtPlugin>
#include <QDebug>
#include <QImageReader>
#include "openslide.h"
#include <iostream>
#include "TiffImgWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TiffImgWidget w;
    w.OpenImage("C:\\Users\\sui\\Desktop\\CellMarker\\t.tif");
    //std::cout << "111";
    //openslide_t* img =  openslide_open("C:\\Users\\sui\\Desktop\\CellMarker\\t.tif");
    //std::cout << openslide_get_level_count(img);


    //w.OpenImage("C:\\Users\\sui\\Desktop\\CellMarker\\t.tif");
    w.show();

    return a.exec();
}
