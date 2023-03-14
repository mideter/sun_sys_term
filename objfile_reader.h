#ifndef OBJFILELOADERTO3DMODEL_H
#define OBJFILELOADERTO3DMODEL_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "model_3dobject.h"


struct VertexObjFile
{
    // Каждая вершина определяется идентификаторами координат в пространстве,
    // координатами текстуры и вектором нормали
    int positionId;
    int texturePositionId;
    int normalId;
};


// Грань
struct FaceObjFile
{
    // Грань определяется тремя вершинами.
    QVector<VertexObjFile> vertexes;
    uint materialId;
};


struct MaterialObjFile
{
    QString name;

    float alpha;
    float ns;
    float ni;

    QVector3D di;
    QVector3D am;
    QVector3D sp;
    QVector3D em;

    int illum;
    GLuint texture;

    bool isPlane;
};


class ObjFileReader
{
    QString directoryPath;

    uint currentMaterial;

    QVector<FaceObjFile> faces;
    QVector<MaterialObjFile> materials;

    QVector<QVector3D> vertexPositions;
    QVector<QVector2D> vertexTexturePositions;
    QVector<QVector3D> vertexNormals;

    QVector<GLuint> textureIds;
    QVector<QString> textureNames;

    QVector<QString> readFile(QString filePath);
    void handleFile(QString filePath);
    void handleFileString(std::string str);
    void readStringWithMaterialInfo(QString materialStr);
    void someProcessWithMaterial(const MaterialObjFile &material, Model3DObject *M, const uint matIndex);

    Model3DObject *createModel3DObject();

public:
    explicit ObjFileReader() = default;

    Model3DObject* load(QString filePath);
};

#endif // OBJFILELOADERTO3DMODEL_H
