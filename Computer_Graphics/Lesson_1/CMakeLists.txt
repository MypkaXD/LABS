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

add_executable(First examples/src/example_first.cpp)
target_link_libraries(First ${VTK_LIBRARIES})

add_executable(ImageData examples/src/example_imagedata.cpp)
target_link_libraries(ImageData ${VTK_LIBRARIES})

add_executable(StructuredGrid examples/src/example_structuredgrid.cpp)
target_link_libraries(StructuredGrid ${VTK_LIBRARIES})

add_executable(PolyData examples/src/example_polydata.cpp)
target_link_libraries(PolyData ${VTK_LIBRARIES})

add_executable(ActorProperties examples/src/example_actorproperties.cpp)
target_link_libraries(ActorProperties ${VTK_LIBRARIES})

add_executable(ConeLight examples/src/example_conelight.cpp)
target_link_libraries(ConeLight ${VTK_LIBRARIES})

add_executable(TrackballCamera examples/src/example_trackballcamera.cpp)
target_link_libraries(TrackballCamera ${VTK_LIBRARIES})

add_executable(Save3dArray examples/src/example_save3darray.cpp)
target_link_libraries(Save3dArray ${VTK_LIBRARIES})

add_executable(ObjReader examples/src/example_objreader.cpp)
target_link_libraries(ObjReader ${VTK_LIBRARIES})

add_executable(ContourFilter examples/src/example_contourfilter.cpp)
target_link_libraries(ContourFilter ${VTK_LIBRARIES})

add_executable(SmoothPolyFilter examples/src/example_smoothpolyfilter.cpp)
target_link_libraries(SmoothPolyFilter ${VTK_LIBRARIES})

add_executable(GaussFilter examples/src/example_gaussfilter.cpp)
target_link_libraries(GaussFilter ${VTK_LIBRARIES})

add_executable(CutterFilter examples/src/example_cutterfilter.cpp)
target_link_libraries(CutterFilter ${VTK_LIBRARIES})

add_executable(ClipData examples/src/example_clipdata.cpp)
target_link_libraries(ClipData ${VTK_LIBRARIES})

add_executable(ScalarBar examples/src/example_scalarbar.cpp)
target_link_libraries(ScalarBar ${VTK_LIBRARIES})

add_executable(Historgam examples/src/example_histogram.cpp)
target_link_libraries(Historgam ${VTK_LIBRARIES})

add_executable(LookupTable examples/src/example_lookuptable.cpp)
target_link_libraries(LookupTable ${VTK_LIBRARIES})

add_executable(VolumeRender examples/src/example_volumerender.cpp)
target_link_libraries(VolumeRender ${VTK_LIBRARIES})

add_executable(PlainCutWidget examples/src/example_planecutwidget.cpp)
target_link_libraries(PlainCutWidget ${VTK_LIBRARIES})

add_executable(VectorField examples/src/example_vectorfield.cpp)
target_link_libraries(VectorField ${VTK_LIBRARIES})

add_executable(TetraMesh examples/src/example_tetramesh.cpp)
target_link_libraries(TetraMesh ${VTK_LIBRARIES})