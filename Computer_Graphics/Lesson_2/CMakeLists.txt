cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME Computer_Graphics)

project(${PROJECT_NAME})

find_package(ITK 
	PATHS "C:/dev/Source/LABS/Computer_Graphics/itk/ITK/install/lib/cmak/ITK-6.0"
	NO_DEFAULT_PATH
)
include(${ITK_USE_FILE})

find_package(VTK 
	PATHS "C:/dev/Source/LABS/Computer_Graphics/vtk/vtk/install/lib/cmake/vtk-9.5"
	NO_DEFAULT_PATH
)
include(${VTK_USE_FILE})

set(SOURCE_EXE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(Lab_2 ${SOURCE_EXE})

target_link_libraries(Lab_2 ${VTK_LIBRARIES} ${ITK_LIBRARIES})

add_subdirectory(examples)