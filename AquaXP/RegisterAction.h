#pragma once

#define AQUA_REGISTER(ACTION_NAME) \
    namespace AquaXP::Actions { \
        constexpr int ACTION_NAME = 64 + __COUNTER__;\
    };
