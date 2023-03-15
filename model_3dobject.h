#ifndef MODEL3DOBJECT_H
#define MODEL3DOBJECT_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QtGui/qopengl.h>

class QOpenGLShaderProgram;


class LightInteractingMaterial
{
private:
    QVector3D Ka;
    QVector3D Kd;
    QVector3D Ks;
    QVector3D Ke;
    float shininess;

public:
    LightInteractingMaterial() = default;
    LightInteractingMaterial(const QVector3D &Ka, const QVector3D &Kd,
                             const QVector3D &Ks, const QVector3D &Ke, float shininess);
    ~LightInteractingMaterial() = default;

    QVector3D getKa() const;
    QVector3D getKd() const;
    QVector3D getKs() const;
    QVector3D getKe() const;
    float getShininess() const;
};


class Vertex
{
private:
    QVector3D position;
    QVector2D texturePosition;
    QVector3D normal;

public:
    Vertex() = default;
    Vertex(const QVector3D &position, const QVector2D &texturePosition, const QVector3D &normal);
    ~Vertex() = default;

    static int getPositionAttribOffset();
    static int getTexturePositionAttribOffset();
    static int getNormalAttribOffset();
};


class Face3v
{
private:
    enum{ countVertexesInFace = 3 };
    Vertex v1, v2, v3; // Vertexes is numbered in counterclockwise order.

public:
    Face3v() = default;
    // Вершины должны передаваться в порядке обхода против часовой стрелки.
    explicit Face3v(const QVector<Vertex> &vertexes);
    // Вершины должны передаваться в порядке обхода против часовой стрелки.
    Face3v(const Vertex &v1, const Vertex &v2, const Vertex &v3);
    ~Face3v() = default;

    static int getCountVertexes();

    Vertex* vertexData();
};


class SurfaceWithOneMaterial
{
private:
    LightInteractingMaterial material;
    QVector<Face3v> faces;
    QImage texture;

public:
    SurfaceWithOneMaterial() = default;
    explicit SurfaceWithOneMaterial(const LightInteractingMaterial &material);
    SurfaceWithOneMaterial(const LightInteractingMaterial &material, const QVector<Face3v> &faces, const QImage &texture = QImage());
    ~SurfaceWithOneMaterial() = default;

    void setMaterial(const LightInteractingMaterial &material);
    void setTexture(const QImage &texture);
    void addFace(const Face3v &face);

    int getCountVertexes() const;
    LightInteractingMaterial getMaterial() const;
    QImage getTexture() const;

    Vertex* vertexData();
};


class Model3DObject
{
private:
    QVector<SurfaceWithOneMaterial> surfaces;

public:
    void addSurface(const SurfaceWithOneMaterial &surface);

    std::size_t getCountVertexes() const;
    LightInteractingMaterial getMainMaterial() const;
    QImage getMainTexture() const;

    Vertex* vertexData();
};

#endif // MODEL3DOBJECT_H
