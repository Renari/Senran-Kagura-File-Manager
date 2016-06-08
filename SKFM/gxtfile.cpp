#include "gxtfile.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QBuffer>

GxtFile::GxtFile(QString filePath)
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
    buffer.read((char*)&fileCount, sizeof(int));
    buffer.read((char*)&fileSize, sizeof(int));
    contentOffsets = new int[fileCount];
    for(int i = 0; i < fileCount; i++)
    {
        buffer.read((char*)&contentOffsets[i], sizeof(int));
    }
}

GxtFile::~GxtFile()
{
    delete[] contentOffsets;
}

int GxtFile::getHeaderSize()
{
    return headerSize;
}

int GxtFile::getFileCount()
{
    return fileCount;
}

int GxtFile::getFileSize()
{
    return fileSize;
}

int GxtFile::getContentOffset(int index)
{
    if (index <= fileCount && index > 0)
    {
        return contentOffsets[index];
    }
    return 0;
}

QString GxtFile::getFileName()
{
    return fileName;
}

QString GxtFile::getFileLocation()
{
    return location;
}

QByteArray GxtFile::readFileData(int index)
{
    if (index > fileCount || index < 1)
        return QByteArray();

    QBuffer buffer(&fileData);
    buffer.open(QBuffer::ReadOnly);
    buffer.seek(contentOffsets[index - 1] + headerSize);

    int size;

    // if this is the last file the file ends at the end of the gxt file
    if (index == fileCount)
    {
        size = buffer.size() - contentOffsets[index - 1] - headerSize;
    }
    else
    {
        size = contentOffsets[index] - contentOffsets[index - 1];
    }

    return buffer.read(size);
}

QPixmap GxtFile::readDDSFile(int index)
{
    if (index > fileCount || index < 1)
        return QPixmap();

    QByteArray dds = readFileData(index);
    QPixmap pixmap;
    pixmap.loadFromData(dds);

    return pixmap;
}
