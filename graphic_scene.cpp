#include "graphic_scene.h"

#include <QOpenGLWindow>


GraphicScene::GraphicScene(QOpenGLWindow *window)
    : openGlWindow(window)
{}


GraphicScene::~GraphicScene()
{
    // Makes current window context to correct deleting OpenGL resources.
    window()->makeCurrent();
}


void GraphicScene::initialize()
{
    initializeOpenGLFunctions();
    shaderProgram.reset(new QOpenGLShaderProgram(context()));
}


QOpenGLShaderProgram& GraphicScene::getShaderProgram()
{
    return *shaderProgram;
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
