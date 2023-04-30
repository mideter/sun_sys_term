#include "sst_window.h"
#include "sunsystem_scene.h"

#include <QGuiApplication>


int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    sst::MainWindow mainWindow;

    SunSystemScene sunSystemScene;
    mainWindow.setScene(&sunSystemScene);
    mainWindow.showFullScreen();

    return a.exec();
}
