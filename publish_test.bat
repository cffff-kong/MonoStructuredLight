rmdir /s /q build

cmake --preset VS2022_x64-Release
cmake --build --preset Release-x64

cmake --preset VS2022_x64-Debug
cmake --build .\\build\\x64\\ --preset Debug-x64

