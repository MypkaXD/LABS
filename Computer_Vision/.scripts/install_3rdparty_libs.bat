@echo off

set INSTALL_DIR=../3rdparty

setlocal enabledelayedexpansion

set DOWNLOAD_URL[0]=https://github.com/glfw/glfw/archive/refs/heads/master.zip
set DOWNLOAD_URL[1]=https://github.com/opencv/opencv/archive/refs/tags/4.12.0.zip
set DOWNLOAD_URL[2]=https://github.com/ocornut/imgui/archive/refs/heads/master.zip

set ZIP_NAME[0]=glfw-master.zip
set ZIP_NAME[1]=opencv-4.12.0.zip
set ZIP_NAME[2]=imgui-master.zip

set FOLDER_NAME[0]=glfw-master
set FOLDER_NAME[1]=opencv-4.12.0
set FOLDER_NAME[2]=imgui-master

for /l %%i in (0,1,2) do (
	echo Start install !FOLDER_NAME[%%i]!
	echo Downloading !FOLDER_NAME[%%i]! from !DOWNLOAD_URL[%%i]!
	powershell -Command "(New-Object Net.WebClient).DownloadFile('!DOWNLOAD_URL[%%i]!', '!ZIP_NAME[%%i]!')"
	echo Extracting archive !ZIP_NAME[%%i]!
	powershell -Command "Expand-Archive -Path '!ZIP_NAME[%%i]!' -DestinationPath '%INSTALL_DIR%'"
	echo Cleaning up
	del !ZIP_NAME[%%i]!
	echo Creating folders
	if not exist "%INSTALL_DIR%/!FOLDER_NAME[%%i]!/build" mkdir "%INSTALL_DIR%/!FOLDER_NAME[%%i]!/build"
	if not exist "%INSTALL_DIR%/!FOLDER_NAME[%%i]!/install" mkdir "%INSTALL_DIR%/!FOLDER_NAME[%%i]!/install"
)

echo Configuration CMake for %FOLDER_NAME[0]%
cmake -S "%INSTALL_DIR%/%FOLDER_NAME[0]%" ^
	-B "%INSTALL_DIR%/%FOLDER_NAME[0]%/build" ^
	-G "Visual Studio 17 2022" -A x64 ^
	-DBUILD_SHARED_LIBS=OFF ^
	-DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%/%FOLDER_NAME[0]%/install" ^
	-DGLFW_BUILD_DOCS=OFF ^
	-DGLFW_BUILD_EXAMPLES=OFF ^
	-DGLFW_BUILD_TESTS=OFF ^
	-DGLFW_BUILD_WIN32=ON ^
	-DGLFW_INSTALL=ON ^
	-DGLFW_USE_HYBRID_HPG=OFF ^
	-USE_MSVC_RUNTIME_LIBRARY_DLL=OFF ^
	-DCMAKE_BUILD_TYPE=Release
echo [6/6] Building %FOLDER_NAME[0]%
cmake --build "%INSTALL_DIR%/%FOLDER_NAME[0]%/build" --config Release --target install


echo Configuration CMake %FOLDER_NAME[1]%
cmake -S "%INSTALL_DIR%/%FOLDER_NAME[1]%" ^
	-B "%INSTALL_DIR%/%FOLDER_NAME[1]%/build" ^
	-G "Visual Studio 17 2022" -A x64 ^
	-DBUILD_TESTS=OFF ^
	-DBUILD_EXAMPLES=OFF ^
	-DBUILD_SHARED_LIBS=OFF ^
	-DBUILD_PERF_TESTS=OFF ^
	-DBUILD_opencv_world=ON	
echo Building OPENCV
cmake --build "%INSTALL_DIR%/%FOLDER_NAME[1]%/build" --config Release
echo Installing OPENCV
cmake --build "%INSTALL_DIR%/%FOLDER_NAME[1]%/build" --config Release --target INSTALL

pause