#include "objfile_reader.h"
#include "sst_exception.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QPixmap>
#include <QDebug>

#include "sst_exception.h"


// Возвращает директория файла с символом / в конце.
QString getFileDirectoryFromFilePath(QString filePath)
{
    QRegularExpression regExp(R"***((.*/)[a-zA-Z0-9]+\.obj$)***");
    QRegularExpressionMatch match = regExp.match(filePath);

    if (match.hasMatch())
        return match.captured(1);

    QString errorMsg = "Error: can't to identify directory path.";
    throw SstException(errorMsg);
}


void ObjFileReader::readStringWithMaterialInfo(QString materialStr)
{
    QString mtlName = materialStr.section("mtllib ", 0, 0, QString::SectionSkipEmpty);
    QString materialFilePath = directoryPath_ + mtlName;

    QFile materialFile(materialFilePath);
    if(!materialFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString errorMsg = QString("Error: can't open %1.").arg(materialFilePath);
        throw SstException(errorMsg);
    }

    std::vector<QString> tmp;
    QTextStream in(&materialFile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        tmp.push_back(line);
    }

    QString materialName;
    QImage texturePix;

    QVector3D amb;
    QVector3D dif;
    QVector3D spec;
    QVector3D emis;

    float alpha, ns, ni;
    int illum;

    bool isPlane;
    bool ismat = false;

    for(uint i = 0; i < tmp.size(); i++)
    {
        if(materialStr.size() == 0 || tmp[i].startsWith("#"))
            continue;

        if(tmp[i].startsWith("newmtl "))
        {
            if (ismat)
                materials_.push_back(MaterialObjFile{ materialName, alpha, ns, ni, dif, amb, spec, emis, illum, texturePix, isPlane });

            ismat = false;
            isPlane = false;
            materialName = tmp[i].section("newmtl ", 0, 0, QString::SectionSkipEmpty);
        }
        else if(tmp[i].startsWith("Ns "))
            sscanf(tmp[i].toStdString().c_str(),"Ns %f", &ns);
        else if(tmp[i].startsWith("Ka "))
            sscanf(tmp[i].toStdString().c_str(),"Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
        else if(tmp[i].startsWith("Kd "))
            sscanf(tmp[i].toStdString().c_str(),"Kd %f %f %f", &dif[0], &dif[1], &dif[2]);
        else if(tmp[i].startsWith("Ks "))
            sscanf(tmp[i].toStdString().c_str(),"Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
        else if(tmp[i].startsWith("Ke "))
            sscanf(tmp[i].toStdString().c_str(),"Ke %f %f %f", &emis[0], &emis[1], &emis[2]);
        else if(tmp[i].startsWith("Ni "))
            sscanf(tmp[i].toStdString().c_str(),"Ni %f", &ni);
        else if(tmp[i].startsWith("d "))
            sscanf(tmp[i].toStdString().c_str(),"d %f", &alpha);
        else if(tmp[i].startsWith("illum "))
        {
            sscanf(tmp[i].toStdString().c_str(),"illum %d", &illum);
            ismat = true;
        }
        else if(tmp[i].startsWith("isPlane "))
        {
            int t;
            sscanf(tmp[i].toStdString().c_str(), "isPlane %d", &t);
            isPlane = t;
        }
        else if(tmp[i].startsWith("map_Kd "))
        {
            QString textureName = tmp[i].section("map_Kd ", 0, 0, QString::SectionSkipEmpty);

            int texIndex = textureNames_.indexOf(textureName);
            if (texIndex != -1) {
                texturePix = texturePixmaps_[texIndex];
            }
            else {
                texturePix = QImage(directoryPath_ + textureName);
                // TODO: сделать проверку загрузки текстуры.
                texturePixmaps_.push_back(texturePix);
                textureNames_.push_back(textureName);
            }
            ismat = true;
        }
    }

    if (ismat)
        materials_.push_back(MaterialObjFile{ materialName, alpha, ns, ni, dif, amb, spec, emis, illum, texturePix, isPlane });
}


void ObjFileReader::handleFileString(QString fileStr)
{
    if(fileStr.isEmpty() || fileStr.startsWith("#"))
        return;

    if(fileStr.startsWith("v "))
    {
        float tmpx, tmpy, tmpz;
        sscanf(fileStr.toStdString().c_str(), "v %f %f %f", &tmpx,&tmpy,&tmpz);
        vertexPositions_.push_back(QVector3D(tmpx, tmpy, tmpz));
    }
    else if(fileStr.startsWith("vt "))
    {
        float tmpx, tmpy;
        sscanf(fileStr.toStdString().c_str(), "vt %f %f", &tmpx, &tmpy);
        vertexTexturePositions_.push_back(QVector2D{tmpx, tmpy});
    }
    else if(fileStr.startsWith("vn "))
    {
        float tmpx, tmpy, tmpz;
        sscanf(fileStr.toStdString().c_str(), "vn %f %f %f", &tmpx,&tmpy,&tmpz);
        vertexNormals_.push_back(QVector3D(tmpx, tmpy, tmpz));
    }
    else if(fileStr.startsWith("f "))
    {
        int v[3];
        int n[3];
        int t[3];

        if(std::count(fileStr.begin(), fileStr.end(), ' ') > 3) {
            QString errorMsg = QString("Error: face have more that 3 vertexes.");
            throw SstException(errorMsg);
        }

        bool textureUse = false;
        if(fileStr.indexOf("//") != -1) {
            sscanf(fileStr.toStdString().c_str(),"f %d//%d %d//%d %d//%d", &v[0], &n[0],  &v[1], &n[1], &v[2], &n[2] );
        }
        else if(fileStr.indexOf("/") != -1) {
            sscanf(fileStr.toStdString().c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0],  &v[1], &t[1], &n[1],  &v[2], &t[2], &n[2] );
            textureUse = true;
        }
        else {
            QString errorMsg = QString("Error: undefined string of face information.");
            throw SstException(errorMsg);
        }

        QVector<VertexObjFile> tmpVec(3);
        tmpVec[0].positionId = v[0];
        tmpVec[1].positionId = v[1];
        tmpVec[2].positionId = v[2];

        tmpVec[0].normalId = n[0];
        tmpVec[1].normalId = n[1];
        tmpVec[2].normalId = n[2];

        if(textureUse)
        {
            tmpVec[0].texturePositionId = t[0];
            tmpVec[1].texturePositionId = t[1];
            tmpVec[2].texturePositionId = t[2];
        }

        faces_.push_back(FaceObjFile{ tmpVec, currentMaterial_ });
    }
    else if(fileStr.startsWith("mtllib "))
        readStringWithMaterialInfo(fileStr);
    else if(fileStr.startsWith("usemtl "))
    {
        QString tmp = fileStr.section("usemtl ", 0, 0, QString::SectionSkipEmpty);

        for( int j = 0; j < materials_.size(); j++)
        {
            if(materials_[j].name == tmp)
            {
                currentMaterial_ = j;
                break;
            }
        }
    }
}


void ObjFileReader::someProcessWithMaterial(const MaterialObjFile &material, Model3DObject *M, const uint matIndex)
{
    QVector3D am(material.am[0], material.am[1], material.am[2]);
    QVector3D di(material.di[0], material.di[1], material.di[2]);
    QVector3D sp(material.sp[0], material.sp[1], material.sp[2]);
    QVector3D em(material.em[0], material.em[1], material.em[2]);

    // TODO: нужно сделать, чтобы работало более, чем с одной текстурой в 3д модели.
    SurfaceWithOneMaterial surfaceWithOneMaterial( LightInteractingMaterial{ am, di, sp, em, material.ns },
                                                   getFacesForMaterial(matIndex),
                                                   texturePixmaps_[0] );

    vertexPositions_.clear();
    vertexTexturePositions_.clear();
    vertexNormals_.clear();

    M->addSurface(surfaceWithOneMaterial);
}


QVector<Face3v> ObjFileReader::getFacesForMaterial(const uint matIndex) const
{
    QVector<Face3v> facesOfOneMaterial;
    for (auto objFace : faces_)
    {
        if (objFace.materialId == matIndex)
        {
            QVector<Vertex> tmpVertexVector;
            for(int i = 0; i < 3; i++)
            {
                QVector3D position = vertexPositions_[ objFace.vertexes[i].positionId - 1 ];
                QVector2D texturePosition = vertexTexturePositions_[ objFace.vertexes[i].texturePositionId - 1 ];
                QVector3D normal = vertexNormals_[ objFace.vertexes[i].normalId - 1 ];
                tmpVertexVector.push_back( Vertex{ position, texturePosition, normal } );
            }

            facesOfOneMaterial.push_back(Face3v(tmpVertexVector));
        }
    }
    return facesOfOneMaterial;
}


Model3DObject* ObjFileReader::load(QString filePath)
try
{
    handleFile(filePath);
    return createModel3DObject();
}
catch (SstException &ex) {
    qFatal(ex.what());
    throw SstException("Не удалось загрузить файл");
}


void ObjFileReader::handleFile(QString filePath)
{
    directoryPath_ = getFileDirectoryFromFilePath(filePath);
    const QVector<QString> fileStrings = this->readFile(filePath);
    qDebug() << QString("Strings count in file %1 = %2").arg(filePath).arg(fileStrings.size());

    for(int i = 0; i < fileStrings.size(); i++) {
        handleFileString(fileStrings[i]);
    }
}


Model3DObject* ObjFileReader::createModel3DObject()
{
    Model3DObject *M = new Model3DObject();

    for(int i = 0; i < materials_.size(); i++) {
        someProcessWithMaterial(materials_[i], M, i);
    }

    faces_.clear();
    materials_.clear();
    texturePixmaps_.clear();
    textureNames_.clear();

    return M;
}


QVector<QString> ObjFileReader::readFile(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString errorMsg = QString("Error: can't to open file (%1)").arg(filePath);
        throw SstException(errorMsg);
    }

    qDebug() << QString("File %1 opened").arg(filePath);

    QVector<QString> strStore;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        strStore.push_back(line);
    }

    return strStore;
}
