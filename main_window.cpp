#include "main_window.h"
#include "graphic_scene.h"


MainWindow::MainWindow(QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
    , windowUpdateAnimation(this, "framesCount")
    , fpsSetting(60)
{
    QSurfaceFormat surfaceFormat(this->format());
    // Добавляем к формату поверхности уточнение, что используется версия для OpenGL ES.
    // Чтобы в шейдеры не добалялись #define указателей точности, которые их скрывают.
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGLES);
    setFormat(surfaceFormat);

    windowUpdateAnimation.setStartValue(1);
    windowUpdateAnimation.setEndValue(this->fpsSetting);
    windowUpdateAnimation.setDuration(1000);
    windowUpdateAnimation.setLoopCount(-1);
    windowUpdateAnimation.start();
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


void MainWindow::setframesCount(int val)
{
    if (val > 0)
    {
        framesCount = val;
        this->update();
    }
}


GraphicScene* MainWindow::scene()
{
    return graphicScene;
}


void MainWindow::setScene(GraphicScene *scene)
{
    graphicScene = scene;
}
