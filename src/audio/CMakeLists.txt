# Include SDL headers
IF(USE_SDL2)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
ELSE(USE_SDL2)
    INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR})
ENDIF(USE_SDL2)

ADD_LIBRARY( 
    audio STATIC 
    AudioManager.cpp
)

# Link to the following libraries
IF(USE_SDL2)
    TARGET_LINK_LIBRARIES(
        audio
        core
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_LIBRARIES}
    )
ELSE(USE_SDL2)
    TARGET_LINK_LIBRARIES(
        audio
        core
        ${SDLMIXER_LIBRARY}
        ${SDL_LIBRARY}
    )
ENDIF(USE_SDL2)
