#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //La classe QApplication gère le flux de contrôle et les paramètres principaux de l'application interface graphic user
    MainWindow window;         //Une fenêtre principale fournit un cadre pour créer l'interface utilisateur d'une application
    window.setStyleSheet("QMainWindow {background: 'black';}");
    window.show();

    return a.exec();
}
