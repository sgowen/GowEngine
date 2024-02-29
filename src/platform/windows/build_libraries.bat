set "solutionDir=%cd%"

cd %solutionDir%

if EXIST box2d.lib (
    set "box2d=0"
) else (
    set "box2d=1"
)

if EXIST box2d_debug.lib (
    set "box2d=0"
) else (
    set "box2d=1"
)

if EXIST glfw3.lib (
    set "glfw3=0"
) else (
    set "glfw3=1"
)

if EXIST sndfile.lib (
    set "sndfile=0"
) else (
    set "sndfile=1"
)

if EXIST OpenAL32.lib (
    set "OpenAL32=0"
) else (
    set "OpenAL32=1"
)

cd ..\..\..\deps

if "%box2d%"=="1" (
    ECHO building box2d.lib
    cd box2d
    IF EXIST cmake_build @RD /S /Q cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake ..
    cmake --build . --config Release --target box2d
    cmake --build . --config Debug --target box2d
    xcopy bin\Release\box2d.lib %solutionDir%\ /Y
    echo F|xcopy /S /Q /Y /F "bin\Debug\box2d.lib" "%solutionDir%\box2d_debug.lib"
    cd ..
    IF EXIST cmake_build @RD /S /Q cmake_build
    cd ..
) else (
    ECHO box2d.lib already exists
)

if "%glfw3%"=="1" (
    ECHO building glfw3.lib
    cd glfw
    IF EXIST cmake_build @RD /S /Q cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake ..
    cmake --build . --config Release --target glfw
    xcopy src\Release\glfw3.lib %solutionDir%\ /Y
    cd ..
    IF EXIST cmake_build @RD /S /Q cmake_build
    cd ..
) else (
    ECHO glfw3.lib already exists
)

if "%sndfile%"=="1" (
    ECHO building sndfile.lib
    cd libsndfile
    IF EXIST cmake_build @RD /S /Q cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -DBUILD_SHARED_LIBS=1 -DBUILD_TESTING=0
    cmake --build . --config Release --target sndfile
    xcopy Release\sndfile.lib %solutionDir%\ /Y
    xcopy Release\sndfile.dll %solutionDir%\ /Y
    cd ..
    IF EXIST cmake_build @RD /S /Q cmake_build
    cd ..
) else (
    ECHO sndfile.lib already exists
)

if "%OpenAL32%"=="1" (
    ECHO building OpenAL32.lib
    cd openal-soft
    IF EXIST cmake_build @RD /S /Q cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake ..
    cmake --build . --config Release --target OpenAL
    xcopy Release\OpenAL32.lib %solutionDir%\ /Y
    xcopy Release\OpenAL32.dll %solutionDir%\ /Y
    cd ..
    IF EXIST cmake_build @RD /S /Q cmake_build
    cd ..
) else (
    ECHO OpenAL32.lib already exists
)

ECHO copying steam_api64.dll into solution directory
xcopy %solutionDir%\..\..\..\..\GowEngine\deps\steamworks_sdk\redistributable_bin\win64\steam_api64.dll %solutionDir%\ /Y

ECHO copying steam_api64.lib into solution directory
xcopy %solutionDir%\..\..\..\..\GowEngine\deps\steamworks_sdk\redistributable_bin\win64\steam_api64.lib %solutionDir%\ /Y
    
ECHO copying msvcp140.dll into solution directory
xcopy C:\Windows\System32\msvcp140.dll %solutionDir%\ /Y

ECHO copying vcruntime140_1.dll into solution directory
xcopy C:\Windows\System32\vcruntime140_1.dll %solutionDir%\ /Y

ECHO copying vcruntime140.dll into solution directory
xcopy C:\Windows\System32\vcruntime140.dll %solutionDir%\ /Y
