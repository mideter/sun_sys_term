#include "skybox.h"
#include "model_3dobject.h"

#include <QVector>
#include <QString>


const QVector<Vertex> Skybox::vertexesForQuads = {
    // Back cube side
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
    { { 100.0f,-100.0f,-100.0f }, {0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
    { { 100.0f, 100.0f,-100.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f } },
    { {-100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f, 0.0f, 1.0f } },

    // Front cube side
    { {-100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f, 0.0f, -1.0f } },
    { {-100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f, -1.0f } },
    { { 100.0f, 100.0f, 100.0f }, {1.0f, 0.0f}, { 0.0f, 0.0f, -1.0f } },
    { { 100.0f,-100.0f, 100.0f }, {1.0f, 1.0f}, { 0.0f, 0.0f, -1.0f } },

    // Left cube side
    { {-100.0f, 100.0f, 100.0f }, {1.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f,-100.0f, 100.0f }, {1.0f, 1.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f,-100.0f,-100.0f }, {0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f, 100.0f,-100.0f }, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } },

    // Right cube side
    { { 100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f } },

    // Top cube side
    { { 100.0f, 100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f,-1.0f, 0.0f } },
    { {-100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f,-1.0f, 0.0f } },
    { {-100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f,-1.0f, 0.0f } },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f,-1.0f, 0.0f } },

    // Bottom cube side
    { { 100.0f,-100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f } },
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f, 1.0f, 0.0f } },
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f, 1.0f, 0.0f } },
    { {-100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f } }
};


const QVector<Vertex> Skybox::vertexes = {
    // Back cube side
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
    { { 100.0f,-100.0f,-100.0f }, {0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
    { { 100.0f, 100.0f,-100.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f } },

    { {-100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
    { { 100.0f, 100.0f,-100.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f } },
    { {-100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f, 0.0f, 1.0f } },

    // Front cube side
    { {-100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f, 0.0f, -1.0f } },
    { {-100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f, -1.0f } },
    { { 100.0f, 100.0f, 100.0f }, {1.0f, 0.0f}, { 0.0f, 0.0f, -1.0f } },

    { {-100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f, 0.0f, -1.0f } },
    { { 100.0f, 100.0f, 100.0f }, {1.0f, 0.0f}, { 0.0f, 0.0f, -1.0f } },
    { { 100.0f,-100.0f, 100.0f }, {1.0f, 1.0f}, { 0.0f, 0.0f, -1.0f } },

    // Left cube side
    { {-100.0f, 100.0f, 100.0f }, {1.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f,-100.0f, 100.0f }, {1.0f, 1.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f,-100.0f,-100.0f }, {0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f } },

    { {-100.0f, 100.0f, 100.0f }, {1.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f,-100.0f,-100.0f }, {0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f } },
    { {-100.0f, 100.0f,-100.0f }, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f } },

    // Right cube side
    { { 100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f } },

    { { 100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f } },
    { { 100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f } },

    // Top cube side
    { { 100.0f, 100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f,-1.0f, 0.0f } },
    { {-100.0f, 100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f,-1.0f, 0.0f } },
    { {-100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f,-1.0f, 0.0f } },

    { { 100.0f, 100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f,-1.0f, 0.0f } },
    { {-100.0f, 100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f,-1.0f, 0.0f } },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f,-1.0f, 0.0f } },

    // Bottom cube side
    { { 100.0f,-100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f } },
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 0.0f}, { 0.0f, 1.0f, 0.0f } },
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f, 1.0f, 0.0f } },

    { { 100.0f,-100.0f, 100.0f }, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f } },
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 1.0f}, { 0.0f, 1.0f, 0.0f } },
    { {-100.0f,-100.0f, 100.0f }, {0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f } }

};

Skybox::Skybox(QString pathToCubTextures)
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
    return vertexes.size();
}


const Vertex* Skybox::vertexData() const
{
    return vertexes.data();
}


const QVector<QImage> Skybox::getTextures() const
{
    return textures;
}