#include "QuaMainWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile tFile(":/stylesheets/stylesheet.css");

    if( tFile.open(QIODevice::ReadOnly) )
    {
        a.setStyleSheet(tFile.readAll());
        tFile.close();
    }

    IBFMainWindow w;
    w.show();

    return a.exec();
}

//  TODO
//
//  - Spell Checker

//  - Link
//  - Unlink

//  - Image
//  - Table
//  - Code

//  - Find / Replace


//  HTML
//Tag       Description	Comment

//dd        Definition data
//dl        Definition list	Supports the standard block attributes.
//dt        Definition term	Supports the standard block attributes.

//li        List item
//nobr  	Non-breakable text

