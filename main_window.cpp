#include "main_window.h"
#include "graphic_scene.h"


MainWindow::MainWindow(QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
{}

void MainWindow::initializeGL()
{
    if(graphicScene)
        graphicScene->initialize();
}

void MainWindow::paintGL()
{
    if(graphicScene)
        graphicScene->paint();
}

GraphicScene* MainWindow::scene()
{
    return graphicScene;
}

void MainWindow::setScene(GraphicScene *scene)
{
    graphicScene = scene;
}
