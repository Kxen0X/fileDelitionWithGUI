@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Ошибка: не указан путь к файлу или папке.
    exit /b 1
)

set "target=%~1"
set "userName=%USERNAME%"
if exist "%target%\" (
    takeown /f "%target%" /r /d y /a
    if errorlevel 1 (
	exit /b 1
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
    exit /b 1
    echo "Failed to change access permissions for" "%target%".
)

if exist "%target%\" (
    rd "%target%" /s /q
    if errorlevel 1 (
	exit /b 1
        echo Error: failed to delete the folder: "%target%".
    )
) else (
    del "%target%" /q
    if errorlevel 1 (
	exit /b 1
        echo Error: failed to delete the file: "%target%".
    )
)

echo Operation completed succesfully.
endlocal