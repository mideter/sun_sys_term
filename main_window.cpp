#include "main_window.h"
#include "sunsystem_scene.h"

#include <QKeyEvent>
#include <QOpenGLDebugLogger>


MainWindow::MainWindow(QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
    , windowUpdateAnimation(this, "framesCount")
    , openGlDebugLogger{ new QOpenGLDebugLogger(this) }
    , fpsSetting(60)
    , mouseSensetive(0.1)
{
    QSurfaceFormat surfaceFormat(this->format());
    // Добавляем к формату поверхности уточнение, что используется версия для OpenGL ES.
    // Чтобы в шейдеры не добалялись #define указателей точности, которые их скрывают.
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGLES);
    surfaceFormat.setDepthBufferSize(24);
    surfaceFormat.setOption(QSurfaceFormat::DebugContext);
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
    this->makeCurrent(); // make current this window OpenGL context.
    bool doesOpenGLDebugLoggerInitialize = openGlDebugLogger->initialize(); // in current OpenGL context.
    qDebug() << "OpenGL debug logger was initialized: " << doesOpenGLDebugLoggerInitialize;

    if(graphicScene)
        graphicScene->initialize();
    connect(openGlDebugLogger, &QOpenGLDebugLogger::messageLogged, this, &MainWindow::handleLoggedMessage);
    openGlDebugLogger->startLogging();
}


void MainWindow::handleLoggedMessage(const QOpenGLDebugMessage &message)
{
    qDebug() << "OpenGL debug message: " <<  message.message();
}


void MainWindow::paintGL()
{
    if(graphicScene)
        graphicScene->paint();

    const QList<QOpenGLDebugMessage> messages = openGlDebugLogger->loggedMessages();
    for (QOpenGLDebugMessage message : messages)
        qDebug() << message;
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
    QVector<int> keysToMoveUp{ Qt::Key_Space };
    QVector<int> keysToMoveDown{ Qt::Key_X };

    if( keysToMoveForward.contains(ev->key()))
        graphicScene->cameraMoveForward();
    else if (keysToMoveBack.contains(ev->key()))
        graphicScene->cameraMoveBack();
    else if (keysToMoveLeft.contains(ev->key()))
        graphicScene->cameraMoveLeft();
    else if (keysToMoveRight.contains(ev->key()))
        graphicScene->cameraMoveRight();
    else if (keysToMoveUp.contains(ev->key()))
        graphicScene->cameraMoveUp();
    else if (keysToMoveDown.contains(ev->key()))
        graphicScene->cameraMoveDown();
}


void MainWindow::setCursorToWindowCenter()
{
    QCursor newPositionCursor{ this->cursor() };
    newPositionCursor.setPos( this->windowCenterInGlobal() );
    setCursor(newPositionCursor);
}


QPoint MainWindow::windowCenterInGlobal() const
{
    QPoint windowPos = this->position();
    QSize windowSize = this->size();
    return windowPos + QPoint(windowSize.width(), windowSize.height()) / 2.0f;
}


void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    QPointF tmpDifferent = ev->globalPos() - this->windowCenterInGlobal();
    QVector2D angleRotationsByXYAxises{ static_cast<float>(tmpDifferent.y()), // Движение мыши по оси Y соответвует поворуту вокруг оси X, и наоборот.
                                        static_cast<float>(tmpDifferent.x()) };

    graphicScene->cameraRotateByXYZAxises(angleRotationsByXYAxises * mouseSensetive);
    setCursorToWindowCenter();
}


void MainWindow::focusInEvent(QFocusEvent *)
{
    setCursorToWindowCenter();
}
