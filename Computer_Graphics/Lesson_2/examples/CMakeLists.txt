set(EXAMPLE_SRC_1 ${CMAKE_CURRENT_SOURCE_DIR}/src/example_first.cpp)
set(EXAMPLE_SRC_2 ${CMAKE_CURRENT_SOURCE_DIR}/src/example_itkvtkdicom.cpp)

add_executable(Example_1 ${EXAMPLE_SRC_1})
add_executable(Example_2 ${EXAMPLE_SRC_2})

target_link_libraries(Example_1 ${VTK_LIBRARIES} ${ITK_LIBRARIES})
target_link_libraries(Example_2 ${VTK_LIBRARIES} ${ITK_LIBRARIES})