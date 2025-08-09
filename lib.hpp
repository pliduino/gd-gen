#pragma once

#include "core/gclass/macros.hpp"
#include "core/genum/macros.hpp"
#include "core/gfunction/macros.hpp"
#include "core/gproperty/macros.hpp"
#include "core/gsignal/macros.hpp"
#include "core/gstruct/macros.hpp"
#include "macro_helpers.hpp"

#define GODOT_GENERATOR_GLUE_(A, B) A##_##B
#define GODOT_GENERATOR_GLUE(A, B) GODOT_GENERATOR_GLUE_(A, B)

#ifdef GODOT_GENERATOR_EXPAND_MACROS
#define GENERATED_BODY() \
    GODOT_GENERATOR_GLUE(FILE_IDENTIFIER, GODOT_GENERATOR_GLUE(__LINE__, GENERATED_BODY()))
#else
#define GENERATED_BODY() \
    GODOT_GENERATOR_GLUE(FILE_IDENTIFIER, GODOT_GENERATOR_GLUE(__LINE__, CORE_GENERATED_BODY()))
#endif
