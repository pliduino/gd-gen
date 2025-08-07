#pragma once

#include "macro_helpers.hpp"

constexpr bool is_valid_gstruct_flag(const char *flag)
{
    constexpr const char *valid_flags[] = {
        "Required",
        "NoStorage",
        "HideInInspector",
        "MultilineText",
    };
    constexpr size_t num_valid = sizeof(valid_flags) / sizeof(valid_flags[0]);

    for (size_t i = 0; i < num_valid; ++i)
    {
        if (str_equal(flag, valid_flags[i]))
            return true;
    }
    if (starts_with(flag, "Description("))
        return true;

    return false;
}

#define CHECK_GSTRUCT_FLAG(flag) \
    static_assert(is_valid_gstruct_flag(#flag), "Invalid GSTRUCT flag: " #flag)

#define VALIDATE_EACH_GSTRUCT_FLAG(...) FOR_EACH(CHECK_GSTRUCT_FLAG, SEMICOLON_END, __VA_ARGS__);

#define GSTRUCT(...) \
    VALIDATE_EACH_GSTRUCT_FLAG(__VA_ARGS__);