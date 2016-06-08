#ifndef CATFILE_H
#define CATFILE_H

#include<QPixmap>
#include "resourcefile.h"

class CatFile : public ResourceFile
{
public:
    CatFile(QString);
    ~CatFile();
    int getHeaderSize();
    int getFileCount();
    int getFileSize();
    int getContentOffset(int);
    QString getFileName();
    QString getFileLocation();
    QPixmap readDDSFile(int);
    QByteArray readFileData(int);
private:
    int headerSize;
    int extraData;
    int extraSize;
    int fileCount;
    int fileSize;
    int* contentOffsets;
    QByteArray fileData;
    QString fileName;
    QString location;
};

#endif // CATFILE_H
