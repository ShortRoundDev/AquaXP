#pragma once

template<typename T>
concept IsAnyVector = requires {
    typename T::value_type;
    typename T::allocator_type;
        requires std::same_as<T, std::vector<typename T::value_type, typename T::allocator_type>>;
};
