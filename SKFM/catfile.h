#ifndef CATFILE_H
#define CATFILE_H

#include "resourcefile.h"
#include<QPixmap>
#include<vector>

using std::vector;

class CatFile : public ResourceFile
{
public:
    CatFile(QString);
    ~CatFile();
    int getHeaderSize();
    int getFileCount();
    int getFileSize();
    QString getFileName();
    QString getFileLocation();
    QPixmap readDDSFile(int);
    QByteArray readFileData(int);
private:
    class ExtraData
    {
    public:
        ExtraData(int,int*,QByteArray*);
        ~ExtraData();
        int getExtraCount();
        QByteArray readFileData(int,QByteArray*);
    private:
        int extraOffset;
        int extraHeader;
        int extraCount;
        int extraSize;
        int* contentOffsets;
    };

    int headerSize;
    int fileCount;
    int fileSize;
    QVector<ExtraData*> extraData;
    QByteArray fileData;
    QString fileName;
    QString location;
};

#endif // CATFILE_H
