#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <memory>
#include <QObject>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>

class QOpenGLWindow;

class GraphicScene : public QObject,
                     public QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    explicit GraphicScene(QOpenGLWindow * window = nullptr);
    virtual ~GraphicScene() = 0;

    QOpenGLWindow* window() const;
    QOpenGLContext* context();
    const QOpenGLContext* context() const;
    QOpenGLShaderProgram& shaderProgram();

    virtual void initialize() = 0;
    virtual void paint() = 0;

protected:
    std::unique_ptr<QOpenGLShaderProgram> openGlShaderProgram = nullptr;

private:
    QOpenGLWindow *openGlWindow = nullptr;
};

#endif // GRAPHICSCENE_H
