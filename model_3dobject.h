#ifndef SST_MODEL3DOBJECT_H
#define SST_MODEL3DOBJECT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QtGui/qopengl.h>


class QOpenGLShaderProgram;


class LightInteractingMaterial
{
public:
    LightInteractingMaterial() = default;
    LightInteractingMaterial(const QVector3D &Ka, const QVector3D &Kd,
                             const QVector3D &Ks, const QVector3D &Ke, float shininess);
    ~LightInteractingMaterial() = default;

    QVector3D ka() const;
    QVector3D kd() const;
    QVector3D ks() const;
    QVector3D ke() const;
    float shininess() const;

private:
    QVector3D ka_;
    QVector3D kd_;
    QVector3D ks_;
    QVector3D ke_;
    float shininess_;
};


class Vertex
{
public:
    Vertex() = default;
    Vertex(const QVector3D &position, const QVector2D &texturePosition, const QVector3D &normal = QVector3D());
    ~Vertex() = default;

    static int positionAttribOffset();
    static int textureCoordsAttribOffset();
    static int normalAttribOffset();

private:
    QVector3D position_;
    QVector2D texturePosition_;
    QVector3D normal_;
};


class Face3v
{
public:
    Face3v() = default;
    // Вершины должны передаваться в порядке обхода против часовой стрелки.
    explicit Face3v(const QVector<Vertex> &vertexes);
    // Вершины должны передаваться в порядке обхода против часовой стрелки.
    Face3v(const Vertex &v1, const Vertex &v2, const Vertex &v3);
    ~Face3v() = default;

    static int countVertexes();

    Vertex* vertexData();

private:
    enum{ countVertexesInFace_ = 3 };
    Vertex v1, v2, v3; // Vertexes is numbered in counterclockwise order.
};


class SurfaceWithOneMaterial
{
public:
    SurfaceWithOneMaterial() = default;
    explicit SurfaceWithOneMaterial(const LightInteractingMaterial &material);
    SurfaceWithOneMaterial(const LightInteractingMaterial &material, const QVector<Face3v> &faces, const QImage &texture = QImage());
    ~SurfaceWithOneMaterial() = default;

    void setMaterial(const LightInteractingMaterial &material);
    void setTexture(const QImage &texture);
    void addFace(const Face3v &face);

    int countVertexes() const;
    LightInteractingMaterial material() const;
    QImage texture() const;

    Vertex* vertexData();

private:
    LightInteractingMaterial material_;
    QVector<Face3v> faces_;
    QImage texture_;
};


class Model3DObject
{
public:
    void addSurface(const SurfaceWithOneMaterial &surface);

    std::size_t countVertexes() const;
    LightInteractingMaterial mainMaterial() const;
    QImage mainTexture() const;

    Vertex* vertexData();

private:
    QVector<SurfaceWithOneMaterial> surfaces_;
};


#endif // SST_MODEL3DOBJECT_H
