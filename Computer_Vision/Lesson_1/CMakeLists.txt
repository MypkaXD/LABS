add_executable(Lesson_1 main.cpp)

target_link_libraries(Lesson_1 PUBLIC 3rdparty)

# if(MSVC)
    # target_compile_options(Lesson_1 PRIVATE $<$<CONFIG:Release>:/MT>)
    # target_compile_options(Lesson_1 PRIVATE $<$<CONFIG:Debug>:/MTd>)
# endif()