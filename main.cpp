#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Configuration de l'application
    a.setApplicationName("Jeu du Pendu");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("MonOrganisation");
    
    MainWindow w;
    w.show();

    return a.exec();
}