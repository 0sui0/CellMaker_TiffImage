
#include <QtWidgets/QApplication>
#include <QComboBox>
#include <QLayout>
#include <QPushButton>
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

    magnifyBox.addItem(QWidget::tr("x8"));
    magnifyBox.addItem(QWidget::tr("x16"));
    magnifyBox.addItem(QWidget::tr("x20"));
    magnifyBox.setCurrentIndex(0);

    mainLayout.addLayout(&btnBox);
    mainWin.setLayout(&mainLayout);

    QPushButton backBtn;
    backBtn.setText("back");
    btnBox.addWidget(&backBtn);

    QObject::connect(&backBtn, SIGNAL(clicked(bool)), &w, SLOT(backToEntirety()));
    QObject::connect(&magnifyBox, SIGNAL(currentIndexChanged(int)) ,&w, SLOT(reMagnify(int)));
    w.OpenImage("C:\\Users\\sui\\Desktop\\CellMarker\\t.tif");

    mainWin.show();

    return a.exec();
}
