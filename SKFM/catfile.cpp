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
        file.read((char*)&headerSize, sizeof(int));
        file.read((char*)&extraData, sizeof(int));
        file.seek(extraData);
        file.read((char*)&extraSize, sizeof(int));
        file.read((char*)&fileCount, sizeof(int));
        file.read((char*)&fileSize, sizeof(int));
        contentOffsets = new int[fileCount];
        for(int i = 0; i < fileCount; i++)
        {
            file.read((char*)&contentOffsets[i], sizeof(int));
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Unable to open " + fileName);
        msgBox.exec();
    }
    file.close();
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
    QFile file(location);
    if(file.open(QIODevice::ReadOnly))
    {
        file.seek(contentOffsets[index - 1] + extraData + extraSize);

        int size;

        // if this is the last file the file ends at the end of the gxt file
        if (index == fileCount)
        {
            size = file.size() - contentOffsets[index - 1] - extraData - extraSize;
        }
        else
        {
            size = contentOffsets[index] - contentOffsets[index - 1];
        }
        return file.read(size);
    }
    return QByteArray();
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
