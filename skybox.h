#ifndef SKYBOX_H
#define SKYBOX_H

#include "model_3dobject.h"

#include <QVector>
#include <QImage>


class Skybox
{
private:
    static const QVector<Vertex> vertexesForQuads;
    static const QVector<Vertex> vertexes;
    QVector<QImage> textures;

public:
    Skybox(QString pathToCubTextures);
    ~Skybox() = default;

    int getCountVertexes() const;
    const Vertex* vertexData() const;

    const QVector<QImage> getTextures() const;
};


#endif // SKYBOX_H
