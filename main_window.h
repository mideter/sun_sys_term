#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QPropertyAnimation>


class GraphicScene;

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

    Q_PROPERTY(int framesCount MEMBER framesCount WRITE setframesCount)

public:
    explicit MainWindow(QWindow *parent = nullptr);
    ~MainWindow() = default;

    void initializeGL() override;
    void paintGL() override;

    GraphicScene* scene();
    void setScene(GraphicScene *scene);

private:
    QPropertyAnimation windowUpdateAnimation;
    GraphicScene *graphicScene = nullptr;

    const int fpsSetting;
    int framesCount;

    void setframesCount(int val);
};

#endif // MAINWINDOW_H
