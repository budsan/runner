add_library(GuyFramework STATIC
    environment.cpp
    game.cpp
    persistencelayer.cpp
    variable.cpp
    graphics/screen.cpp
    graphics/texture.cpp
    graphics/texturemanager.cpp
    graphics/shaderprogram.cpp
    graphics/shader.cpp
    graphics/framebuffer.cpp
    graphics/camera2d.cpp
    graphics/primitives.cpp
    graphics/color.cpp
    graphics/mesh.cpp
    graphics/vertexattribute.cpp
    graphics/graphics.cpp
    graphics/model.cpp
    graphics/effect.cpp
    graphics/renderstate.cpp
    input/input.cpp
    input/keyboard.cpp
    input/gamepad.cpp
    input/controller.cpp
    tools/transition.cpp
    math/hit.cpp
    audio/emyl.cpp
    graphics/tools/frameshud.cpp
    graphics/tools/texthud.cpp
    graphics/font.cpp
    tools/particleemitter.cpp
    tools/particle.cpp
    deps/sdl/sdlscreen.cpp
    deps/sdl/sdlinput.cpp
    deps/sdl/sdlenvironment.cpp
    deps/win/winmain.cpp
    deps/win/winenvironment.cpp
    deps/win/winlog.cpp
    deps/win/winpersistencelayer.cpp
    deps/win/glew.c
)

target_include_directories(GuyFramework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENAL_INCLUDE_DIR}
    # GL/glew.h: glew.c is vendored source (not a vcpkg dependency). The
    # other vendored headers (SDL2/AL/ogg/vorbis/freetype) were pruned from
    # extlibs/include since vcpkg now supplies those.
    ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/include
)

target_link_libraries(GuyFramework PUBLIC
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    SDL2::SDL2main
    freetype
    Ogg::ogg
    Vorbis::vorbis
    Vorbis::vorbisenc
    Vorbis::vorbisfile
    opengl32
    glu32
    ${OPENAL_LIBRARY}
    Runner::WarningsAsErrors
)

target_compile_definitions(GuyFramework PUBLIC
    GUY_USE_WINDOWS
    GLEW_STATIC
)

if(MSVC AND NOT RUNNER_CLANG_CL)
    target_compile_options(GuyFramework PRIVATE /sdl)
endif()

if(MSVC)
    target_compile_options(GuyFramework PRIVATE $<$<CONFIG:DebugAsan>:$<IF:$<BOOL:${RUNNER_CLANG_CL}>,-fsanitize=address,/fsanitize=address>>)
endif()
