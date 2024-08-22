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
@echo off
set /P library_choice=Compile the executable with: (1 for Static Window Handler, 2 for Shared Window Handler):
if "%library_choice%"=="1" (
    set LINK_TYPE=Static
    set SHARED_ENABLE=OFF
    set STATIC_ENABLE=ON
) else if "%library_choice%"=="2" (
    set LINK_TYPE=Shared
    set SHARED_ENABLE=ON
    set STATIC_ENABLE=OFF
) else (
    echo Invalid library type. Please enter 1 or 2.
    exit /b 1
)
@echo.
echo Selected Window Handler type: %LINK_TYPE%
@echo.
set /P "=Press any key to start compile operations... " <nul & pause >nul & echo(
@echo.

if %LINK_TYPE%==Shared (
    cmake -G %CMAKE_GENERATOR% ^
    -D CMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -D SHARED_EXPORT=%SHARED_ENABLE% ^
    -D STATIC_EXPORT=%STATIC_ENABLE% ^
    -B __build_out__/%LINK_TYPE%/%BUILD_TYPE% .

    cmake --build __build_out__/%LINK_TYPE%/%BUILD_TYPE% --config %BUILD_TYPE% -j8
) else (
    cmake -G %CMAKE_GENERATOR% ^
    -D CMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -D SHARED_EXPORT=%SHARED_ENABLE% ^
    -D STATIC_EXPORT=%STATIC_ENABLE% ^
    -B __build_out__/%LINK_TYPE%/%BUILD_TYPE% .

    cmake --build __build_out__/%LINK_TYPE%/%BUILD_TYPE% --config %BUILD_TYPE% -j8
)

:: end of file
