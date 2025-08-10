#pragma once

../../

    constexpr bool is_valid_gsignal_flag(const char *flag)
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
        if (str_equal(flag, valid_flags[i])) return true;
    }

    return false;
}

constexpr bool is_valid_gsignal_signature(const char *signature)
{
    // TODO
    return true;
}

#define CHECK_GSIGNAL_FLAG(flag) \
    static_assert(is_valid_gsignal_flag(#flag), "Invalid GSIGNAL flag: " #flag)

#define VALIDATE_EACH_GSIGNAL_FLAG(...) FOR_EACH(CHECK_GSIGNAL_FLAG, SEMICOLON_END, __VA_ARGS__);

#define GSIGNAL(signature, ...)                                                        \
    static_assert(is_valid_gsignal_signature(#signature), "Invalid GSIGNAL signature") \
        VALIDATE_EACH_GSIGNAL_FLAG(__VA_ARGS__)