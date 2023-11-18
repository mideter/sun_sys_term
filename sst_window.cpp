#include "sst_window.h"
#include "sunsystem_scene.h"

#include <QKeyEvent>
#include <QOpenGLDebugLogger>


namespace sst {


MainWindow::MainWindow(QWindow *parent)
    : Window(parent)
    , windowUpdateAnimation_(this, "framesCount_")
{
    windowUpdateAnimation_.setStartValue(1);
    windowUpdateAnimation_.setEndValue(this->fpsSetting_);
    windowUpdateAnimation_.setDuration(1000);
    windowUpdateAnimation_.setLoopCount(-1);
    windowUpdateAnimation_.start();
}


Window::Window(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
    , fpsSetting_(60)
    , openGlDebugLogger_{ new QOpenGLDebugLogger(this) }
    , mouseSensetive_(0.1)
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
    bool hasGlKhrDebug = this->context()->hasExtension(QByteArrayLiteral("GL_KHR_debug"));
    qDebug() << "sst: OpenGL Error: GL_KHR_debug extension is avaialble: " << hasGlKhrDebug;
    bool isOpenGLDebugLoggerInitialized = openGlDebugLogger_->initialize(); // in current OpenGL context.
    qDebug() << "OpenGL debug logger was initialized: " << isOpenGLDebugLoggerInitialized;
    connect(openGlDebugLogger_, &QOpenGLDebugLogger::messageLogged, this, &MainWindow::handleLoggedMessage);
    openGlDebugLogger_->startLogging();

    if(graphicScene_)
        graphicScene_->initialize();
}


void Window::handleLoggedMessage(const QOpenGLDebugMessage &message)
{
    qDebug() << "OpenGL debug message: " <<  message.message();
}


void Window::paintGL()
{
    if(graphicScene_)
        graphicScene_->paint();
}


void Window::setframesCount(int val)
{
    if (val > 0)
    {
        framesCount_ = val;
        this->update();
    }
}


SunSystemScene* Window::scene()
{
    return graphicScene_;
}


void Window::setScene(SunSystemScene *scene)
{
    if (!scene)
        return;

    scene->setWindow(this);
    graphicScene_ = scene;
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
        graphicScene_->cameraMoveForward();
    else if (keysToMoveBack.contains(ev->key()))
        graphicScene_->cameraMoveBack();
    else if (keysToMoveLeft.contains(ev->key()))
        graphicScene_->cameraMoveLeft();
    else if (keysToMoveRight.contains(ev->key()))
        graphicScene_->cameraMoveRight();
    else if (keysToMoveUp.contains(ev->key()))
        graphicScene_->cameraMoveUp();
    else if (keysToMoveDown.contains(ev->key()))
        graphicScene_->cameraMoveDown();
}


void Window::setCursorToWindowCenter()
{
    QCursor newCursor{ this->cursor() };
    newCursor.setPos( this->windowCenterInGlobal() );
    newCursor.setShape(Qt::CursorShape::BlankCursor);
    this->setCursor(newCursor);
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
    QVector2D angleRotationsByXYAxises{ static_cast<float>(tmpDifferent.y()), // Движение мыши по оси Y соответвует повороту вокруг оси X, и наоборот.
                                        static_cast<float>(tmpDifferent.x()) };

    graphicScene_->cameraRotateByXYZAxises(angleRotationsByXYAxises * mouseSensetive_);
    setCursorToWindowCenter();
}


void Window::focusInEvent(QFocusEvent *)
{
    setCursorToWindowCenter();
}


} // namespace sst
