#include "main_window.h"
#include "sunsystem_scene.h"

#include <QKeyEvent>


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

    setMouseGrabEnabled(true);
    QCursor blankCursor{ this->cursor() };
    blankCursor.setShape(Qt::BlankCursor);
    this->setCursor(blankCursor);

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


SunSystemScene* MainWindow::scene()
{
    return graphicScene;
}


void MainWindow::setScene(SunSystemScene *scene)
{
    graphicScene = scene;
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    QVector<int> keysToMoveForward{ Qt::Key_W, Qt::Key_Up };
    QVector<int> keysToMoveBack{ Qt::Key_S, Qt::Key_Down };
    QVector<int> keysToMoveLeft{ Qt::Key_A, Qt::Key_Left };
    QVector<int> keysToMoveRight{ Qt::Key_D, Qt::Key_Right };

    if( keysToMoveForward.contains(ev->key()))
        graphicScene->cameraMoveForward();
    else if (keysToMoveBack.contains(ev->key()))
        graphicScene->cameraMoveBack();
    else if (keysToMoveLeft.contains(ev->key()))
        graphicScene->cameraMoveLeft();
    else if (keysToMoveRight.contains(ev->key()))
        graphicScene->cameraMoveRight();
}


void MainWindow::setCursorToWindowCenter()
{
    QCursor newPositionCursor{ this->cursor() };
    QPoint windowPos = this->position();
    QSize windowSize = this->size();
    QPoint windowCenterPos = windowPos + QPoint(windowSize.width(), windowSize.height()) / 2.0f;
    newPositionCursor.setPos(windowCenterPos);
    setCursor(newPositionCursor);
}


void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    static QPointF oldCursorPosition = {0, 0};
    static const float moveMoveSensetive = 0.1;

    QPointF tmpDifferent = ev->globalPos() - oldCursorPosition;
    QVector2D mouseMovingVector{ static_cast<float>(tmpDifferent.y()), // Движение мыши по оси Y соответвует поворуту вокруг оси X, и наоборот.
                                 static_cast<float>(tmpDifferent.x()) };

    static QRectF rectForCursorMoving = {-20, -20, 40, 40};
    if (rectForCursorMoving.contains(tmpDifferent))
        graphicScene->cameraRotateByXYZAxises(mouseMovingVector * moveMoveSensetive);

    setCursorToWindowCenter();
    oldCursorPosition = this->cursor().pos();
}
