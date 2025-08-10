#pragma once

#include "../../macro_helpers.hpp"

namespace GDGenInternal
{
constexpr bool is_valid_gclass_flag() { return true; }

constexpr bool is_valid_gclass_flag(const char *flag)
{
    constexpr const char *valid_flags[] = {
        "CustomBindings", "Resource", "Virtual", "Abstract", "Runtime", "Internal",
    };
    constexpr size_t num_valid = sizeof(valid_flags) / sizeof(valid_flags[0]);

    for (size_t i = 0; i < num_valid; ++i)
    {
        if (str_equal(flag, valid_flags[i])) return true;
    }

    return false;
}
}  // namespace GDGenInternal

#define CHECK_GCLASS_FLAG(flag) \
    static_assert(GDGenInternal::is_valid_gclass_flag(#flag), "Invalid GCLASS flag: " #flag);

#define VALIDATE_EACH_GCLASS_FLAG(...) FOR_EACH(CHECK_GCLASS_FLAG, SEMICOLON_END, __VA_ARGS__)

#define GCLASS(...) VALIDATE_EACH_GCLASS_FLAG(__VA_ARGS__)