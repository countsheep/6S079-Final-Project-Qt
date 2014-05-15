#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string.h>
#include <locale>

#include <map>
#include <vector>

#include "Extra.h"
#include "camera.h"

#include <stdio.h>

#include <QtDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
