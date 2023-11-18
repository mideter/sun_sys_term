#ifndef SST_MAINWINDOW_H
#define SST_MAINWINDOW_H

#include <QOpenGLWindow>
#include <QPropertyAnimation>
#include <QOpenGLDebugMessage>

class SunSystemScene;
class QOpenGLDebugLogger;


namespace sst {


class Window : public QOpenGLWindow{
    Q_OBJECT

public:
    explicit Window(QWindow *parent = nullptr);
    ~Window() = default;

    void initializeGL() override;
    void paintGL() override;

    SunSystemScene* scene();
    void setScene(SunSystemScene *scene);

protected:
    const int fpsSetting_;
    int framesCount_;

    void setframesCount(int val);

    void keyPressEvent(QKeyEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void focusInEvent(QFocusEvent *ev) override;

private:
    SunSystemScene *graphicScene_ = nullptr;
    QOpenGLDebugLogger *openGlDebugLogger_ = nullptr;

    float mouseSensetive_;

    void setCursorToWindowCenter();
    QPoint windowCenterInGlobal() const;

private slots:
    void handleLoggedMessage(const QOpenGLDebugMessage &message);
};


class MainWindow : public Window
{
    Q_OBJECT

    Q_PROPERTY(int framesCount_ MEMBER framesCount_ WRITE setframesCount)

public:
    explicit MainWindow(QWindow *parent = nullptr);
    ~MainWindow() = default;

private:
    QPropertyAnimation windowUpdateAnimation_;
};


} // namespace sst


#endif // SST_MAINWINDOW_H
