#ifndef SST_OBJFILELOADERTO3DMODEL_H
#define SST_OBJFILELOADERTO3DMODEL_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QImage>

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
    QImage texture;

    bool isPlane;
};


class ObjFileReader
{
public:
    explicit ObjFileReader() = default;

    Model3DObject* load(QString filePath);

private:
    QVector<QString> readFile(QString filePath);
    void handleFile(QString filePath);
    void handleFileString(QString str);
    void readStringWithMaterialInfo(QString materialStr);
    void someProcessWithMaterial(const MaterialObjFile &material, Model3DObject *M, const uint matIndex);
    QVector<Face3v> getFacesForMaterial(const uint matIndex) const;

    Model3DObject *createModel3DObject();

    QString directoryPath_;

    uint currentMaterial_;

    QVector<FaceObjFile> faces_;
    QVector<MaterialObjFile> materials_;

    QVector<QVector3D> vertexPositions_;
    QVector<QVector2D> vertexTexturePositions_;
    QVector<QVector3D> vertexNormals_;

    QVector<QImage> texturePixmaps_;
    QVector<QString> textureNames_;
};


#endif // SST_OBJFILELOADERTO3DMODEL_H
