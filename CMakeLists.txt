cmake_minimum_required(VERSION 3.20)

cmake_policy(SET CMP0091 NEW)

project(Runner LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(RUNNER_CONFIGS Debug DebugAsan Release)

get_property(_is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_is_multi_config)
    set(CMAKE_CONFIGURATION_TYPES ${RUNNER_CONFIGS} CACHE STRING "" FORCE)
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${RUNNER_CONFIGS})
endif()

foreach(_lang C CXX)
    set(CMAKE_${_lang}_FLAGS_DEBUGASAN "${CMAKE_${_lang}_FLAGS_DEBUG}")
endforeach()
set(CMAKE_EXE_LINKER_FLAGS_DEBUGASAN "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUGASAN "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
set(CMAKE_STATIC_LINKER_FLAGS_DEBUGASAN "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}")

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Runner only supports x64 builds.")
endif()

set(RUNNER_CLANG_CL FALSE)
if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(RUNNER_CLANG_CL TRUE)

    execute_process(
        COMMAND "${CMAKE_CXX_COMPILER}" -print-resource-dir
        OUTPUT_VARIABLE CLANG_RESOURCE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CLANG_ASAN_RUNTIME_LIB_DIR "${CLANG_RESOURCE_DIR}/lib/windows")
endif()

if(RUNNER_CLANG_CL)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug,DebugAsan>:Debug>DLL")
endif()

option(RUNNER_UNITY_BUILD "Batch each target's sources into unity build translation units" ON)
set(CMAKE_UNITY_BUILD ${RUNNER_UNITY_BUILD})
set(CMAKE_UNITY_BUILD_BATCH_SIZE 10)

# SDL2/freetype/ogg/vorbis/OpenAL are resolved via vcpkg (see vcpkg.json).
# guyframework's own extlibs/ only ships MinGW-format archives (libs-mingw/*.a),
# which aren't usable here; configure with -DCMAKE_TOOLCHAIN_FILE pointing at
# vcpkg's scripts/buildsystems/vcpkg.cmake (the CMakePresets already do this
# via the VCPKG_ROOT environment variable).
find_package(SDL2 CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(Ogg CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(OpenAL REQUIRED)

set(RUNNER_MSVC_WARNING_FLAGS
    /W4
    /wd4061 /wd4100 /wd4250 /wd4255 /wd4266 /wd4324 /wd4342 /wd4347 /wd4350
    /wd4355 /wd4365 /wd4371 /wd4435 /wd4464 /wd4481 /wd4505 /wd4512 /wd4514
    /wd4548 /wd4571 /wd4574 /wd4582 /wd4583 /wd4592 /wd4599 /wd4619 /wd4623
    /wd4625 /wd4626 /wd4628 /wd4668 /wd4710 /wd4711 /wd4714 /wd4738 /wd4774
    /wd4820 /wd4917 /wd4986 /wd4996 /wd5026 /wd5027 /wd5038 /wd5045
    /wd4018 /wd4189 /wd4244 /wd4245 /wd4267 /wd4310 /wd4456 /wd4457 /wd4458 /wd4477 /wd4706 /wd4834 /wd4005
    /wd4068 /wd4290 /wd4302 /wd4311 /wd4312 /wd4702 /wd4459
)
set(RUNNER_CLANG_WARNING_FLAGS
    -Wall -Wextra -Wunreachable-code -Wshorten-64-to-32 -Wfloat-conversion
    -Wno-unused-parameter -Wno-reorder -Wno-invalid-source-encoding
    -Wno-deprecated-register -Wno-missing-braces -Wno-unused-local-typedef
    -Wno-c++11-extensions -Wno-unknown-warning-option -Wno-extern-c-compat
    -Wno-range-loop-analysis -Wno-cast-function-type-mismatch
    -Wno-error=constant-conversion -Wno-deprecated-declarations
    -Wno-unused-function -Wno-unused-private-field
    -Wno-error=sign-compare -Wno-error=unused-private-field -Wno-error=unused-function
    -Wno-reserved-macro-identifier -Wno-reserved-identifier -Wno-c++98-compat-pedantic
    -Wno-sign-conversion -Wno-old-style-cast -Wno-cast-align
    -Wno-nonportable-system-include-path -Wno-language-extension-token
    -Wno-gnu-zero-variadic-macro-arguments
    -Wno-error=c++20-extensions -Wno-error=cast-function-type-strict -Wno-error=cast-qual
    -Wno-error=comma -Wno-error=covered-switch-default
    -Wno-error=deprecated-copy-with-user-provided-copy -Wno-error=deprecated-copy-with-user-provided-dtor
    -Wno-error=documentation -Wno-error=documentation-deprecated-sync
    -Wno-error=documentation-pedantic -Wno-error=documentation-unknown-command
    -Wno-error=double-promotion -Wno-error=exit-time-destructors
    -Wno-error=extra-semi -Wno-error=extra-semi-stmt -Wno-error=float-conversion
    -Wno-error=float-equal -Wno-error=format -Wno-error=format-non-iso -Wno-error=format-security
    -Wno-error=global-constructors -Wno-header-hygiene -Wno-error=implicit-fallthrough
    -Wno-error=implicit-float-conversion -Wno-error=implicit-int-conversion
    -Wno-error=implicit-int-float-conversion -Wno-error=invalid-utf8
    -Wno-error=microsoft-unqualified-friend -Wno-error=misleading-indentation
    -Wno-error=missing-field-initializers -Wno-error=missing-noreturn
    -Wno-error=missing-prototypes -Wno-error=missing-variable-declarations
    -Wno-error=newline-eof -Wno-error=pointer-bool-conversion -Wno-error=shadow
    -Wno-error=shadow-field-in-constructor -Wno-error=shorten-64-to-32
    -Wno-error=suggest-destructor-override -Wno-error=suggest-override
    -Wno-error=switch-default -Wno-error=switch-enum
    -Wno-error=tautological-type-limit-compare -Wno-error=tautological-unsigned-zero-compare
    -Wno-error=undef -Wno-error=unsafe-buffer-usage -Wno-error=unused-but-set-variable
    -Wno-error=unused-variable -Wno-error=writable-strings -Wno-error=zero-as-null-pointer-constant
)

add_library(runner_warnings INTERFACE)
add_library(runner_warnings_as_errors INTERFACE)
target_link_libraries(runner_warnings_as_errors INTERFACE runner_warnings)
if(MSVC AND NOT RUNNER_CLANG_CL)
    target_compile_options(runner_warnings INTERFACE ${RUNNER_MSVC_WARNING_FLAGS})
    target_compile_options(runner_warnings_as_errors INTERFACE $<$<COMPILE_LANGUAGE:CXX>:/WX>)
else()
    target_compile_options(runner_warnings INTERFACE ${RUNNER_CLANG_WARNING_FLAGS})
    target_compile_options(runner_warnings_as_errors INTERFACE $<$<COMPILE_LANGUAGE:CXX>:-Werror>)
endif()
add_library(Runner::Warnings ALIAS runner_warnings)
add_library(Runner::WarningsAsErrors ALIAS runner_warnings_as_errors)

add_subdirectory(guyframework)

add_executable(runner WIN32
    runner.cpp
    action.cpp
    actionsrunner.cpp
    logic/tilemapcharacter.cpp
    logic/tilemap.cpp
    logic/player.cpp
    logic/runnertilemap.cpp
    logic/generator/runnermapchunk.cpp
    graphics/sprite.cpp
    graphics/spriteanim.cpp
    graphics/spritestatic.cpp
)

target_include_directories(runner PRIVATE
    ${CMAKE_SOURCE_DIR}
    # sprite.cpp/spriteanim.cpp/spritestatic.cpp use same-directory-relative
    # includes (e.g. "texturemanager.h") left over from when they lived
    # inside guyframework/graphics/, before being restored into runner/graphics/.
    ${CMAKE_SOURCE_DIR}/guyframework/graphics
)

target_compile_definitions(runner PRIVATE
    $<$<CONFIG:Debug,DebugAsan>:_DEBUG>
)

target_link_libraries(runner PRIVATE
    GuyFramework
    Runner::WarningsAsErrors
)

if(MSVC AND NOT RUNNER_CLANG_CL)
    target_compile_options(runner PRIVATE /sdl)
endif()

if(MSVC AND NOT RUNNER_CLANG_CL)
    target_compile_options(runner PRIVATE $<$<CONFIG:DebugAsan>:/fsanitize=address>)
elseif(RUNNER_CLANG_CL)
    target_compile_options(runner PRIVATE $<$<CONFIG:DebugAsan>:-fsanitize=address>)
    target_link_options(runner PRIVATE $<$<CONFIG:DebugAsan>:-fsanitize=address>)

    if(CMAKE_BUILD_TYPE STREQUAL "DebugAsan")
        target_link_directories(runner PRIVATE ${CLANG_ASAN_RUNTIME_LIB_DIR})
        target_link_libraries(runner PRIVATE
            clang_rt.asan_dynamic-x86_64.lib
            clang_rt.asan_dynamic_runtime_thunk-x86_64.lib
        )
        add_custom_command(TARGET runner POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CLANG_ASAN_RUNTIME_LIB_DIR}/clang_rt.asan_dynamic-x86_64.dll"
                "$<TARGET_FILE_DIR:runner>/clang_rt.asan_dynamic-x86_64.dll"
        )
    endif()
endif()

