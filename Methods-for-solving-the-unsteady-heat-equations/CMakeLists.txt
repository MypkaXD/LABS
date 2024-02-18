cmake_minimum_required(VERSION 3.2) # Min version CMake

set(CMAKE_CXX_STANDARD 17)             # Стандарт C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)    # Обязательность установки указанного стандарта (если OFF (по умолчанию), то воспринимается как совет)

project(Methods_for_solving_the_unsteady_heat_equation
	VERSION 1.0
	LANGUAGES C CXX
)

add_executable(Methods_for_solving_the_unsteady_heat_equation ${CMAKE_CURRENT_SOURCE_DIR}/src/Methods_for_solving_the_unsteady_heat_equation.cpp)

target_include_directories(
  Methods_for_solving_the_unsteady_heat_equation                           # Цель, при сборке которой учитываются указанные пути к заголовочным файлам
  PUBLIC                                # область видимости (PRIVATE, INTERFACE или PUBLIC)
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
