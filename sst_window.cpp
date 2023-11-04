#include "sst_window.h"
#include "sunsystem_scene.h"

#include <QKeyEvent>
#include <QOpenGLDebugLogger>


namespace sst {


MainWindow::MainWindow(QWindow *parent)
    : Window(parent)
    , windowUpdateAnimation(this, "framesCount")
{
    windowUpdateAnimation.setStartValue(1);
    windowUpdateAnimation.setEndValue(this->fpsSetting);
    windowUpdateAnimation.setDuration(1000);
    windowUpdateAnimation.setLoopCount(-1);
    windowUpdateAnimation.start();
}


Window::Window(QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
    , fpsSetting(60)
    , openGlDebugLogger{ new QOpenGLDebugLogger(this) }
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
}


void Window::initializeGL()
{
    this->makeCurrent(); // make current this window OpenGL context.
    bool doesOpenGLDebugLoggerInitialize = openGlDebugLogger->initialize(); // in current OpenGL context.
    qDebug() << "OpenGL debug logger was initialized: " << doesOpenGLDebugLoggerInitialize;
    connect(openGlDebugLogger, &QOpenGLDebugLogger::messageLogged, this, &MainWindow::handleLoggedMessage);
    openGlDebugLogger->startLogging();

    if(graphicScene)
        graphicScene->initialize();
}


void Window::handleLoggedMessage(const QOpenGLDebugMessage &message)
{
    qDebug() << "OpenGL debug message: " <<  message.message();
}


void Window::paintGL()
{
    if(graphicScene)
        graphicScene->paint();
}


void Window::setframesCount(int val)
{
    if (val > 0)
    {
        framesCount = val;
        this->update();
    }
}


SunSystemScene* Window::scene()
{
    return graphicScene;
}


void Window::setScene(SunSystemScene *scene)
{
    if (!scene)
        return;

    scene->setWindow(this);
    graphicScene = scene;
}


void Window::keyPressEvent(QKeyEvent *ev)
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


void Window::setCursorToWindowCenter()
{
    QCursor newPositionCursor{ this->cursor() };
    newPositionCursor.setPos( this->windowCenterInGlobal() );
    this->setCursor(newPositionCursor);
}


QPoint Window::windowCenterInGlobal() const
{
    QPoint windowPos = this->position();
    QSize windowSize = this->size();
    return windowPos + QPoint(windowSize.width(), windowSize.height()) / 2.0f;
}


void Window::mouseMoveEvent(QMouseEvent *ev)
{
    QPointF tmpDifferent = ev->globalPos() - this->windowCenterInGlobal();
    qDebug() << "движение мыши по y = " << tmpDifferent.y();
    QVector2D angleRotationsByXYAxises{ static_cast<float>(tmpDifferent.y()), 0} ;// Движение мыши по оси Y соответвует повороту вокруг оси X, и наоборот.
                                    //    static_cast<float>(tmpDifferent.x()) };
    qDebug() << "angleRotationsByXYAxises = " << angleRotationsByXYAxises.x();

    graphicScene->cameraRotateByXYZAxises(angleRotationsByXYAxises * mouseSensetive);
    setCursorToWindowCenter();
}


void Window::focusInEvent(QFocusEvent *)
{
    setCursorToWindowCenter();
}


} // namespace sst
