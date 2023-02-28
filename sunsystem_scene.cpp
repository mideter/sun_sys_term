#include "sunsystem_scene.h"

#include <QOpenGLWindow>

SunSystemScene::SunSystemScene(QOpenGLWindow *window)
    : GraphicScene(window)
{
    initializeObjectData();
}

void SunSystemScene::initializeObjectData()
{
    scenePoints = {
        // BACK FACE
        {{-0.5, -0.5, -0.5}, {0.0, 0.0, -1.0}}, {{-0.5,  0.5, -0.5}, {0.0, 0.0, -1.0}},
        {{ 0.5,  0.5, -0.5}, {0.0, 0.0, -1.0}}, {{ 0.5, -0.5, -0.5}, {0.0, 0.0, -1.0}},

        // FRONT FACE
        {{-0.5, -0.5, 0.5}, {0.0, 0.0,  1.0}}, {{ 0.5, -0.5, 0.5}, {0.0, 0.0,  1.0}},
        {{ 0.5,  0.5, 0.5}, {0.0, 0.0,  1.0}}, {{-0.5,  0.5, 0.5}, {0.0, 0.0,  1.0}},

        // BOTTOM FACE
        {{-0.5, -0.5,  0.5}, { 0.0, -1.0,  0.0}}, {{-0.5, -0.5, -0.5}, { 0.0, -1.0,  0.0}},
        {{ 0.5, -0.5, -0.5}, { 0.0, -1.0,  0.0}}, {{ 0.5, -0.5,  0.5}, { 0.0, -1.0,  0.0}},

        // TOP FACE
        {{-0.5,  0.5,  0.5}, { 0.0,  1.0,  0.0}}, {{ 0.5,  0.5,  0.5}, { 0.0,  1.0,  0.0}},
        {{ 0.5,  0.5, -0.5}, { 0.0,  1.0,  0.0}}, {{-0.5,  0.5, -0.5}, { 0.0,  1.0,  0.0}},

        // RIGHT FACE
        {{ 0.5,  0.5, -0.5}, { 1.0,  0.0,  0.0}}, {{ 0.5,  0.5,  0.5}, { 1.0,  0.0,  0.0}},
        {{ 0.5, -0.5,  0.5}, { 1.0,  0.0,  0.0}}, {{ 0.5, -0.5, -0.5}, { 1.0,  0.0,  0.0}},

        // LEFT FACE
        {{-0.5,  0.5, -0.5}, {-1.0,  0.0,  0.0}}, {{-0.5, -0.5, -0.5}, {-1.0,  0.0,  0.0}},
        {{-0.5, -0.5,  0.5}, {-1.0,  0.0,  0.0}}, {{-0.5,  0.5,  0.5}, {-1.0,  0.0,  0.0}}
    };
}

void SunSystemScene::initialize()
{
    GraphicScene::initialize();
    glClearColor(0, 0, 0, 0);

    openGlShaderProgram.reset(new QOpenGLShaderProgram(context()));
    openGlShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/phong.vert");
    openGlShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":phong.frag");
    openGlShaderProgram->link();

    openGlShaderProgram->bind();
    openGlShaderProgram->setAttributeArray("Vertex", GL_FLOAT, &scenePoints[0].coords, 3, sizeof(ScenePoint));
    openGlShaderProgram->enableAttributeArray("Vertex");

    openGlShaderProgram->setAttributeArray("Normal", GL_FLOAT, &scenePoints[0].normal, 3, sizeof(ScenePoint));
    openGlShaderProgram->enableAttributeArray("Normal");

    openGlShaderProgram->setUniformValue("mat.ka", QVector3D(0.1, 0.0, 0.0));
    openGlShaderProgram->setUniformValue("mat.kd", QVector3D(0.7, 0.0, 0.0));
    openGlShaderProgram->setUniformValue("mat.ks", QVector3D(1.0, 1.0, 1.0));
    openGlShaderProgram->setUniformValue("mat.shininess", 128.0f);

    openGlShaderProgram->setUniformValue("light.position", QVector3D(2, 1, 1));
    openGlShaderProgram->setUniformValue("light.intensity", QVector3D(1, 1, 1));
}

void SunSystemScene::paint()
{
    projectionMatrix.setToIdentity();
    float aspectRatio = qreal(window()->width()) / window()->height();
    projectionMatrix.perspective(90, aspectRatio, 0.5, 40);

    viewMatrix.setToIdentity();
    QVector3D eye(0, 0, 2);
    QVector3D center(0, 0, 0);
    QVector3D up(0, 1, 0);
    viewMatrix.lookAt(eye, center, up);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window()->width(), window()->height());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    modelMatrix.setToIdentity();
 // modelMatrix.rotate(angle(), 0, 1, 0);
 // modelMatrix.rotate(angle(), 0, 0, 1);
    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;
    paintObject(modelViewMatrix);
}

void SunSystemScene::paintObject(const QMatrix4x4 &mvMatrix)
{
    openGlShaderProgram->bind();
    openGlShaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    openGlShaderProgram->setUniformValue("modelViewMatrix", mvMatrix);
    openGlShaderProgram->setUniformValue("mvpMatrix", projectionMatrix * mvMatrix);
    openGlShaderProgram->setUniformValue("normalMatrix", mvMatrix.normalMatrix());

    glDrawArrays(GL_QUADS, 0, scenePoints.size());
}
