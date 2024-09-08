#pragma once

using namespace AquaXP;

bool wideStrToMbStr(const std::wstring& in, std::string& out)
{
    // Get buffer size
    i32 bufferSize = WideCharToMultiByte(
        CP_UTF8, 0,
        in.c_str(), (i32)in.size(),
        nullptr, 0,
        nullptr, nullptr
    );

    // Set outstring to new buffer size
    out = std::string(bufferSize, '\0');

    //Convert
    i32 res = WideCharToMultiByte(
        CP_UTF8, 0,
        in.c_str(), (i32)in.size(),
        &out[0], (i32)out.size(),
        nullptr, nullptr
    );
    if (!res)
    {
        return false;
    }
    return true;
}

bool mbStrToWideChar(const std::string& in, std::wstring& out)
{
    bool status = true;
    i32 bufferSize = MultiByteToWideChar(
        CP_ACP, 0, in.c_str(), -1, nullptr, 0
    );

    WCHAR* buffer = new WCHAR[bufferSize];
    i32 conversionResult = MultiByteToWideChar(
        CP_ACP, 0,
        in.c_str(), -1,
        buffer, bufferSize
    );

    if (!conversionResult)
    {
        status = false;
        goto Delete;
    }

    out = std::wstring(buffer);

Delete:
    if (buffer)
    {
        delete[] buffer;
    }
    return status;
}