#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QPropertyAnimation>
#include <QOpenGLDebugMessage>

class SunSystemScene;
class QOpenGLDebugLogger;

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

    Q_PROPERTY(int framesCount MEMBER framesCount WRITE setframesCount)

public:
    explicit MainWindow(QWindow *parent = nullptr);
    ~MainWindow() = default;

    void initializeGL() override;
    void paintGL() override;

    SunSystemScene* scene();
    void setScene(SunSystemScene *scene);

protected:
    void keyPressEvent(QKeyEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void focusInEvent(QFocusEvent *ev) override;

private:
    QPropertyAnimation windowUpdateAnimation;
    SunSystemScene *graphicScene = nullptr;
    QOpenGLDebugLogger *openGlDebugLogger = nullptr;

    const int fpsSetting;
    int framesCount;

    float mouseSensetive;

    void setframesCount(int val);
    void setCursorToWindowCenter();
    QPoint windowCenterInGlobal() const;

private slots:
    void handleLoggedMessage(const QOpenGLDebugMessage &message);
};

#endif // MAINWINDOW_H
