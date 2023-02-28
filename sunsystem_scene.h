#ifndef SUNSYSTEMSCENE_H
#define SUNSYSTEMSCENE_H

#include "graphic_scene.h"

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>

struct ScenePoint {
    QVector3D coords;
    QVector3D normal;
    ScenePoint(const QVector3D &c = QVector3D(),
               const QVector3D &n = QVector3D())
        : coords(c), normal(n)
    {}
};


class SunSystemScene : public GraphicScene
{
    Q_OBJECT

public:
    explicit SunSystemScene(QOpenGLWindow *window);
    ~SunSystemScene() = default;

    void initialize() override;
    void paint() override;

protected:
    void initializeObjectData();
    void paintObject(const QMatrix4x4 &mvMatrix);

private:
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QVector<ScenePoint> scenePoints;
};

#endif // SUNSYSTEMSCENE_H
