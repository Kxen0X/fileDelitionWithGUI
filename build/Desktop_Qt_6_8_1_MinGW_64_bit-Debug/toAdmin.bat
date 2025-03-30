@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Ошибка: не указан путь к файлу или папке.
    exit /b 1
)

set "target=%~1"
set "userName=%USERNAME%"
@echo "%target%" 
if exist "%target%\" (
    takeown /f "%target%" /r /d y /a
    if errorlevel 1 (
        echo "Error: Failed to set ownership permissions" "%target%"
    )
) else (
    takeown /f "%target%" /a
    if errorlevel 1 (
        echo "Error: Failed to set ownership permissions" "%target%"
    )
)

icacls "%target%" /grant "%userName%":(F) /t
if errorlevel 1 (
    echo "Failed to change access permissions for" "%target%".
    exit /b 1
)

if exist "%target%\" (
    echo Deliting the folder: "%target%"
    rd "%target%" /s /q
    if errorlevel 1 (
        echo Error: failed to delete the folder: "%target%".
        exit /b 1
    )
) else (
    echo Deliting the file: "%target%"
    del "%target%" /q
    if errorlevel 1 (
        echo Error: failed to delete the file: "%target%".
        exit /b 1
    )
)

echo Operation completed succesfully.
endlocal