#!/bin/bash
echo "==========================================="
echo "AxonEngine Build Inditasa (Linux)"
echo "==========================================="

if [ ! -f "External/vcpkg/vcpkg" ]; then
    echo "[0/2] Vcpkg motor nem talalhato. Inicializalas..."
    ./External/vcpkg/bootstrap-vcpkg.sh
    if [ $? -ne 0 ]; then
        echo "[HIBA] Vcpkg inicializalasa sikertelen!"
        exit 1
    fi
fi

echo "[1/2] CMake Konfiguralas es Vcpkg fuggosegek ellenorzese..."
cmake --preset linux-static

if [ $? -ne 0 ]; then
    echo "[HIBA] A CMake konfiguralas elszallt!"
    exit 1
fi

echo "[2/2] AxonEngine forditasa..."
cmake --build build --config Release

if [ $? -eq 0 ]; then
    echo "==========================================="
    echo "SIKERES BUILD! A futtathato fajl itt: bin/AxonEngine"
    echo "==========================================="
else
    echo "[HIBA] A forditas sikertelen!"
fi