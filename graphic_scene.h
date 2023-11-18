#ifndef SST_GRAPHICSCENE_H
#define SST_GRAPHICSCENE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <memory>


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
    QOpenGLShaderProgram& shaderProgram();

    virtual void initialize() = 0;
    virtual void paint() = 0;

protected:
    std::unique_ptr<QOpenGLShaderProgram> shaderProgram_ = nullptr;

private:
    sst::Window* sceneWindow_ = nullptr;
};


#endif // SST_GRAPHICSCENE_H
