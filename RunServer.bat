@echo off

SETLOCAL EnableDelayedExpansion

set UE_PATH_FILE=UEPath.txt

IF EXIST %UE_PATH_FILE% (
    echo %UE_PATH_FILE%
    set /p UE_PATH=<%UE_PATH_FILE%
    echo !UE_PATH!

    start "Server" "!UE_PATH!\Engine\Binaries\Win64\UnrealEditor.exe" "%cd%\PuzzleCrushGame.uproject" GameMap -server -log -nosteam -port=7777

) ELSE (
    
    echo UE PATH file NOT FOUND

    set UE_PATH=
    set /P UE_PATH=Please enter Unreal engine 5.2 RootPath: %=%
    echo !UE_PATH! >  %UE_PATH_FILE%
    start "Server" "!UE_PATH!\Engine\Binaries\Win64\UnrealEditor.exe" "%cd%\PuzzleCrushGame.uproject" GameMap -server -log -nosteam -port=7777
)