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

    Q_PROPERTY(float angleByEarthAxis_ MEMBER angleByEarthAxis_)

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
    void paintObject(QOpenGLBuffer &vertexBuffer, QOpenGLTexture *texture,
                     const LightInteractingMaterial& material,
                     const int countVertexes, const QMatrix4x4 &mvMatrix);
    void paintSkybox();

private:
    QMatrix4x4 viewMatrix_;
    QMatrix4x4 viewMatrixWithoutTranslate_;
    QMatrix4x4 projectionMatrix_;

    std::unique_ptr<Skybox> skybox_;
    std::unique_ptr<Model3DObject> earth3dModel_;
    std::unique_ptr<Model3DObject> moon3dModel_;

    QOpenGLBuffer vertexBufferForEarthPlanet_;
    QOpenGLBuffer vertexBufferForMoonPlanet_;
    QOpenGLBuffer vertexBufferForSkybox_;
    std::unique_ptr<QOpenGLTexture> textureEarthPlanet_ = nullptr;
    std::unique_ptr<QOpenGLTexture> textureMoonPlanet_ = nullptr;
    std::unique_ptr<QOpenGLTexture> textureSkybox_[6];

    float angleByEarthAxis_;
    QPropertyAnimation rotationByEarthAxis_;

    QVector3D cameraPosition_;
    QVector3D cameraRotationAnglesXYZInDegrees_;

    float worldStep_;

    void cameraMove(const QVector3D deltaToMove);
};


#endif // SST_SUNSYSTEMSCENE_H
