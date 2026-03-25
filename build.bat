@echo off
echo ===========================================
echo AxonEngine Build Inditasa (Windows Static)
echo ===========================================

REM 
if not exist "External\vcpkg\vcpkg.exe" (
    echo [0/2] Vcpkg motor nem talalhato. Inicializalas...
    call External\vcpkg\bootstrap-vcpkg.bat
    if %ERRORLEVEL% NEQ 0 (
        echo [HIBA] Vcpkg inicializalasa sikertelen!
        pause
        exit /b %ERRORLEVEL%
    )
)

REM 
echo [1/2] CMake Konfiguralas es Vcpkg fuggosegek ellenorzese...
cmake --preset windows-static

if %ERRORLEVEL% NEQ 0 (
    echo [HIBA] A CMake konfiguralas elszallt!
    pause
    exit /b %ERRORLEVEL%
)

REM 
echo [2/2] AxonEngine forditasa...
cmake --build build --config Release

if %ERRORLEVEL% EQU 0 (
    echo ===========================================
    echo SIKERES BUILD! A futtathato fajl itt: bin\Release\AxonEngine.exe
    echo ===========================================
) else (
    echo [HIBA] A forditas sikertelen!
)

pause