#include "pch.h"
#include "FileHelpers.h"

_Success_(return)
bool AquaXP::TryReadFile(
    _In_ char const* path,
    _Out_ u8 * *buffer,
    _Out_opt_ sz * outSize
)
{
    if (buffer == NULL)
    {
        return false;
    }
    HANDLE handle = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    bool result = AquaXP::TryReadFileHandle(handle, buffer, outSize);
    CloseHandle(handle);
    return result;
}

_Success_(return)
bool AquaXP::TryReadFile(
    _In_ WCHAR const* path,
    _Out_ u8 **buffer,
    _Out_opt_ sz * outSize
)
{
    if (buffer == NULL)
    {
        return false;
    }
    HANDLE handle = CreateFileW(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    bool result = TryReadFileHandle(handle, buffer, outSize);
    CloseHandle(handle);
    return result;
}

_Success_(return)
bool AquaXP::TryReadFile(
    _In_ WCHAR const* path,
    _Out_ std::string & buffer
)
{
    u8* tempBuffer = nullptr;
    sz size = 0;
    if (!TryReadFile(path, &tempBuffer, &size))
    {
        return false;
    }
    buffer = std::string(reinterpret_cast<char*>(tempBuffer), size);
    delete[] tempBuffer;
    return true;
}

_Success_(return)
bool AquaXP::TryReadFile(
    _In_ char const* path,
    _Out_ std::string & buffer
)
{
    u8* tempBuffer = nullptr;
    sz size = 0;
    if (!TryReadFile(path, &tempBuffer, &size))
    {
        return false;
    }
    buffer = std::string(reinterpret_cast<char*>(tempBuffer), size);
    delete[] tempBuffer;
    return true;
}

_Success_(return)
bool AquaXP::TryReadFileHandle(
    HANDLE handle,
    _Out_ u8 * *buffer,
    _Out_opt_ sz * outSize
)
{
    u32 size = GetFileSize(handle, NULL);
    if (outSize != NULL)
    {
        *outSize = size;
    }

    if (size == 0)
    {
        return false;
    }

    *buffer = new u8[size + 1];
    ZeroMemory(*buffer, size + 1);
    return ReadFile(handle, *buffer, size + 1, NULL, NULL);
}

/* Local helper */
_Success_(return)
bool CheckFileAttributesExists(DWORD const fileAttributes)
{
    if (fileAttributes == INVALID_FILE_ATTRIBUTES)
    {
        DWORD lastError = GetLastError();
        if (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND)
        {
            return false;
        }
    }
    return true;

}

_Success_(return)
bool AquaXP::FileExists(_In_ char const* const path)
{
    DWORD fileAttributes = GetFileAttributesA(path);
    return CheckFileAttributesExists(fileAttributes);
}

_Success_(return)
bool AquaXP::FileExistsW(_In_ WCHAR const* const path)
{
    DWORD fileAttributes = GetFileAttributesW(path);
    return CheckFileAttributesExists(fileAttributes);
}