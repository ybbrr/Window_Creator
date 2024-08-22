@if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
)

@echo.
@echo off
set /P build_choice=Choose build type (1 for Release, 2 for Debug):
if "%build_choice%"=="1" (
    set BUILD_TYPE=Release
) else if "%build_choice%"=="2" (
    set BUILD_TYPE=Debug
) else (
    echo Invalid build type choice. Please enter 1 or 2.
    exit /b 1
)
@echo.
echo Selected build type: %BUILD_TYPE%
@echo.
@echo.
@echo off
set /P generator_choice=Choose CMake Generator (1 for Ninja, 2 for Visual Studio 17 2022):
if "%generator_choice%"=="1" (
    set CMAKE_GENERATOR=Ninja
) else if "%generator_choice%"=="2" (
    set CMAKE_GENERATOR="Visual Studio 17 2022"
) else (
    echo Invalid CMake Generator choice. Please enter 1 or 2.
    exit /b 1
)
@echo.
echo Selected CMake Generator: %CMAKE_GENERATOR%
@echo.


@echo.
set /P "=Press any key to start compile operations... " <nul & pause >nul & echo(
@echo.

set /A NUM_THREADS=%NUMBER_OF_PROCESSORS% - 2

cmake -G %CMAKE_GENERATOR% ^
    -D CMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -D SHARED_EXPORT=OFF ^
    -D STATIC_EXPORT=ON ^
    -D BUILD_EXAMPLE=ON ^
    -D CMAKE_INSTALL_PREFIX=./__build_out__/%BUILD_TYPE%/example_exe ^
    -B __build_dir__/%BUILD_TYPE% .

@if %CMAKE_GENERATOR%==Ninja (
    cmake --build __build_dir__/%BUILD_TYPE% --config %BUILD_TYPE% -j%NUM_THREADS%
    cmake --build __build_dir__/%BUILD_TYPE% -- install
) else (
    cmake --build __build_dir__/%BUILD_TYPE% --config %BUILD_TYPE% --target INSTALL -j%NUM_THREADS%
)

:: end of file
