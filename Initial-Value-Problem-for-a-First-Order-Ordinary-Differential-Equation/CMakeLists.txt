cmake_minimum_required(VERSION 3.2) # Min version CMake

set(CMAKE_CXX_STANDARD 17)             # Стандарт C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)    # Обязательность установки указанного стандарта (если OFF (по умолчанию), то воспринимается как совет)

project(First_Order_ODE_Initial_Value_Problem
	VERSION 1.0
	LANGUAGES C CXX
)

add_executable(First_Order_ODE_Initial_Value_Problem ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)

target_include_directories(
  First_Order_ODE_Initial_Value_Problem                        # Цель, при сборке которой учитываются указанные пути к заголовочным файлам
  PUBLIC                                # область видимости (PRIVATE, INTERFACE или PUBLIC)
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(First_Order_ODE_Initial_Value_Problem
  PUBLIC
    ImGui-SFML::ImGui-SFML
	implot
	ImGui
)