#include "model_3dobject.h"
#include "sst_exeption.h"

#include <QOpenGLShaderProgram>


void Model3DObject::addSurface(const SurfaceWithOneMaterial &surface)
{
    surfaces.push_back(surface);
}

std::size_t Model3DObject::getCountVertexes() const
{
    return surfaces[0].getCountVertexes();
}


LightInteractingMaterial Model3DObject::getMainMaterial() const
{
    return surfaces[0].getMaterial();
}


QImage Model3DObject::getMainTexture() const
{
    return surfaces[0].getTexture();
}


Vertex* Model3DObject::vertexData()
{
    return surfaces[0].vertexData();
}


SurfaceWithOneMaterial::SurfaceWithOneMaterial(const LightInteractingMaterial &material)
    : material(material)
{}


SurfaceWithOneMaterial::SurfaceWithOneMaterial(const LightInteractingMaterial &material, const QVector<Face3v> &faces,
                                               const QImage &texture)
    : material(material)
    , faces(faces)
    , texture(texture)
{
    // TODO сделать проверку инварианта.
}


void SurfaceWithOneMaterial::setMaterial(const LightInteractingMaterial &material)
{
    this->material = material;
}


void SurfaceWithOneMaterial::setTexture(const QImage &texture)
{
    this->texture = texture;
}


void SurfaceWithOneMaterial::addFace(const Face3v &face)
{
    faces.push_back(face);
}


int SurfaceWithOneMaterial::getCountVertexes() const
{
    return faces.size() * Face3v::getCountVertexes();
}


LightInteractingMaterial SurfaceWithOneMaterial::getMaterial() const
{
    return material;
}


QImage SurfaceWithOneMaterial::getTexture() const
{
    return texture;
}


Vertex* SurfaceWithOneMaterial::vertexData()
{
    return faces[0].vertexData();
}


LightInteractingMaterial::LightInteractingMaterial(const QVector3D &Ka, const QVector3D &Kd,
                             const QVector3D &Ks, const QVector3D &Ke, float shininess)
    : Ka(Ka), Kd(Kd), Ks(Ks), Ke(Ke), shininess(shininess)
{
    // TODO: сделать проверку инвариантов класса.
}


QVector3D LightInteractingMaterial::getKa() const
{
    return Ka;
}


QVector3D LightInteractingMaterial::getKd() const
{
    return Kd;
}


QVector3D LightInteractingMaterial::getKs() const
{
    return Ks;
}


QVector3D LightInteractingMaterial::getKe() const
{
    return Ke;
}


float LightInteractingMaterial::getShininess() const
{
    return shininess;
}

// Вершины должны передаваться в порядке обхода против часовой стрелки.
Face3v::Face3v(const QVector<Vertex> &vertexes)
{
    if (vertexes.size() != countVertexesInFace)
        throw SstExeption("Error: face can't include more or less than 3 vertexes.");
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


int Face3v::getCountVertexes()
{
    return countVertexesInFace;
}


Vertex* Face3v::vertexData()
{
    return &v1;
}


Vertex::Vertex(const QVector3D &position, const QVector2D &texturePosition, const QVector3D &normal)
    : position(position)
    , texturePosition(texturePosition)
    , normal(normal)
{
    // TODO: сделать проверку инвариантов класса.
}


int Vertex::getPositionAttribOffset()
{
    Vertex tmp;
    return reinterpret_cast<long>(&tmp.position) - reinterpret_cast<long>(&tmp);
}


int Vertex::getTextureCoordsAttribOffset()
{
    Vertex tmp;
    return reinterpret_cast<long>(&tmp.texturePosition) - reinterpret_cast<long>(&tmp);
}


int Vertex::getNormalAttribOffset()
{
    Vertex tmp;
    return reinterpret_cast<long>(&tmp.normal) - reinterpret_cast<long>(&tmp);
}

