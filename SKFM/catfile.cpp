#include "catfile.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QBuffer>

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

    QBuffer buffer(&fileData);
    buffer.open(QBuffer::ReadOnly);
    buffer.read((char*)&headerSize, sizeof(int));
    buffer.read((char*)&extraData, sizeof(int));
    buffer.seek(extraData);
    buffer.read((char*)&extraSize, sizeof(int));
    buffer.read((char*)&fileCount, sizeof(int));
    buffer.read((char*)&fileSize, sizeof(int));
    contentOffsets = new int[fileCount];
    for(int i = 0; i < fileCount; i++)
    {
        buffer.read((char*)&contentOffsets[i], sizeof(int));
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

    QBuffer buffer(&fileData);
    buffer.open(QBuffer::ReadOnly);
    buffer.seek(contentOffsets[index - 1] + extraData + extraSize);

    int size;

    // if this is the last file the file ends at the end of the cat file
    if (index == fileCount)
    {
        size = buffer.size() - contentOffsets[index - 1] - extraData - extraSize;
    }
    else
    {
        size = contentOffsets[index] - contentOffsets[index - 1];
    }
    return buffer.read(size);
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
