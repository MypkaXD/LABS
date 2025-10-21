cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME Computer_Graphics)

project(${PROJECT_NAME})

find_package(VTK 
	PATHS "C:\\dev\\Source\\LABS\\Computer_Graphics\\vtk\\vtk\\install\\lib\\cmake\\vtk-9.5"
	NO_DEFAULT_PATH
)
include(${VTK_USE_FILE})

set(SOURCE_EXE main.cpp)

add_executable(Lesson_1 ${SOURCE_EXE})

target_link_libraries(Lesson_1 ${VTK_LIBRARIES})