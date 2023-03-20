#include "main_window.h"
#include "sunsystem_scene.h"

#include <QGuiApplication>


int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    MainWindow mainWindow;

    SunSystemScene sunSystemScene(&mainWindow);
    mainWindow.setScene(&sunSystemScene);
    mainWindow.resize(1200, 800);
    mainWindow.show();

    return a.exec();
}
