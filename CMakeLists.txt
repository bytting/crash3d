
cmake_minimum_required(VERSION 2.6)

project(crash3d)

set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
# set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one
set(OGRE_OVERLAY_INCLUDEPATH "/usr/include/OGRE/Overlay/")


if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

find_package(OGRE REQUIRED)
if(NOT OGRE_FOUND)
        message(SEND_ERROR "Failed to find OGRE")
endif()

find_package(OIS REQUIRED)
if(NOT OIS_FOUND)
        message(SEND_ERROR "Failed to find OIS")
endif()

# Find Boost
# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})


set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
# Components that need linking (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
if (NOT Boost_FOUND)
        # Try again with the other type of libs
        set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()
find_package(Boost QUIET)

# Set up referencing of Boost
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL_NO_LIB)
set(OGRE_LIBRARIES OgreOverlay ${OGRE_LIBRARIES} ${Boost_LIBRARIES})

set(SRCS ./crash3d.cpp ./grid.cpp ./spectrum.cpp ./session.cpp ./main.cpp)

include_directories(${OIS_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS} ${OGRE_OVERLAY_INCLUDEPATH})

add_executable(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/media)

file(COPY ${CMAKE_SOURCE_DIR}/plugins.cfg ${CMAKE_SOURCE_DIR}/resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY
    ${CMAKE_SOURCE_DIR}/media/SdkTrays.zip
    ${CMAKE_SOURCE_DIR}/media/sphere.mesh
    ${CMAKE_SOURCE_DIR}/media/column.mesh
    ${CMAKE_SOURCE_DIR}/media/basic_colors.material
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/media)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        CONFIGURATIONS All)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/media
        DESTINATION ./
        CONFIGURATIONS Release RelWithDebInfo Debug
)

install(FILES ${CMAKE_SOURCE_DIR}/bin/plugins.cfg
        ${CMAKE_SOURCE_DIR}/bin/resources.cfg
        DESTINATION bin
        CONFIGURATIONS Release RelWithDebInfo Debug
)
