#include "graphic_scene.h"
#include "sst_window.h"


GraphicScene::~GraphicScene()
{
    // Makes current window context to correct deleting OpenGL resources.
    if (window() != nullptr)
        window()->makeCurrent();
}


void GraphicScene::initialize()
{
    initializeOpenGLFunctions();
    shaderProgram_.reset(new QOpenGLShaderProgram(context()));
}


void GraphicScene::setWindow(sst::Window *window)
{
    sceneWindow_ = window;
}


QOpenGLShaderProgram& GraphicScene::shaderProgram()
{
    return *shaderProgram_;
}


sst::Window* GraphicScene::window() const
{
    return sceneWindow_;
}


QOpenGLContext* GraphicScene::context()
{
    return sceneWindow_ ? sceneWindow_->context() : nullptr;
}


const QOpenGLContext* GraphicScene::context() const
{
    return sceneWindow_ ? sceneWindow_->context() : nullptr;
}
