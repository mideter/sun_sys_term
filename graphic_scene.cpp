#include "graphic_scene.h"

#include <sst_window.h>


GraphicScene::~GraphicScene()
{
    // Makes current window context to correct deleting OpenGL resources.
    if (window() != nullptr)
        window()->makeCurrent();
}


void GraphicScene::initialize()
{
    initializeOpenGLFunctions();
    shaderProgram.reset(new QOpenGLShaderProgram(context()));
}


void GraphicScene::setWindow(sst::Window *window)
{
    sceneWindow = window;
}


QOpenGLShaderProgram& GraphicScene::getShaderProgram()
{
    return *shaderProgram;
}


sst::Window* GraphicScene::window() const
{
    return sceneWindow;
}


QOpenGLContext* GraphicScene::context()
{
    return sceneWindow ? sceneWindow->context() : nullptr;
}


const QOpenGLContext* GraphicScene::context() const
{
    return sceneWindow ? sceneWindow->context() : nullptr;
}
