#include "catfile.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

CatFile::CatFile(QString filePath)
{
    location = filePath;
    fileName = QFileInfo(filePath).fileName();
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        fileData = file.readAll();
        file.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Unable to open " + fileName);
        msgBox.exec();
        return;
    }

    QDataStream stream(&fileData, QIODevice::ReadOnly);
    stream.read((char*)&headerSize, sizeof(int));
    stream.read((char*)&extraData, sizeof(int));
    stream.seek(extraData);
    stream.read((char*)&extraSize, sizeof(int));
    stream.read((char*)&fileCount, sizeof(int));
    stream.read((char*)&fileSize, sizeof(int));
    contentOffsets = new int[fileCount];
    for(int i = 0; i < fileCount; i++)
    {
        stream.read((char*)&contentOffsets[i], sizeof(int));
    }
}

CatFile::~CatFile()
{
    delete[] contentOffsets;
}

int CatFile::getHeaderSize()
{
    return headerSize;
}

int CatFile::getFileCount()
{
    return fileCount;
}

int CatFile::getFileSize()
{
    return fileSize;
}

int CatFile::getContentOffset(int index)
{
    if (index <= fileCount && index > 0)
    {
        return contentOffsets[index];
    }
    return 0;
}

QString CatFile::getFileName()
{
    return fileName;
}

QString CatFile::getFileLocation()
{
    return location;
}

QByteArray CatFile::readFileData(int index)
{
    if (index > fileCount || index < 1)
        return QByteArray();

    QDataStream stream(fileData, QIODevice::ReadOnly);
    stream.seek(contentOffsets[index - 1] + extraData + extraSize);

    int size;

    // if this is the last file the file ends at the end of the cat file
    if (index == fileCount)
    {
        size = stream.size() - contentOffsets[index - 1] - extraData - extraSize;
    }
    else
    {
        size = contentOffsets[index] - contentOffsets[index - 1];
    }
    return stream.read(size);
}

QPixmap CatFile::readDDSFile(int index)
{
    if (index > fileCount || index < 1)
        return QPixmap();

    QByteArray dds = readFileData(index);
    QPixmap pixmap;
    pixmap.loadFromData(dds);

    return pixmap;
}
