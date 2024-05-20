cmake_minimum_required(VERSION 3.2) # Min version CMake

set(CMAKE_CXX_STANDARD 17)             # Стандарт C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)    # Обязательность установки указанного стандарта (если OFF (по умолчанию), то воспринимается как совет)

project(Finite_element_method
	VERSION 1.0
	LANGUAGES C CXX
)

add_executable(Finite_element_method ${CMAKE_CURRENT_SOURCE_DIR}/source/Finite_element_method.cpp)