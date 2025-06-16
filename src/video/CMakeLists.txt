# SDL2-only build
INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)
SET(VIDEO_SOURCES
    Color.cpp
    DataRenderer.cpp
    FPSLogger.cpp
    Renderer.cpp
    VideoManager.cpp
    SketchyRenderer.cpp
)
SET(VIDEO_LIBS
    math
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

ADD_LIBRARY(video STATIC ${VIDEO_SOURCES})
TARGET_LINK_LIBRARIES(video ${VIDEO_LIBS})
