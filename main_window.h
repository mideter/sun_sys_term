#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>

class GraphicScene;

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = nullptr);
    ~MainWindow() = default;

    void initializeGL() override;
    void paintGL() override;

    GraphicScene* scene();
    void setScene(GraphicScene *scene);

private:
    GraphicScene *graphicScene = nullptr;
};

#endif // MAINWINDOW_H
