#include "sunsystem_scene.h"
#include "objfile_reader.h"

#include <sst_window.h>
#include <QtCore/QtMath>


SunSystemScene::SunSystemScene()
    : GraphicScene()
    , vertexBufferForEarthPlanet_(QOpenGLBuffer::VertexBuffer)
    , vertexBufferForMoonPlanet_(QOpenGLBuffer::VertexBuffer)
    , vertexBufferForSkybox_(QOpenGLBuffer::VertexBuffer)
    , rotationByEarthAxis_(this, "angleByEarthAxis_")
    , cameraPosition_( 0.0f, 0.0f, 25.0f)
    , cameraRotationAnglesXYZInDegrees_(0.0f, 0.0f, 0.0f)
    , worldStep_(0.5f)
{
    vertexBufferForEarthPlanet_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBufferForMoonPlanet_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBufferForSkybox_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    initializeObjectData();

    rotationByEarthAxis_.setStartValue(0);
    rotationByEarthAxis_.setEndValue(359);
    rotationByEarthAxis_.setDuration(180000);
    rotationByEarthAxis_.setLoopCount(-1);
    rotationByEarthAxis_.start();
}


SunSystemScene::~SunSystemScene()
{
    // Makes current window context to correct deleting OpenGL resources.
    window()->makeCurrent();
}


void SunSystemScene::initializeObjectData()
{
    skybox_.reset(new Skybox(":data/skybox/"));

    ObjFileReader objFileReader;
    earth3dModel_.reset(objFileReader.load(":data/Earth/Earth.obj"));
    qDebug() << "Count vertexes in Earth = " << earth3dModel_->countVertexes();
    qDebug() << "Earth Image size = " << earth3dModel_->mainTexture().size();

    moon3dModel_.reset(objFileReader.load(":data/Moon/Moon.obj"));
    qDebug() << "Count vertexes in Moon = " << earth3dModel_->countVertexes();
    qDebug() << "Moon Image size = " << earth3dModel_->mainTexture().size();
}


void SunSystemScene::initialize()
{
    GraphicScene::initialize();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.8, 0.9, 0.8, 1.0);

    shaderProgram_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":vertex.shader");
    shaderProgram_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":fragment.shader");
    shaderProgram_->link();

    shaderProgram_->bind();

    vertexBufferForEarthPlanet_.create();
    vertexBufferForEarthPlanet_.bind();
    vertexBufferForEarthPlanet_.allocate(earth3dModel_->vertexData(), sizeof(Vertex) * earth3dModel_->countVertexes());

    vertexBufferForMoonPlanet_.create();
    vertexBufferForMoonPlanet_.bind(); // bind() must be called before allocate()
    vertexBufferForMoonPlanet_.allocate(moon3dModel_->vertexData(), sizeof(Vertex) * moon3dModel_->countVertexes());

    vertexBufferForSkybox_.create();
    vertexBufferForSkybox_.bind();
    vertexBufferForSkybox_.allocate(skybox_->vertexData(), sizeof(Vertex) * skybox_->getCountVertexes());

    textureEarthPlanet_.reset( new QOpenGLTexture{ earth3dModel_->mainTexture().mirrored() });
    textureEarthPlanet_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureEarthPlanet_->setMagnificationFilter(QOpenGLTexture::Linear);

    textureMoonPlanet_.reset( new QOpenGLTexture{ moon3dModel_->mainTexture().mirrored() } );
    textureMoonPlanet_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureMoonPlanet_->setMagnificationFilter(QOpenGLTexture::Linear);

    for (int i = 0; i < 6; i++)
    {
        textureSkybox_[i].reset( new QOpenGLTexture{ skybox_->getTextures()[i].mirrored() } );
        textureSkybox_[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        textureSkybox_[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }
}


void SunSystemScene::paint()
{
    projectionMatrix_.setToIdentity();
    float aspectRatio = qreal(window()->width()) / window()->height();
    projectionMatrix_.perspective(45.0f, aspectRatio, 0.1, 10000.0f);

    viewMatrixWithoutTranslate_.setToIdentity();
    viewMatrixWithoutTranslate_.rotate(cameraRotationAnglesXYZInDegrees_[0], QVector3D{ 1, 0, 0 });
    viewMatrixWithoutTranslate_.rotate(cameraRotationAnglesXYZInDegrees_[1], QVector3D{ 0, 1, 0 });
   // viewMatrix.rotate(cameraRatationAnglesXYZ[2], QVector3D{ 0, 0, 1});
    viewMatrix_ = viewMatrixWithoutTranslate_;
    viewMatrix_.translate(-cameraPosition_); // Относительно камеры все объекты имеют инвертированное положение.
    // Матричные преобразования идут в обратном вызовам функций порядке.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window()->width(), window()->height());

    glEnable(GL_CULL_FACE); // Включаем отсечение граней.
    glCullFace(GL_BACK); // Устанавливаем отсечение граней, обращенных к наблюдателю нелицевой стороной.

    shaderProgram_->setUniformValue("light.position", QVector3D{ viewMatrix_ * QVector4D(6, 6, 16, 1)} );
    shaderProgram_->setUniformValue("light.intensity", QVector3D(1.0f, 1.0f, 1.0f));

    QMatrix4x4 modelMatrix;
    modelMatrix.rotate(angleByEarthAxis_, QVector3D{0, 1, 0});
    QMatrix4x4 modelViewMatrix = viewMatrix_ * modelMatrix;

    paintObject(vertexBufferForEarthPlanet_, textureEarthPlanet_.get(), earth3dModel_->mainMaterial(), earth3dModel_->countVertexes(), modelViewMatrix);
    GLenum error = GL_NO_ERROR;
    do {
        error = glGetError();
        if (error != GL_NO_ERROR)
            qDebug() << "error occured number " << error;
    } while (error != GL_NO_ERROR);

    modelMatrix.setToIdentity();
    modelMatrix.rotate(angleByEarthAxis_, QVector3D{0, 1, 0});
    modelMatrix.translate(QVector3D(20.0f, 0.0f, 0.0f));
    modelViewMatrix = viewMatrix_ * modelMatrix;
    paintObject(vertexBufferForMoonPlanet_, textureMoonPlanet_.get(), moon3dModel_->mainMaterial(), moon3dModel_->countVertexes(), modelViewMatrix);

    paintSkybox();
}


void SunSystemScene::paintObject(QOpenGLBuffer &vertexBuffer, QOpenGLTexture *texture,
                                 const LightInteractingMaterial& material,
                                 const int countVertexes,
                                 const QMatrix4x4 &modelViewMatrix)
{
    shaderProgram_->setUniformValue("projectionMatrix", projectionMatrix_);
    shaderProgram_->setUniformValue("modelViewMatrix", modelViewMatrix);
    shaderProgram_->setUniformValue("modelViewProjectionMatrix", projectionMatrix_ * modelViewMatrix);
    shaderProgram_->setUniformValue("normalMatrix", modelViewMatrix.normalMatrix());

    LightInteractingMaterial mainMaterial = material;
    shaderProgram_->setUniformValue("mat.ka", mainMaterial.ka());
    shaderProgram_->setUniformValue("mat.kd", mainMaterial.kd());
    shaderProgram_->setUniformValue("mat.ks", mainMaterial.ks());
    shaderProgram_->setUniformValue("mat.shininess", mainMaterial.shininess());

    vertexBuffer.bind();
    texture->bind();
    shaderProgram_->setUniformValue("useTexture", true);
    shaderProgram_->setUniformValue("useNormal", true);

    shaderProgram_->enableAttributeArray("vertexPosition");
    shaderProgram_->enableAttributeArray("vertexNormal");
    shaderProgram_->enableAttributeArray("vertexTextureCoord");

    shaderProgram_->setAttributeBuffer("vertexPosition", GL_FLOAT, Vertex::positionAttribOffset(), 3, sizeof(Vertex));
    shaderProgram_->setAttributeBuffer("vertexNormal", GL_FLOAT, Vertex::normalAttribOffset(), 3, sizeof(Vertex));
    shaderProgram_->setAttributeBuffer("vertexTextureCoord", GL_FLOAT, Vertex::textureCoordsAttribOffset(), 2, sizeof(Vertex));

    glDrawArrays(GL_TRIANGLES, 0, countVertexes );
}


void SunSystemScene::paintSkybox()
{
    shaderProgram_->setUniformValue("modelViewProjectionMatrix", projectionMatrix_ * viewMatrixWithoutTranslate_);

    vertexBufferForSkybox_.bind();
    shaderProgram_->enableAttributeArray("vertexPosition");
    shaderProgram_->enableAttributeArray("vertexNormal");
    shaderProgram_->enableAttributeArray("vertexTextureCoord");

    shaderProgram_->setAttributeBuffer("vertexPosition", GL_FLOAT, Vertex::positionAttribOffset(), 3, sizeof(Vertex));
    shaderProgram_->setAttributeBuffer("vertexNormal", GL_FLOAT, Vertex::normalAttribOffset(), 3, sizeof(Vertex));
    shaderProgram_->setAttributeBuffer("vertexTextureCoord", GL_FLOAT, Vertex::textureCoordsAttribOffset(), 2, sizeof(Vertex));

    shaderProgram_->setUniformValue("useTexture", true);
    shaderProgram_->setUniformValue("useNormal", false);
    for(int i = 0; i < 6; i++) {
        textureSkybox_[i]->bind();
        //glDrawArrays(GL_QUADS, i * 6, 6);
        glDrawArrays(GL_TRIANGLES, i * 6, 6);
    }
}


void SunSystemScene::cameraMoveForward()
{
    float zMove = -worldStep_ * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]));
    float xMove = worldStep_ * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]));
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveBack()
{
    float zMove = worldStep_ * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]));
    float xMove = -worldStep_ * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]));
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveLeft()
{
    float zMove = worldStep_ * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]) + M_PI_2);
    float xMove = -worldStep_ * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]) + M_PI_2);
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveRight()
{
    float zMove = -worldStep_ * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]) + M_PI_2);
    float xMove = worldStep_ * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees_[1]) + M_PI_2);
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveUp()
{
    float yMove = worldStep_;
    cameraMove(QVector3D(0.0f, yMove, 0.0f));
}


void SunSystemScene::cameraMoveDown()
{
    float yMove = -worldStep_;
    cameraMove(QVector3D(0.0f, yMove, 0.0f));
}


void SunSystemScene::cameraMove(const QVector3D deltaToMove)
{
    cameraPosition_ += deltaToMove;
}


void SunSystemScene::cameraRotateByXYZAxises(const QVector3D &xyzRotate)
{
    cameraRotationAnglesXYZInDegrees_ += xyzRotate;
}


void SunSystemScene::cameraRotateByXYZAxises(float xRotate, float yRotate, float zRotate)
{
    cameraRotateByXYZAxises(QVector3D{ xRotate, yRotate, zRotate });
}
