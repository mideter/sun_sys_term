#include "graphic_scene.h"

#include <QOpenGLWindow>

GraphicScene::GraphicScene(QOpenGLWindow *window)
    : openGlWindow(window)
{}

GraphicScene::~GraphicScene()
{}

void GraphicScene::initialize()
{
    initializeOpenGLFunctions();
    openGlShaderProgram.reset(new QOpenGLShaderProgram(context()));
}

QOpenGLShaderProgram& GraphicScene::shaderProgram()
{
    return *openGlShaderProgram;
}

QOpenGLWindow* GraphicScene::window() const
{
    return openGlWindow;
}

QOpenGLContext* GraphicScene::context()
{
    return openGlWindow ? openGlWindow->context() : nullptr;
}

const QOpenGLContext* GraphicScene::context() const
{
    return openGlWindow ? openGlWindow->context() : nullptr;
}
