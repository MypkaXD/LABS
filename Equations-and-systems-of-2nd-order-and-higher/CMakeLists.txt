cmake_minimum_required(VERSION 3.2) # Min version CMake

set(CMAKE_CXX_STANDARD 17)             # Стандарт C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)    # Обязательность установки указанного стандарта (если OFF (по умолчанию), то воспринимается как совет)

project(Equations_and_systems_of_2nd_order_and_higher
	VERSION 1.0
	LANGUAGES C CXX
)

add_executable(Equations_and_systems_of_2nd_order_and_higher ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)

target_include_directories(
  Equations_and_systems_of_2nd_order_and_higher                        # Цель, при сборке которой учитываются указанные пути к заголовочным файлам
  PUBLIC                                # область видимости (PRIVATE, INTERFACE или PUBLIC)
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(Equations_and_systems_of_2nd_order_and_higher
  PUBLIC
    ImGui-SFML::ImGui-SFML
	implot
	ImGui
)