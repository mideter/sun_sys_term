#include "sunsystem_scene.h"
#include "objfile_reader.h"

#include <QOpenGLWindow>
#include <QtCore/QtMath>


SunSystemScene::SunSystemScene(QOpenGLWindow *window)
    : GraphicScene(window)
    , vertexBufferForMoonPlanet(QOpenGLBuffer::VertexBuffer)
    , vertexBufferForSkybox(QOpenGLBuffer::VertexBuffer)
    , rotationByEarthAxis(this, "angleByEarthAxis")
    , cameraPosition( 0.0f, 0.0f, 25.0f)
    , cameraRotationAnglesXYZInDegrees(0.0f, 0.0f, 0.0f)
    , worldStep(0.5f)
{
    vertexBufferForMoonPlanet.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBufferForSkybox.setUsagePattern(QOpenGLBuffer::StaticDraw);
    initializeObjectData();

    rotationByEarthAxis.setStartValue(0);
    rotationByEarthAxis.setEndValue(359);
    rotationByEarthAxis.setDuration(180000);
    rotationByEarthAxis.setLoopCount(-1);
    rotationByEarthAxis.start();
}


SunSystemScene::~SunSystemScene()
{
    // Makes current window context to correct deleting OpenGL resources.
    window()->makeCurrent();
}


void SunSystemScene::initializeObjectData()
{
    skybox.reset(new Skybox("data/skybox/"));

    ObjFileReader objFileReader;
    earth3DModel.reset(objFileReader.load("data/Earth/Earth.obj"));
    moon3DModel.reset(objFileReader.load("data/Moon/Moon.obj"));
}


void SunSystemScene::initialize()
{
    GraphicScene::initialize();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.8, 0.9, 0.8, 1.0);

    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":vertex.shader");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":fragment.shader");
    shaderProgram->link();

    shaderProgram->bind();

    vertexBufferForEarthPlanet.create();
    vertexBufferForEarthPlanet.bind();
    vertexBufferForEarthPlanet.allocate(earth3DModel->vertexData(), sizeof(Vertex) * earth3DModel->getCountVertexes());

    vertexBufferForMoonPlanet.create();
    vertexBufferForMoonPlanet.bind(); // bind() must be called before allocate()
    vertexBufferForMoonPlanet.allocate(moon3DModel->vertexData(), sizeof(Vertex) * moon3DModel->getCountVertexes());

    vertexBufferForSkybox.create();
    vertexBufferForSkybox.bind();
    vertexBufferForSkybox.allocate(skybox->vertexData(), sizeof(Vertex) * skybox->getCountVertexes());

    textureEarthPlanet.reset( new QOpenGLTexture{ earth3DModel->getMainTexture().mirrored() });
    textureEarthPlanet->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureEarthPlanet->setMagnificationFilter(QOpenGLTexture::Linear);

    textureMoonPlanet.reset( new QOpenGLTexture{ moon3DModel->getMainTexture().mirrored() } );
    textureMoonPlanet->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureMoonPlanet->setMagnificationFilter(QOpenGLTexture::Linear);

    for (int i = 0; i < 6; i++)
    {
        textureSkybox[i].reset( new QOpenGLTexture{ skybox->getTextures()[i].mirrored() } );
        textureSkybox[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        textureSkybox[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }
}


void SunSystemScene::paint()
{
    projectionMatrix.setToIdentity();
    float aspectRatio = qreal(window()->width()) / window()->height();
    projectionMatrix.perspective(45.0f, aspectRatio, 0.1, 10000.0f);

    viewMatrixWithoutTranslate.setToIdentity();
    viewMatrixWithoutTranslate.rotate(cameraRotationAnglesXYZInDegrees[0], QVector3D{ 1, 0, 0 });
    viewMatrixWithoutTranslate.rotate(cameraRotationAnglesXYZInDegrees[1], QVector3D{ 0, 1, 0 });
   // viewMatrix.rotate(cameraRatationAnglesXYZ[2], QVector3D{ 0, 0, 1});
    viewMatrix = viewMatrixWithoutTranslate;
    viewMatrix.translate(-cameraPosition); // Относительно камеры все объекты имеют инвертированное положение.
    // Матричные преобразования идут в обратном вызовам функций порядке.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window()->width(), window()->height());

    glEnable(GL_CULL_FACE); // Включаем отсечение граней.
    glCullFace(GL_BACK); // Устанавливаем отсечение граней, обращенных к наблюдателю нелицевой стороной.

    shaderProgram->setUniformValue("light.position", QVector3D{ viewMatrix * QVector4D(6, 6, 16, 1)} );
    shaderProgram->setUniformValue("light.intensity", QVector3D(1.0f, 1.0f, 1.0f));

    QMatrix4x4 modelMatrix;
    modelMatrix.rotate(angleByEarthAxis, QVector3D{0, 1, 0});
    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;

    paintObject(vertexBufferForEarthPlanet, textureEarthPlanet.get(), modelViewMatrix);
    GLenum error = GL_NO_ERROR;
    do {
    error = glGetError();
    if (error != GL_NO_ERROR)
            qDebug() << "error occured number " << error;
    } while (error != GL_NO_ERROR);

    modelMatrix.setToIdentity();
    modelMatrix.rotate(angleByEarthAxis, QVector3D{0, 1, 0});
    modelMatrix.translate(QVector3D(20.0f, 0.0f, 0.0f));
    modelViewMatrix = viewMatrix * modelMatrix;
    paintObject(vertexBufferForMoonPlanet, textureMoonPlanet.get(), modelViewMatrix);

    paintSkybox();
}


void SunSystemScene::paintObject(QOpenGLBuffer &vertexBuffer, QOpenGLTexture *texture,
                                 const QMatrix4x4 &modelViewMatrix)
{
    shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    shaderProgram->setUniformValue("modelViewMatrix", modelViewMatrix);
    shaderProgram->setUniformValue("modelViewProjectionMatrix", projectionMatrix * modelViewMatrix);
    shaderProgram->setUniformValue("normalMatrix", modelViewMatrix.normalMatrix());

    LightInteractingMaterial mainMaterial = moon3DModel->getMainMaterial();
    shaderProgram->setUniformValue("mat.ka", mainMaterial.getKa());
    shaderProgram->setUniformValue("mat.kd", mainMaterial.getKd());
    shaderProgram->setUniformValue("mat.ks", mainMaterial.getKs());
    shaderProgram->setUniformValue("mat.shininess", mainMaterial.getShininess());

    vertexBuffer.bind();
    texture->bind();
    shaderProgram->setUniformValue("useTexture", true);
    shaderProgram->setUniformValue("useNormal", true);

    shaderProgram->enableAttributeArray("vertexPosition");
    shaderProgram->enableAttributeArray("vertexNormal");
    shaderProgram->enableAttributeArray("vertexTextureCoord");

    shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, Vertex::getPositionAttribOffset(), 3, sizeof(Vertex));
    shaderProgram->setAttributeBuffer("vertexNormal", GL_FLOAT, Vertex::getNormalAttribOffset(), 3, sizeof(Vertex));
    shaderProgram->setAttributeBuffer("vertexTextureCoord", GL_FLOAT, Vertex::getTextureCoordsAttribOffset(), 2, sizeof(Vertex));

    glDrawArrays(GL_TRIANGLES, 0, moon3DModel->getCountVertexes() );
}


void SunSystemScene::paintSkybox()
{
    shaderProgram->setUniformValue("modelViewProjectionMatrix", projectionMatrix * viewMatrixWithoutTranslate);

    vertexBufferForSkybox.bind();
    shaderProgram->enableAttributeArray("vertexPosition");
    shaderProgram->enableAttributeArray("vertexNormal");
    shaderProgram->enableAttributeArray("vertexTextureCoord");

    shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, Vertex::getPositionAttribOffset(), 3, sizeof(Vertex));
    shaderProgram->setAttributeBuffer("vertexNormal", GL_FLOAT, Vertex::getNormalAttribOffset(), 3, sizeof(Vertex));
    shaderProgram->setAttributeBuffer("vertexTextureCoord", GL_FLOAT, Vertex::getTextureCoordsAttribOffset(), 2, sizeof(Vertex));

    shaderProgram->setUniformValue("useTexture", true);
    shaderProgram->setUniformValue("useNormal", false);
    for(int i = 0; i < 6; i++)
    {
        textureSkybox[i]->bind();
        //glDrawArrays(GL_QUADS, i * 6, 6);
        glDrawArrays(GL_TRIANGLES, i * 6, 6);
    }
}


void SunSystemScene::cameraMoveForward()
{
    float zMove = -worldStep * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]));
    float xMove = worldStep * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]));
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveBack()
{
    float zMove = worldStep * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]));
    float xMove = -worldStep * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]));
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveLeft()
{
    float zMove = worldStep * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]) + M_PI_2);
    float xMove = -worldStep * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]) + M_PI_2);
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveRight()
{
    float zMove = -worldStep * qCos(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]) + M_PI_2);
    float xMove = worldStep * qSin(qDegreesToRadians(cameraRotationAnglesXYZInDegrees[1]) + M_PI_2);
    cameraMove(QVector3D( xMove, 0.0f, zMove));
}


void SunSystemScene::cameraMoveUp()
{
    float yMove = worldStep;
    cameraMove(QVector3D(0.0f, yMove, 0.0f));
}


void SunSystemScene::cameraMoveDown()
{
    float yMove = -worldStep;
    cameraMove(QVector3D(0.0f, yMove, 0.0f));
}


void SunSystemScene::cameraMove(const QVector3D deltaToMove)
{
    cameraPosition += deltaToMove;
}


void SunSystemScene::cameraRotateByXYZAxises(const QVector3D &xyzRotate)
{
    cameraRotationAnglesXYZInDegrees += xyzRotate;
}


void SunSystemScene::cameraRotateByXYZAxises(float xRotate, float yRotate, float zRotate)
{
    cameraRotateByXYZAxises(QVector3D{ xRotate, yRotate, zRotate });
}
