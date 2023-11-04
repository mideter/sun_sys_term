#ifndef SST_SUNSYSTEMSCENE_H
#define SST_SUNSYSTEMSCENE_H

#include "graphic_scene.h"
#include "model_3dobject.h"
#include "skybox.h"

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QPropertyAnimation>

#include <memory>


class SunSystemScene : public GraphicScene {
    Q_OBJECT

    Q_PROPERTY(float angleByEarthAxis MEMBER angleByEarthAxis)

public:
    explicit SunSystemScene();
    ~SunSystemScene();

    void initialize() override;
    void paint() override;

    void cameraMoveForward();
    void cameraMoveBack();
    void cameraMoveLeft();
    void cameraMoveRight();
    void cameraMoveUp();
    void cameraMoveDown();
    void cameraRotateByXYZAxises(float xRotate, float yRotate, float zRotate = 0);
    void cameraRotateByXYZAxises(const QVector3D &xyzRotate);

protected:
    void initializeObjectData();
    void paintObject(QOpenGLBuffer &vertexBuffer, QOpenGLTexture *texture, const QMatrix4x4 &mvMatrix);
    void paintSkybox();

private:
    QMatrix4x4 viewMatrix;
    QMatrix4x4 viewMatrixWithoutTranslate;
    QMatrix4x4 projectionMatrix;

    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<Model3DObject> earth3DModel;
    std::unique_ptr<Model3DObject> moon3DModel;

    QOpenGLBuffer vertexBufferForEarthPlanet;
    QOpenGLBuffer vertexBufferForMoonPlanet;
    QOpenGLBuffer vertexBufferForSkybox;
    std::unique_ptr<QOpenGLTexture> textureEarthPlanet = nullptr;
    std::unique_ptr<QOpenGLTexture> textureMoonPlanet = nullptr;
    std::unique_ptr<QOpenGLTexture> textureSkybox[6];

    float angleByEarthAxis;
    QPropertyAnimation rotationByEarthAxis;

    QVector3D cameraPosition;
    QVector3D cameraRotationAnglesXYZInDegrees;

    float worldStep;

    void cameraMove(const QVector3D deltaToMove);
};


#endif // SST_SUNSYSTEMSCENE_H
