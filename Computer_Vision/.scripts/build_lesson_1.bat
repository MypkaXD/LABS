@echo off

set IMGUI_INSTALL_DIR="%cd%/../3rdparty/imgui-master/"
set GLFW_INSTALL_DIR="%cd%/../3rdparty/glfw-master/install/"
set GLAD_INSTALL_DIR="%cd%/../3rdparty/glad/"
set INSTALL_DIR=%cd%/../

set count_of_step=2

echo [1/%count_of_step%] Creating folders...
if not exist "%INSTALL_DIR%/install" mkdir "%INSTALL_DIR%/install"
if not exist "%INSTALL_DIR%/build" mkdir "%INSTALL_DIR%/build"

echo [2/%count_of_step%] CMake...
cmake -S "%INSTALL_DIR%" ^
	-B "%INSTALL_DIR%/build" ^
	-G "Visual Studio 17 2022" -A x64 ^
	-DIMGUI_INSTALL_DIR=%IMGUI_INSTALL_DIR% ^
	-DGLAD_INSTALL_DIR=%GLAD_INSTALL_DIR% ^
	-DGLFW_INSTALL_DIR=%GLFW_INSTALL_DIR%
	
pause