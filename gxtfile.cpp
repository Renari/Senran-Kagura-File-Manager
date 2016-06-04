#include "gxtfile.h"
#include <QFile>
#include <QFileInfo>
#include <QGLPixelBuffer>
#include <QMessageBox>

GxtFile::GxtFile(QString filePath)
{
    location = filePath;
    fileName = QFileInfo(filePath).fileName();
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        file.read((char*)&headerSize, sizeof(int));
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
    QFile file(location);
    if(file.open(QIODevice::ReadOnly))
    {
        file.seek(contentOffsets[index - 1] + headerSize);

        int size;

        // if this is the last file the file ends at the end of the gxt file
        if (index == fileCount)
        {
            size = file.size() - contentOffsets[index - 1] - headerSize;
        }
        else
        {
            size = contentOffsets[index] - contentOffsets[index - 1];
        }
        return file.read(size);
    }
    return QByteArray();
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
