#pragma once

namespace AquaXP
{
	enum class Bind
	{
        None = 0,
        VS = 1,
        PS = 2,
        GS = 4,
        HS = 8,
        CS = 16,
        DS = 32
    };

    inline Bind operator |(Bind a, Bind b)
    {
        return static_cast<Bind>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline Bind operator &(Bind a, Bind b)
    {
        return static_cast<Bind>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline bool HasFlag(Bind value, Bind flag)
    {
        return (value & flag) != Bind::None;
    }
}