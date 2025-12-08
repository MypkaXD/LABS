cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME Computer_Vision)

project(${PROJECT_NAME})

SET(IMGUI_INSTALL_DIR "3rdparty/imgui-master" CACHE PATH "Set Dear Imgui directory path")
SET(GLAD_INSTALL_DIR "3rdparty/glad" CACHE PATH "Set GLAD directory path")
SET(GLFW_INSTALL_DIR "3rdparty/glfw-master" CACHE PATH "Set GLFW directory path")

set(OpenCV_DIR "3rdparty/opencv-4.12.0/build")

find_package(OpenCV 4.12.0 REQUIRED
    COMPONENTS world
)

add_library(3rdparty STATIC
#Dear ImGui
	${IMGUI_INSTALL_DIR}/imgui.cpp
    ${IMGUI_INSTALL_DIR}/imgui_draw.cpp
    ${IMGUI_INSTALL_DIR}/imgui_widgets.cpp
    ${IMGUI_INSTALL_DIR}/imgui_tables.cpp
    ${IMGUI_INSTALL_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_INSTALL_DIR}/backends/imgui_impl_opengl3.cpp
#GLAD
	${GLAD_INSTALL_DIR}/src/glad.c
)

target_include_directories(3rdparty PUBLIC
#Dear ImGui
	${IMGUI_INSTALL_DIR}
	${IMGUI_INSTALL_DIR}/backends
#GLFW
	${GLFW_INSTALL_DIR}/include
#GLAD
	${GLAD_INSTALL_DIR}/include
#OPENCV
	${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(3rdparty PUBLIC 
	${GLFW_INSTALL_DIR}/lib/glfw3.lib
	${OpenCV_LIBS}
)

add_subdirectory(Lesson_1)
add_subdirectory(Lesson_2)