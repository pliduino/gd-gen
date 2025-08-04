#pragma once

#include <cstddef>
#include <cstring>

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

// #define STRINGIZE(x) #x
#define EXPAND(x) x

// Used for the FOR_EACH macros
#define COMMA_END() ,
#define SEMICOLON_END() ;

#define FOR_EACH_0(action, ...)

#define FOR_EACH_1(action, end, x) action(x)
#define FOR_EACH_2(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_1(action, end, __VA_ARGS__))
#define FOR_EACH_3(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_2(action, end, __VA_ARGS__))
#define FOR_EACH_4(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_3(action, end, __VA_ARGS__))
#define FOR_EACH_5(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_4(action, end, __VA_ARGS__))
#define FOR_EACH_6(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_5(action, end, __VA_ARGS__))
#define FOR_EACH_7(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_6(action, end, __VA_ARGS__))
#define FOR_EACH_8(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_7(action, end, __VA_ARGS__))
#define FOR_EACH_9(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_8(action, end, __VA_ARGS__))
#define FOR_EACH_10(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_9(action, end, __VA_ARGS__))
#define FOR_EACH_11(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_10(action, end, __VA_ARGS__))
#define FOR_EACH_12(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_11(action, end, __VA_ARGS__))
#define FOR_EACH_13(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_12(action, end, __VA_ARGS__))
#define FOR_EACH_14(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_13(action, end, __VA_ARGS__))
#define FOR_EACH_15(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_14(action, end, __VA_ARGS__))
#define FOR_EACH_16(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_15(action, end, __VA_ARGS__))
#define FOR_EACH_17(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_16(action, end, __VA_ARGS__))
#define FOR_EACH_18(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_17(action, end, __VA_ARGS__))
#define FOR_EACH_19(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_18(action, end, __VA_ARGS__))
#define FOR_EACH_20(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_19(action, end, __VA_ARGS__))
#define FOR_EACH_21(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_20(action, end, __VA_ARGS__))
#define FOR_EACH_22(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_21(action, end, __VA_ARGS__))
#define FOR_EACH_23(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_22(action, end, __VA_ARGS__))
#define FOR_EACH_24(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_23(action, end, __VA_ARGS__))
#define FOR_EACH_25(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_24(action, end, __VA_ARGS__))
#define FOR_EACH_26(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_25(action, end, __VA_ARGS__))
#define FOR_EACH_27(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_26(action, end, __VA_ARGS__))
#define FOR_EACH_28(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_27(action, end, __VA_ARGS__))
#define FOR_EACH_29(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_28(action, end, __VA_ARGS__))
#define FOR_EACH_30(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_29(action, end, __VA_ARGS__))
#define FOR_EACH_31(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_30(action, end, __VA_ARGS__))
#define FOR_EACH_32(action, end, x, ...) action(x) end() EXPAND(FOR_EACH_31(action, end, __VA_ARGS__))

#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,          \
                  _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                  _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
                  _31, _32, NAME, ...) NAME

#define FOR_EACH(action, end, ...)                                       \
    EXPAND(GET_MACRO(__VA_ARGS__ __VA_OPT__(, )                          \
                         FOR_EACH_32,                                    \
                     FOR_EACH_31, FOR_EACH_30, FOR_EACH_29,              \
                     FOR_EACH_28, FOR_EACH_27, FOR_EACH_26, FOR_EACH_25, \
                     FOR_EACH_24, FOR_EACH_23, FOR_EACH_22, FOR_EACH_21, \
                     FOR_EACH_20, FOR_EACH_19, FOR_EACH_18, FOR_EACH_17, \
                     FOR_EACH_16, FOR_EACH_15, FOR_EACH_14, FOR_EACH_13, \
                     FOR_EACH_12, FOR_EACH_11, FOR_EACH_10, FOR_EACH_9,  \
                     FOR_EACH_8, FOR_EACH_7, FOR_EACH_6, FOR_EACH_5,     \
                     FOR_EACH_4, FOR_EACH_3, FOR_EACH_2, FOR_EACH_1,     \
                     FOR_EACH_0)(action, end, __VA_ARGS__))

// #define TO_STRING_LIST(...) FOR_EACH(STRINGIZE, COMMA_END, __VA_ARGS__)

constexpr bool str_equal(const char *a, const char *b)
{
    for (; *a && *b; ++a, ++b)
    {
        if (*a != *b)
            return false;
    }
    return *a == *b;
}

constexpr bool starts_with(const char *str, const char *prefix)
{
    while (*prefix)
    {
        if (*str != *prefix)
            return false;
        ++str;
        ++prefix;
    }
    return true;
}