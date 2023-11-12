#ifndef SST_SKYBOX_H
#define SST_SKYBOX_H

#include "model_3dobject.h"

#include <QVector>
#include <QImage>


class Skybox
{
public:
    Skybox(QString pathToCubTextures);
    ~Skybox() = default;

    int getCountVertexes() const;
    const Vertex* vertexData() const;

    const QVector<QImage> getTextures() const;

private:
    static const QVector<Vertex> vertexesForQuads;
    static const QVector<Vertex> vertexes;
    QVector<QImage> textures;
};


#endif // SST_SKYBOX_H
