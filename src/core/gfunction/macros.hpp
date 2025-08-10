#pragma once

#include "../../macro_helpers.hpp"

namespace GDGenInternal
{
constexpr bool is_valid_gfunction_flag() { return true; }

constexpr bool is_valid_gfunction_flag(const char *flag)
{
    // constexpr const char *valid_flags[] = {
    //     "CustomBindings",
    //     "Resource",
    // };
    // constexpr size_t num_valid = sizeof(valid_flags) / sizeof(valid_flags[0]);

    // for (size_t i = 0; i < num_valid; ++i)
    // {
    //     if (str_equal(flag, valid_flags[i]))
    //         return true;
    // }

    return false;
}
}  // namespace GDGenInternal

#define CHECK_GFUNCTION_FLAG(flag)                               \
    static_assert(GDGenInternal::is_valid_gfunction_flag(#flag), \
                  "Invalid GFUNCTION "                           \
                  "flag: " #flag);

#define VALIDATE_EACH_GFUNCTION_FLAG(...) FOR_EACH(CHECK_GFUNCTION_FLAG, SEMICOLON_END, __VA_ARGS__)

#define GFUNCTION(...) VALIDATE_EACH_GFUNCTION_FLAG(__VA_ARGS__)