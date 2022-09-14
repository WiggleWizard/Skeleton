include_guard()

cmake_minimum_required(VERSION 3.2)
project(implot)

CPMAddPackage(
    NAME implot
    GITHUB_REPOSITORY epezent/implot
    VERSION 0.13)

set(IMPLOT_SOURCES
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)

# build as a static library
add_library(${PROJECT_NAME} STATIC ${IMPLOT_SOURCES})

# include libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${implot_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        imgui
        glfw) # Mainly for auto include dirs
