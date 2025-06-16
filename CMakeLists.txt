##########################
# GENERAL OPTIONS
##########################
# We need 2.6 because cmake has freetype2 support from then onwards.
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(JVGS)

# Add debug symbols
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

##########################
# SDL2 (required)
##########################
FIND_PACKAGE(SDL2 REQUIRED)
MESSAGE( STATUS "SDL2 has been found:" )
MESSAGE( STATUS "\tCFLAGS : ${SDL2_INCLUDE_DIRS}" )
MESSAGE( STATUS "\tLDFLAGS: ${SDL2_LIBRARIES}" )

##########################
# SDL2_mixer (required)
##########################
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(SDL2_MIXER REQUIRED SDL2_mixer)
MESSAGE( STATUS "SDL2_mixer has been found:" )
MESSAGE( STATUS "\tCFLAGS : ${SDL2_MIXER_INCLUDE_DIRS}" )
MESSAGE( STATUS "\tLDFLAGS: ${SDL2_MIXER_LIBRARIES}" )

# OpenGL removed - using SDL2 renderer instead

#######################
# Lua
#######################
FIND_PACKAGE(Lua51)
IF(LUA51_FOUND)
    MESSAGE( STATUS "Lua has been found:" )
    MESSAGE( STATUS "\tCFLAGS : ${LUA_INCLUDE_DIR}" )
    MESSAGE( STATUS "\tLDFLAGS: ${LUA_LIBRARIES}" )
ELSE(LUA51_FOUND)
    SET( LUA_INCLUDE_DIR NOTFOUND )
    SET( LUA_LIBRARIES NOTFOUND )
    MESSAGE( STATUS "Lua is required" )
ENDIF(LUA51_FOUND)

#######################
# zlib
#######################
FIND_PACKAGE(ZLIB)
IF(ZLIB_FOUND)
    MESSAGE( STATUS "zlib has been found:" )
    MESSAGE( STATUS "\tCFLAGS : ${ZLIB_INCLUDE_DIRS}" )
    MESSAGE( STATUS "\tLDFLAGS: ${ZLIB_LIBRARIES}" )
ELSE(ZLIB_FOUND)
    SET( ZLIB_INCLUDE_DIRS NOTFOUND )
    SET( ZLIB_LIBRARIES NOTFOUND )
    MESSAGE( STATUS "zlib is required")
ENDIF(ZLIB_FOUND)

#######################
# Freetype
#######################
FIND_PACKAGE(Freetype)
IF(FREETYPE_INCLUDE_DIRS)
    MESSAGE( STATUS "Freetype has been found:" )
    MESSAGE( STATUS "\tCFLAGS : ${FREETYPE_INCLUDE_DIRS}" )
    MESSAGE( STATUS "\tLDFLAGS: ${FREETYPE_LIBRARIES}" )
ELSE(FREETYPE_INCLUDE_DIRS)
    SET( FREETYPE_INCLUDE_DIRS NOTFOUND )
    SET( FREETYPE_LIBRARIES NOTFOUND )
    MESSAGE( STATUS "Freetype is required" )
ENDIF(FREETYPE_INCLUDE_DIRS)

#######################
# Swig
#######################
FIND_PACKAGE(SWIG)
IF(SWIG_FOUND)
    MESSAGE( STATUS "SWIG has been found:" )
    MESSAGE( STATUS "\tSWIG : ${SWIG_EXECUTABLE}" )
ELSE(SWIG_FOUND)
    MESSAGE( STATUS "SWIG is required" )
ENDIF(SWIG_FOUND)

# Always using SDL2 now
ADD_DEFINITIONS(-DUSE_SDL2)

# Continue for subdirectories.
ADD_SUBDIRECTORY(src)

# Documentation
ADD_CUSTOM_TARGET(
    doxy
    doxygen Doxyfile
)
