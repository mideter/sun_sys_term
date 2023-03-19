#include "sunsystem_scene.h"
#include "objfile_reader.h"

#include <QOpenGLWindow>


SunSystemScene::SunSystemScene(QOpenGLWindow *window)
    : GraphicScene(window)
    , vertexBuffer(QOpenGLBuffer::VertexBuffer)
    , rotationByEarthAxis(this, "angleByEarthAxis")
{
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    initializeObjectData();

    rotationByEarthAxis.setStartValue(0);
    rotationByEarthAxis.setEndValue(359);
    rotationByEarthAxis.setDuration(360000);
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
    ObjFileReader objFileReader;
    earth3DModel.reset(objFileReader.load("data/Earth/Earth.obj"));
}

void SunSystemScene::initialize()
{
    GraphicScene::initialize();
    glClearColor(0, 0, 0, 0);

    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":vertex.shader");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":fragment.shader");
    shaderProgram->link();

    shaderProgram->bind();

    shaderProgram->setUniformValue("light.position", QVector3D(0.1, 0.1, 0.1));
    shaderProgram->setUniformValue("light.intensity", QVector3D(1.0f, 1.0f, 1.0f));

    vertexBuffer.create();
    vertexBuffer.bind(); // bind() must be called before allocate()
    vertexBuffer.allocate(earth3DModel->vertexData(), sizeof(Vertex) * earth3DModel->getCountVertexes());

    shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, Vertex::getPositionAttribOffset(), 3, sizeof(Vertex));
    shaderProgram->enableAttributeArray("vertexPosition");

    shaderProgram->setAttributeBuffer("vertexNormal", GL_FLOAT, Vertex::getNormalAttribOffset(), 3, sizeof(Vertex));
    shaderProgram->enableAttributeArray("vertexNormal");

    shaderProgram->setAttributeBuffer("vertexTextureCoord", GL_FLOAT, Vertex::getTexturePositionAttribOffset(), 2, sizeof(Vertex));
    shaderProgram->enableAttributeArray("vertexTextureCoord");

    texture.reset( new QOpenGLTexture{ earth3DModel->getMainTexture().mirrored() } );
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->bind();
}


void SunSystemScene::paint()
{
    projectionMatrix.setToIdentity();
    float aspectRatio = qreal(window()->width()) / window()->height();
    projectionMatrix.perspective(90, aspectRatio, 0.5, 40);

    viewMatrix.setToIdentity();
    QVector3D eye(0, 0, 10);
    QVector3D center(0, 0, 0);
    QVector3D up(0, 1, 0);
    viewMatrix.lookAt(eye, center, up);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window()->width(), window()->height());
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE); // Включаем отсечение граней.
    glCullFace(GL_BACK); // Устанавливаем отсечение граней, обращенных к наблюдателю нелицевой стороной.

    modelMatrix.setToIdentity();
    modelMatrix.rotate(angleByEarthAxis, QVector3D{0, 1, 0});
    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;
    paintObject(modelViewMatrix);
}


void SunSystemScene::paintObject(const QMatrix4x4 &mvMatrix)
{
    shaderProgram->bind();

    shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    shaderProgram->setUniformValue("modelViewMatrix", mvMatrix);
    shaderProgram->setUniformValue("mvpMatrix", projectionMatrix * mvMatrix);
    shaderProgram->setUniformValue("normalMatrix", mvMatrix.normalMatrix());

    LightInteractingMaterial mainMaterial = earth3DModel->getMainMaterial();
    shaderProgram->setUniformValue("mat.ka", mainMaterial.getKa());
    shaderProgram->setUniformValue("mat.kd", mainMaterial.getKd());
    shaderProgram->setUniformValue("mat.ks", mainMaterial.getKs());
    shaderProgram->setUniformValue("mat.shininess", mainMaterial.getShininess());

    glDrawArrays(GL_TRIANGLES, 0, earth3DModel->getCountVertexes());
}
