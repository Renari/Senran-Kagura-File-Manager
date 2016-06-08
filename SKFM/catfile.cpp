#include "catfile.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QBuffer>

CatFile::CatFile(QString filePath)
{
    location = filePath;
    fileName = QFileInfo(filePath).fileName();
    fileCount = 0;

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
    int offset;
    buffer.read((char*)&offset, sizeof(offset));
    do{
        extraData.push_back(new ExtraData(offset, &fileCount, &fileData));
        buffer.read((char*)&offset, sizeof(offset));
    } while(offset != -1);
}

CatFile::~CatFile()
{
    foreach(ExtraData *data, extraData)
    {
        delete data;
    }
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
    return fileData.size();
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

    if (index == fileCount)
    {
        int count = extraData.last()->getExtraCount();
        return extraData.last()->readFileData(count,&fileData);
    }

    //determine which extra data segment this file is in
    int extraFileCount = fileCount;
    int extraIndex = -1;
    do
    {
        extraIndex++;
        extraFileCount -= extraData[extraIndex]->getExtraCount();
    } while (extraFileCount > index);

    return extraData[extraIndex]->readFileData(index - extraFileCount + 1, &fileData);
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

CatFile::ExtraData::ExtraData(int offset, int *fileCount, QByteArray *fileData)
{
    extraOffset = offset;
    QBuffer buffer(fileData);
    buffer.open(QBuffer::ReadOnly);
    buffer.seek(offset);
    QByteArray gxt = buffer.peek(3);
    if (QString(gxt.data()) == "GXT")
    {
        extraHeader = 0;
        extraCount = 0;
        extraSize = 0;
        return;
    }
    buffer.read((char*)&extraHeader, sizeof(extraHeader));
    buffer.read((char*)&extraCount, sizeof(extraCount));
    *fileCount += extraCount; // keep track of the total number of files
    buffer.read((char*)&extraSize, sizeof(extraSize));
    contentOffsets = new int[extraCount];
    for(int i = 0; i < extraCount; i++)
    {
        buffer.read((char*)&contentOffsets[i], sizeof(int));
    }
}

CatFile::ExtraData::~ExtraData()
{
    delete[] contentOffsets;
}

QByteArray CatFile::ExtraData::readFileData(int index, QByteArray *fileData)
{
    if (index > extraCount || index < 1)
        return QByteArray();

    QBuffer buffer(fileData);
    buffer.open(QBuffer::ReadOnly);
    buffer.seek(contentOffsets[index - 1] + extraOffset + extraHeader);

    int size;

    // if this is the last file the file ends at the end of this extra sement
    if (index == extraCount)
    {
        // we add 4 to get to the end of the extra segment here
        // because extra size starts at extraCount not extraHeader
        size = extraSize + 4 - contentOffsets[index - 1] - extraHeader;
    }
    else
    {
        size = contentOffsets[index] - contentOffsets[index - 1];
    }
    return buffer.read(size);
}

int CatFile::ExtraData::getExtraCount()
{
    return extraCount;
}
