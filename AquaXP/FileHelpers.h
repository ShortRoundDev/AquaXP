#pragma once
namespace AquaXP
{
    _Success_(return)
    bool TryReadFile(
        _In_ char const* path,
        _Out_ u8** buffer,
        _Out_opt_ sz* outSize
    );

    _Success_(return)
    bool TryReadFile(
        _In_ char const* path,
        _Out_ std::string& buffer
    );

    _Success_(return)
    bool TryReadFile(
        _In_ WCHAR const* path,
        _Out_ u8** buffer,
        _Out_opt_ sz* outSize
    );

    _Success_(return)
    bool TryReadFile(
        _In_ WCHAR const* path,
        _Out_ std::string & buffer
    );

    _Success_(return)
    bool TryReadFileHandle(
        HANDLE handle,
        _Out_ u8** buffer,
        _Out_opt_ sz* outSize
    );

    _Success_(return)
    bool FileExists(_In_ char const* path);

    _Success_(return)
    bool FileExistsW(_In_ WCHAR const* path);
}