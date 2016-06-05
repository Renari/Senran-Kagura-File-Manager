#ifndef RESOURCEFILE_H
#define RESOURCEFILE_H

#include <QString>

class ResourceFile
{
public:
    ResourceFile();
    virtual ~ResourceFile();
    virtual int getHeaderSize() = 0;
    virtual int getFileCount() = 0;
    virtual int getFileSize() = 0;
    virtual int getContentOffset(int) = 0;
    virtual QString getFileName() = 0;
    virtual QString getFileLocation() = 0;
};

#endif // RESOURCEFILE_H
