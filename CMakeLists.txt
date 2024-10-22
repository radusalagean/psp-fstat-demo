cmake_minimum_required(VERSION 3.22.1)

project(pspfstatdemo)

set(SRC_DIR "${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE SOURCES "main.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ~/pspdev/psp/sdk/include/)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspdebug
    pspdisplay
    pspge
)

create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE "PSP fstat demo"
    BUILD_PRX 1
)
