#include "skybox.h"
#include "model_3dobject.h"

#include <QVector>
#include <QString>


const QVector<Vertex> tmpVertexes = {
    { {-1.0f,-1.0f,-1.0f }, {1.0f, 1.0f} },
    { { 1.0f,-1.0f,-1.0f }, {0.0f, 1.0f} },
    { { 1.0f, 1.0f,-1.0f }, {0.0f, 0.0f} },
    { {-1.0f, 1.0f,-1.0f }, {1.0f, 0.0f} },
    { {-1.0f,-1.0f, 1.0f }, {0.0f, 1.0f} },
    { {-1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
    { { 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f} },
    { { 1.0f,-1.0f, 1.0f }, {1.0f, 1.0f} },
    { {-1.0f, 1.0f, 1.0f }, {1.0f, 0.0f} },
    { {-1.0f,-1.0f, 1.0f }, {1.0f, 1.0f} },
    { {-1.0f,-1.0f,-1.0f }, {0.0f, 1.0f} },
    { {-1.0f, 1.0f,-1.0f }, {0.0f, 0.0f} },
    { { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
    { { 1.0f, 1.0f,-1.0f }, {1.0f, 0.0f} },
    { { 1.0f,-1.0f,-1.0f }, {1.0f, 1.0f} },
    { { 1.0f,-1.0f, 1.0f }, {0.0f, 1.0f} },
    { { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f} },
    { {-1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
    { {-1.0f, 1.0f,-1.0f }, {1.0f, 0.0f} },
    { { 1.0f, 1.0f,-1.0f }, {1.0f, 1.0f} },
    { { 1.0f,-1.0f, 1.0f }, {0.0f, 0.0f} },
    { { 1.0f,-1.0f,-1.0f }, {1.0f, 0.0f} },
    { {-1.0f,-1.0f,-1.0f }, {1.0f, 1.0f} },
    { {-1.0f,-1.0f, 1.0f }, {0.0f, 1.0f} }
};


Skybox::Skybox(QString pathToCubTextures)
    : vertexes(tmpVertexes)
{
    QVector<QString> textureFileNames = { "back.png", "front.png", "right.png"
                                        , "down.png", "left.png", "left.png" };

    for(auto textureName : textureFileNames)
    {
        textures.push_back(QImage(pathToCubTextures + textureName));
    }
}


int Skybox::getCountVertexes() const
{
    return positionCoordinates.size();
}


const float* Skybox::vertexData() const
{
    return reinterpret_cast<const float*>(vertexes.data());
}


const QVector<QImage> Skybox::getTextures() const
{
    return textures;
}
