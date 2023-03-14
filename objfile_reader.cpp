#include "objfile_reader.h"
#include "sst_exeption.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

#include "sst_exeption.h"

// Возвращает директория файла с символом / в конце.
QString getFileDirectoryFromFilePath(QString filePath)
{
    QRegularExpression regExp(R"***((.*/)[a-zA-Z0-9]+\.obj$)***");
    QRegularExpressionMatch match = regExp.match(filePath);

    if (match.hasMatch())
        return match.captured(1);

    QString errorMsg = "Error: can't to identify directory path.";
    throw SstExeption(errorMsg);
}


void ObjFileReader::readStringWithMaterialInfo(QString materialStr)
{
    char  mtlName[64]={'\0'};
    sscanf(materialStr.toStdString().c_str(), "mtllib %s", mtlName);
    QString materialFilePath = directoryPath + mtlName;

    QFile materialFile(materialFilePath);
    if(!materialFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString errorMsg = QString("Error: can't open %1.").arg(materialFilePath);
        throw SstExeption(errorMsg);
    }

    std::vector<QString> tmp;
    QTextStream in(&materialFile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        tmp.push_back(line);
    }

    QString materialName;
    QString textureName;
    QString textureFullPath;

    QVector3D amb;
    QVector3D dif;
    QVector3D spec;
    QVector3D emis;

    float alpha, ns, ni;
    int illum;

    GLuint textureId;
    bool isPlane;
    bool ismat=false;

    for(uint i = 0; i < tmp.size(); i++)
    {
        if(materialStr.size() == 0 || tmp[i][0]=='#')
            continue;

        if(tmp[i][0]=='n' && tmp[i][1]=='e' && tmp[i][2]=='w')
        {
            if (ismat)
            {
                if (textureName.isEmpty())
                    materials.push_back(MaterialObjFile{ materialName, alpha, ns, ni, dif, amb, spec, emis, illum, 0, isPlane });
                else
                {
                    materials.push_back(MaterialObjFile{ materialName, alpha, ns, ni, dif, amb, spec, emis, illum, textureId, isPlane });
                    textureName.clear();
                    textureFullPath.clear();
                }
            }

            ismat=false;
            isPlane=false;
            materialName = tmp[i].section("newmtl ", 0, 0, QString::SectionSkipEmpty);
        }
        else if(tmp[i][0]=='N' && tmp[i][1]=='s')
            sscanf(tmp[i].toStdString().c_str(),"Ns %f", &ns);
        else if(tmp[i][0]=='K' && tmp[i][1]=='a')
            sscanf(tmp[i].toStdString().c_str(),"Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
        else if(tmp[i][0]=='K' && tmp[i][1]=='d')
            sscanf(tmp[i].toStdString().c_str(),"Kd %f %f %f", &dif[0], &dif[1], &dif[2]);
        else if(tmp[i][0]=='K' && tmp[i][1]=='s')
            sscanf(tmp[i].toStdString().c_str(),"Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
        else if(tmp[i][0]=='K' && tmp[i][1]=='e')
            sscanf(tmp[i].toStdString().c_str(),"Ke %f %f %f", &emis[0], &emis[1], &emis[2]);
        else if(tmp[i][0]=='N' && tmp[i][1]=='i')
            sscanf(tmp[i].toStdString().c_str(),"Ni %f", &ni);
        else if(tmp[i][0]=='d' && tmp[i][1]==' ')
            sscanf(tmp[i].toStdString().c_str(),"d %f", &alpha);
        else if(tmp[i][0]=='i' && tmp[i][1]=='l')
        {
            sscanf(tmp[i].toStdString().c_str(),"illum %d", &illum);
            ismat=true;
        }
        else if(tmp[i][0]=='i' && tmp[i][1]=='s' && tmp[i][2]=='P')
        {
            int t;
            sscanf(tmp[i].toStdString().c_str(), "isPlane %d", &t);
            isPlane = t;
        }
        else if(tmp[i][0]=='m' && tmp[i][1]=='a')
        {
     //     sscanf(tmp[i].c_str(), "map_Kd %s", textureName);

     //     int texIndex = textureNames.indexOf(textureName);
     //     if (texIndex != -1)
     //     {
     //         textureId = textureIds[texIndex];
     //     }
     //     else
     //     {
     //         textureFullPath = DirectionFiles;
     //         textureFullPath += textureName;
     //         textureId = loadTexture(textureFullPath);
     //         // TODO: сделать проверку загрузки текстуры.
     //         textureIds.push_back(textureId);
     //         textureNames.push_back(textureName);
     //     }
     //     ismat=true;
        }
    }

    if (ismat)
        materials.push_back(MaterialObjFile{ materialName, alpha, ns, ni, dif, amb, spec, emis, illum, 0, isPlane });
}

void ObjFileReader::handleFileString(std::string fileStr)
{
    if(fileStr.empty() || fileStr[0] == '#')
        return;

    if(fileStr[0] == 'v' && fileStr[1] == ' ')
    {
        float tmpx, tmpy, tmpz;
        sscanf(fileStr.c_str(), "v %f %f %f", &tmpx,&tmpy,&tmpz);
        vertexPositions.push_back(QVector3D(tmpx, tmpy, tmpz));
    }
    else if(fileStr[0] == 'v' && fileStr[1] =='t')
    {
        float tmpx, tmpy;
        sscanf(fileStr.c_str(), "vt %f %f", &tmpx, &tmpy);
        vertexTexturePositions.push_back(QVector2D{tmpx, tmpy});
    }
    else if(fileStr[0] == 'v' && fileStr[1] == 'n')
    {
        float tmpx, tmpy, tmpz;
        sscanf(fileStr.c_str(), "vn %f %f %f", &tmpx,&tmpy,&tmpz);
        vertexNormals.push_back(QVector3D(tmpx, tmpy, tmpz));
    }
    else if(fileStr[0] == 'f')
    {
        int v[3];
        int n[3];
        int t[3];

        if(std::count(fileStr.begin(), fileStr.end(), ' ') > 3)
        {
            QString errorMsg = QString("Error: face have more that 3 vertexes.");
            throw SstExeption(errorMsg);
        }

        bool textureUse = false;
        if(fileStr.find("//")!=std::string::npos)
        {
            sscanf(fileStr.c_str(),"f %d//%d %d//%d %d//%d", &v[0], &n[0],  &v[1], &n[1], &v[2], &n[2] );
        }
        else if(fileStr.find("/")!=std::string::npos)
        {
            sscanf(fileStr.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0],  &v[1], &t[1], &n[1],  &v[2], &t[2], &n[2] );
            textureUse = true;
        }
        else
        {
            QString errorMsg = QString("Error: undefined string of face information.");
            throw SstExeption(errorMsg);
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

        faces.push_back(FaceObjFile{ tmpVec, currentMaterial });
    }
    else if(fileStr[0] == 'm' && fileStr[1] == 't' && fileStr[2] == 'l' && fileStr[3] == 'l')
        readStringWithMaterialInfo(fileStr.c_str());
    else if(fileStr[0] == 'u' && fileStr[1] == 's' && fileStr[2] == 'e')
    {
        char tmp[200];
        sscanf(fileStr.c_str(), "usemtl %s", tmp);

        for( int j = 0; j < materials.size(); j++)
        {
            if(materials[j].name == tmp)
            {
                currentMaterial = j;
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

    SurfaceWithOneMaterial surfaceWithOneMaterial(LightInteractingMaterial{ am, di, sp, em, material.ns });

    for (auto objFace : faces)
    {
        if (objFace.materialId == matIndex)
        {
            QVector<Vertex> tmpVertexVector;
            for(int i = 0; i < 3; i++)
            {
                QVector3D position = vertexPositions[ objFace.vertexes[i].positionId - 1 ];
                QVector2D texturePosition = vertexTexturePositions[ objFace.vertexes[i].texturePositionId - 1 ];
                QVector3D normal = vertexNormals[ objFace.vertexes[i].normalId - 1 ];
                tmpVertexVector.push_back( Vertex{ position, texturePosition, normal } );
            }

            surfaceWithOneMaterial.addFace(Face3v(tmpVertexVector));
        }
    }

    M->addSurface(surfaceWithOneMaterial);
}


Model3DObject* ObjFileReader::load(QString filePath)
{
    handleFile(filePath);
    return createModel3DObject();
}


void ObjFileReader::handleFile(QString filePath)
{
    directoryPath = getFileDirectoryFromFilePath(filePath);
    const QVector<QString> fileStrings = this->readFile(filePath);

    for(int i = 0; i < fileStrings.size(); i++)
    {
        handleFileString(fileStrings[i].toStdString());
    }
}


Model3DObject* ObjFileReader::createModel3DObject()
{
    Model3DObject *M = new Model3DObject();

    for(int i = 0; i < materials.size(); i++)
    {
        someProcessWithMaterial(materials[i], M, i);
    }

    return M;
}


QVector<QString> ObjFileReader::readFile(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString errorMsg = QString("Error: can't to open file (%1)").arg(filePath);
        throw SstExeption(errorMsg);
    }

    QVector<QString> strStore;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        strStore.push_back(line);
    }

    return strStore;
}
