# Include SDL headers for TimeManager
IF(USE_SDL2)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
ELSE(USE_SDL2)
    INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
ENDIF(USE_SDL2)

ADD_LIBRARY( 
    core STATIC 
    LogManager.cpp
    XMLLoadable.cpp
    PersistenceManager.cpp
    PropertyMap.cpp
    TimeManager.cpp
    View.cpp
)
