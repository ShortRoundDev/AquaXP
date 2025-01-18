#pragma once

/* Allow users to use the Windows min/max if preferred. Just set NOMINMAX like regular if needed */
#if defined(min)
#define AQUA_MIN min
#else
#define AQUA_MIN std::min
#endif

namespace AquaXP
{
    template<template<typename> typename Alloc = std::allocator>
    _Success_(return)
    inline bool TryReadFile(
        _In_ char const* path,
        _Out_ std::shared_ptr<u8[]>& buffer,
        _Out_ sz& outSize
    )
    {
        using AllocType = Alloc<u8>;
        using AllocTraits = std::allocator_traits<AllocType>;
        static_assert(std::is_same_v<typename AllocTraits::value_type, u8>,
            "Allocator must be for u8 type");

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
        bool result = TryReadFileHandle<Alloc>(handle, buffer, outSize);
        CloseHandle(handle);
        return result;
    }

    template<template<typename> typename Alloc = std::allocator>
    _Success_(return)
    inline bool TryReadFile(
        _In_ WCHAR const* path,
        _Out_ std::shared_ptr<u8[]>& buffer,
        _Out_ sz& outSize
    )
    {
        using AllocType = Alloc<u8>;
        using AllocTraits = std::allocator_traits<AllocType>;
        static_assert(std::is_same_v<typename AllocTraits::value_type, u8>,
            "Allocator must be for u8 type");

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
        bool result = TryReadFileHandle<Alloc>(handle, buffer, outSize);
        CloseHandle(handle);
        return result;
    }

    template<
        template<typename> typename ByteAlloc = std::allocator,
        template<typename> typename StringAlloc = ByteAlloc
    >
    _Success_(return)
    inline bool TryReadFile(
        _In_ WCHAR const* path,
        _Out_ std::string& buffer
    )
    {
        using ByteAllocType = ByteAlloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

        using StringAllocType = StringAlloc<char>;
        using StringAllocTraits = std::allocator_traits<StringAllocType>;
        static_assert(std::is_same_v<typename StringAllocTraits::value_type, char>,
            "String Allocator must be for char type");

        std::shared_ptr<u8[]> tempBuffer;
        sz size = 0;
        if (!TryReadFile<ByteAlloc>(path, tempBuffer, size))
        {
            return false;
        }

        StringAlloc<char> charAllocator;
        buffer = std::basic_string<char, std::char_traits<char>, StringAlloc<char>>(
            reinterpret_cast<char*>(tempBuffer.get()),
            size,
            charAllocator
        );
        return true;
    }

    template<
        template<typename> typename ByteAlloc = std::allocator,
        template<typename> typename StringAlloc = ByteAlloc
    >
    _Success_(return)
    inline bool TryReadFile(
        _In_ char const* path,
        _Out_ std::string& buffer
    )
    {
        using ByteAllocType = ByteAlloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

        using StringAllocType = StringAlloc<char>;
        using StringAllocTraits = std::allocator_traits<StringAllocType>;
        static_assert(std::is_same_v<typename StringAllocTraits::value_type, char>,
            "String Allocator must be for char type");

        std::shared_ptr<u8[]> tempBuffer;
        sz size = 0;
        if (!TryReadFile<ByteAlloc>(path, tempBuffer, size))
        {
            return false;
        }

        StringAlloc<char> charAllocator;
        buffer = std::basic_string<char, std::char_traits<char>, StringAlloc<char>>(
            reinterpret_cast<char*>(tempBuffer.get()),
            size,
            charAllocator
        );
        return true;
    }

    template<template<typename> typename Alloc = std::allocator>
    _Success_(return)
    inline bool TryReadFileHandle(
        HANDLE handle,
        _Out_ std::shared_ptr<u8[]> &buffer,
        _Out_ sz& outSize
    )
    {
        using AllocType = Alloc<u8>;
        using AllocTraits = std::allocator_traits<AllocType>;
        static_assert(std::is_same_v<typename AllocTraits::value_type, u8>,
            "Allocator must be for u8 type");

        if (handle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(handle, &fileSize))
        {
            return false;
        }

        outSize = fileSize.QuadPart;
        if(outSize == 0)
        {
            return false;
        }

        AllocType alloc;
        buffer = std::allocate_shared_for_overwrite<u8[], AllocType>(alloc, outSize + 1);

        sz remaining = outSize;
        u8* curr = buffer.get();
        while (remaining > 0)
        {
            DWORD toRead = static_cast<DWORD>(AQUA_MIN(remaining, static_cast<sz>(MAXDWORD)));
            DWORD bytesRead;

            if (ReadFile(handle, curr, toRead, &bytesRead, NULL) == FALSE ||
                bytesRead != toRead)
            {
                buffer = nullptr;
                return false;
            }

            remaining -= bytesRead;
            curr += bytesRead;
        }
        buffer.get()[outSize] = 0;
        return true;
    }

    /* Local helper */
    _Success_(return)
    inline bool CheckFileAttributesExists(DWORD const fileAttributes)
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
    inline bool FileExists(_In_ char const* const path)
    {
        DWORD fileAttributes = GetFileAttributesA(path);
        return CheckFileAttributesExists(fileAttributes);
    }

    _Success_(return)
    inline bool FileExistsW(_In_ WCHAR const* const path)
    {
        DWORD fileAttributes = GetFileAttributesW(path);
        return CheckFileAttributesExists(fileAttributes);
    }
}