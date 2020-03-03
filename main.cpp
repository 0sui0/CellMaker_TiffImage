
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtPlugin>
#include <QDebug>
#include <QImageReader>
#include "openslide.h"
#include <iostream>
#include <QComboBox>
#include <QLayout>
#include "TiffImgWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget mainWin;
    QHBoxLayout mainLayout;
    mainLayout.setSizeConstraint(QLayout::SetNoConstraint);

    TiffImgWidget w;


    mainLayout.addWidget(&w);

    QVBoxLayout btnBox;

    QComboBox magnifyBox;
    btnBox.addWidget(&magnifyBox);

    magnifyBox.addItem(QWidget::tr("x4"));
    magnifyBox.addItem(QWidget::tr("x5"));
    magnifyBox.addItem(QWidget::tr("x8"));
    magnifyBox.setCurrentIndex(1);

    mainLayout.addLayout(&btnBox);
    mainWin.setLayout(&mainLayout);

    QObject::connect(&magnifyBox, SIGNAL(currentIndexChanged(int)) ,&w, SLOT(reMagnify(int)));
    w.OpenImage("C:\\Users\\sui\\Desktop\\CellMarker\\t.tif");

    mainWin.show();

    return a.exec();
}
