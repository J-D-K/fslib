#include "Storage.hpp"
#include "String.hpp"
#include <string>

// Globally used error string.
extern std::string g_ErrorString;

FsLib::Storage::Storage(FsBisPartitionId PartitionID)
{
    Storage::Open(PartitionID);
}

FsLib::Storage::~Storage()
{
    Storage::Close();
}

void FsLib::Storage::Open(FsBisPartitionId PartitionID)
{
    Result FsError = fsOpenBisStorage(&m_StorageHandle, PartitionID);
    if (R_FAILED(FsError))
    {
        g_ErrorString = FsLib::String::GetFormattedString("Error 0x%X opening partition ID 0x%X.", FsError, PartitionID);
        return;
    }

    FsError = fsStorageGetSize(&m_StorageHandle, &m_StreamSize);
    if (R_FAILED(FsError))
    {
        Storage::Close();
        g_ErrorString = FsLib::String::GetFormattedString("Error 0x%X getting storage size.");
        return;
    }
    m_Offset = 0;
    m_IsOpen = true;
}

void FsLib::Storage::Close(void)
{
    fsStorageClose(&m_StorageHandle);
}

size_t FsLib::Storage::Read(void *Buffer, size_t BufferSize)
{
    // Storage provides no way to check how much was actually read. Need to correct for this.
    if (m_Offset + static_cast<int64_t>(BufferSize) >= m_StreamSize)
    {
        BufferSize = m_StreamSize - m_Offset;
    }

    Result FsError = fsStorageRead(&m_StorageHandle, m_Offset, Buffer, static_cast<uint64_t>(BufferSize));
    if (R_FAILED(FsError))
    {
        g_ErrorString = FsLib::String::GetFormattedString("Error 0x%X reading from storage.", FsError);
        return 0;
    }
    // There really isn't a way to make sure this worked 100%...
    m_Offset += BufferSize;
    return BufferSize;
}