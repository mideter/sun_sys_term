#ifndef SKYBOX_H
#define SKYBOX_H

#include "model_3dobject.h"

#include <QVector>
#include <QImage>


class Skybox
{
private:
    QVector<Vertex> vertexes;
    QVector<float> textCoordinates;
    QVector<float> positionCoordinates;

    QVector<QImage> textures;

public:
    Skybox(QString pathToCubTextures);
    ~Skybox() = default;

    int getCountVertexes() const;
    const float* vertexData() const;

    const QVector<QImage> getTextures() const;
};


#endif // SKYBOX_H
