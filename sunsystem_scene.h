#ifndef SUNSYSTEMSCENE_H
#define SUNSYSTEMSCENE_H

#include "graphic_scene.h"
#include "model_3dobject.h"

#include <memory>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QPropertyAnimation>


class SunSystemScene : public GraphicScene
{
    Q_OBJECT

    Q_PROPERTY(float angleByEarthAxis MEMBER angleByEarthAxis)

public:
    explicit SunSystemScene(QOpenGLWindow *window);
    ~SunSystemScene();

    void initialize() override;
    void paint() override;

    void cameraMoveForward();
    void cameraMoveBack();
    void cameraMoveLeft();
    void cameraMoveRight();
    void cameraRotateByXYZAxises(float xRotate, float yRotate, float zRotate = 0);
    void cameraRotateByXYZAxises(const QVector3D &xyzRotate);

protected:
    void initializeObjectData();
    void paintObject(const QMatrix4x4 &mvMatrix);

private:
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    std::unique_ptr<Model3DObject> earth3DModel;

    QOpenGLBuffer vertexBuffer;
    std::unique_ptr<QOpenGLTexture> texture = nullptr;

    float angleByEarthAxis;
    QPropertyAnimation rotationByEarthAxis;

    QVector3D cameraPosition;
    QVector3D cameraRatationAnglesXYZ;

    void cameraMove(const QVector3D deltaToMove);
};


#endif // SUNSYSTEMSCENE_H
