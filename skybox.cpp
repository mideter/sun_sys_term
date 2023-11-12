#include "skybox.h"

#include <QVector>
#include <QString>


const QVector<Vertex> Skybox::vertexes = {
    // Back cube side
    { {-100.0f,-100.0f,-100.0f }, {0.0f, 0.0f} },
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 0.0f} },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 1.0f} },

    { {-100.0f,-100.0f,-100.0f }, {0.0f, 0.0f} },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 1.0f} },
    { {-100.0f, 100.0f,-100.0f }, {0.0f, 1.0f} },

    // Front cube side
    { {-100.0f,-100.0f, 100.0f }, {1.0f, 0.0f} }, // Текстурный x равен большему вертексному x.
    { {-100.0f, 100.0f, 100.0f }, {1.0f, 1.0f} },
    { { 100.0f, 100.0f, 100.0f }, {0.0f, 1.0f} },

    { {-100.0f,-100.0f, 100.0f }, {1.0f, 0.0f} },
    { { 100.0f, 100.0f, 100.0f }, {0.0f, 1.0f} },
    { { 100.0f,-100.0f, 100.0f }, {0.0f, 0.0f} },

    // Left cube side
    { {-100.0f, 100.0f, 100.0f }, {0.0f, 1.0f} }, // texture x=0 должен быть при большей Z координате
    { {-100.0f,-100.0f, 100.0f }, {0.0f, 0.0f} }, // Текстурный y соответствует y положения, а текстурная координата x - z положению
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 0.0f} },

    { {-100.0f, 100.0f, 100.0f }, {0.0f, 1.0f} },
    { {-100.0f,-100.0f,-100.0f }, {1.0f, 0.0f} },
    { {-100.0f, 100.0f,-100.0f }, {1.0f, 1.0f} },

    // Right cube side
    { { 100.0f, 100.0f, 100.0f }, {1.0f, 1.0f} }, // Координата текстуры x=0 при меньшем Z положения
    { { 100.0f, 100.0f,-100.0f }, {0.0f, 1.0f} },
    { { 100.0f,-100.0f,-100.0f }, {0.0f, 0.0f} },

    { { 100.0f, 100.0f, 100.0f }, {1.0f, 1.0f} },
    { { 100.0f,-100.0f,-100.0f }, {0.0f, 0.0f} },
    { { 100.0f,-100.0f, 100.0f }, {1.0f, 0.0f} },

    // Top cube side
    { { 100.0f, 100.0f, 100.0f }, {1.0f, 1.0f} },
    { {-100.0f, 100.0f, 100.0f }, {0.0f, 1.0f} },
    { {-100.0f, 100.0f,-100.0f }, {0.0f, 0.0f} },

    { { 100.0f, 100.0f, 100.0f }, {1.0f, 1.0f} },
    { {-100.0f, 100.0f,-100.0f }, {0.0f, 0.0f} },
    { { 100.0f, 100.0f,-100.0f }, {1.0f, 0.0f} },

    // Bottom cube side
    { { 100.0f,-100.0f, 100.0f }, {1.0f, 0.0f} }, // Текстурынй y=0 соответствует большему z положению
    { { 100.0f,-100.0f,-100.0f }, {1.0f, 1.0f} },
    { {-100.0f,-100.0f,-100.0f }, {0.0f, 1.0f} },

    { { 100.0f,-100.0f, 100.0f }, {1.0f, 0.0f} },
    { {-100.0f,-100.0f,-100.0f }, {0.0f, 1.0f} },
    { {-100.0f,-100.0f, 100.0f }, {0.0f, 0.0f} }
};


Skybox::Skybox(QString pathToCubTextures)
{
    QVector<QString> textureFileNames = { "back.png", "front.png", "left.png"
                                        , "right.png", "up.png", "down.png" };

    for(auto textureName : textureFileNames) {
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
