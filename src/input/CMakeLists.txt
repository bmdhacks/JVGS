# Set up include directories.
IF(USE_SDL2)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
ELSE(USE_SDL2)
    INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
ENDIF(USE_SDL2)

ADD_LIBRARY( 
    input STATIC 
    InputConfiguration.cpp
    InputManager.cpp
)

# Link to the following libraries
TARGET_LINK_LIBRARIES(
    input
    core
)
