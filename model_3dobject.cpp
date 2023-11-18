#include "model_3dobject.h"
#include "sst_exception.h"

#include <QOpenGLShaderProgram>


void Model3DObject::addSurface(const SurfaceWithOneMaterial &surface)
{
    surfaces_.push_back(surface);
}


std::size_t Model3DObject::countVertexes() const
{
    return surfaces_[0].countVertexes();
}


LightInteractingMaterial Model3DObject::mainMaterial() const
{
    return surfaces_[0].material();
}


QImage Model3DObject::mainTexture() const
{
    return surfaces_[0].texture();
}


Vertex* Model3DObject::vertexData()
{
    return surfaces_[0].vertexData();
}


SurfaceWithOneMaterial::SurfaceWithOneMaterial(const LightInteractingMaterial &material)
    : material_(material)
{}


SurfaceWithOneMaterial::SurfaceWithOneMaterial(const LightInteractingMaterial &material, const QVector<Face3v> &faces,
                                               const QImage &texture)
    : material_(material)
    , faces_(faces)
    , texture_(texture)
{
    // TODO сделать проверку инварианта.
}


void SurfaceWithOneMaterial::setMaterial(const LightInteractingMaterial &material)
{
    this->material_ = material;
}


void SurfaceWithOneMaterial::setTexture(const QImage &texture)
{
    this->texture_ = texture;
}


void SurfaceWithOneMaterial::addFace(const Face3v &face)
{
    faces_.push_back(face);
}


int SurfaceWithOneMaterial::countVertexes() const
{
    return faces_.size() * Face3v::countVertexes();
}


LightInteractingMaterial SurfaceWithOneMaterial::material() const
{
    return material_;
}


QImage SurfaceWithOneMaterial::texture() const
{
    return texture_;
}


Vertex* SurfaceWithOneMaterial::vertexData()
{
    return faces_[0].vertexData();
}


LightInteractingMaterial::LightInteractingMaterial(const QVector3D &Ka, const QVector3D &Kd,
                             const QVector3D &Ks, const QVector3D &Ke, float shininess)
    : ka_(Ka),kd_(Kd),ks_(Ks), ke_(Ke), shininess_(shininess)
{
    // TODO: сделать проверку инвариантов класса.
}


QVector3D LightInteractingMaterial::ka() const
{
    return ka_;
}


QVector3D LightInteractingMaterial::kd() const
{
    return kd_;
}


QVector3D LightInteractingMaterial::ks() const
{
    return ks_;
}


QVector3D LightInteractingMaterial::ke() const
{
    return ke_;
}


float LightInteractingMaterial::shininess() const
{
    return shininess_;
}

// Вершины должны передаваться в порядке обхода против часовой стрелки.
Face3v::Face3v(const QVector<Vertex> &vertexes)
{
    if (vertexes.size() != countVertexesInFace_)
        throw SstException("Error: face can't include more or less than 3 vertexes.");
    // TODO: сделать дополнительные проверки инвариантов класса.

    v1 = vertexes[0];
    v2 = vertexes[1];
    v3 = vertexes[2];
}


// Вершины должны передаваться в порядке обхода против часовой стрелки.
Face3v::Face3v(const Vertex &v1, const Vertex &v2, const Vertex &v3)
    : v1(v1), v2(v2), v3(v3)
{
    // TODO: сделать проверку инвариантов класса.
}


int Face3v::countVertexes()
{
    return countVertexesInFace_;
}


Vertex* Face3v::vertexData()
{
    return &v1;
}


Vertex::Vertex(const QVector3D &position, const QVector2D &texturePosition, const QVector3D &normal)
    : position_(position)
    , texturePosition_(texturePosition)
    , normal_(normal)
{
    // TODO: сделать проверку инвариантов класса.
}


int Vertex::positionAttribOffset()
{
    Vertex tmp;
    return reinterpret_cast<long>(&tmp.position_) - reinterpret_cast<long>(&tmp);
}


int Vertex::textureCoordsAttribOffset()
{
    Vertex tmp;
    return reinterpret_cast<long>(&tmp.texturePosition_) - reinterpret_cast<long>(&tmp);
}


int Vertex::normalAttribOffset()
{
    Vertex tmp;
    return reinterpret_cast<long>(&tmp.normal_) - reinterpret_cast<long>(&tmp);
}

