#ifndef GXTFILE_H
#define GXTFILE_H

#include <QString>
#include <QPixmap>
#include "resourcefile.h"

class GxtFile : public ResourceFile
{
public:
    GxtFile(QString filePath);
    ~GxtFile();
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
    int fileCount;
    int fileSize;
    int* contentOffsets;
    QByteArray fileData;
    QString fileName;
    QString location;
};

#endif // GXTFILE_H
