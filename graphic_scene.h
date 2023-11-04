#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <memory>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


namespace sst {
class Window;
}


class GraphicScene
    : public QObject
    , public QOpenGLFunctions
{
    Q_OBJECT

public:
    GraphicScene() = default;
    virtual ~GraphicScene() = 0;

    void setWindow(sst::Window* window);
    sst::Window* window() const;
    QOpenGLContext* context();
    const QOpenGLContext* context() const;
    QOpenGLShaderProgram& getShaderProgram();

    virtual void initialize() = 0;
    virtual void paint() = 0;

protected:
    std::unique_ptr<QOpenGLShaderProgram> shaderProgram = nullptr;

private:
    sst::Window* sceneWindow = nullptr;
};

#endif // GRAPHICSCENE_H
