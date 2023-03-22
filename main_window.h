#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QPropertyAnimation>


class SunSystemScene;

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

private:
    QPropertyAnimation windowUpdateAnimation;
    SunSystemScene *graphicScene = nullptr;

    const int fpsSetting;
    int framesCount;

    void setframesCount(int val);
    void setCursorToWindowCenter();
};

#endif // MAINWINDOW_H
