#include "main_window.h"
#include "graphic_scene.h"


MainWindow::MainWindow(QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
{
    QSurfaceFormat surfaceFormat(this->format());
    // Добавляем к формату поверхности уточнение, что используется версия для OpenGL ES.
    // Чтобы в шейдеры не добалялись #define указателей точности, которые их скрывают.
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGLES);
    setFormat(surfaceFormat);
}


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
