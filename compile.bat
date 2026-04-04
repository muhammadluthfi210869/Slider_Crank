@echo off
setlocal

:: Target Nama Program
set OUT=slider_crank.exe

:: Pastikan path mengarah ke raylib/w64devkit/bin
set "PATH=C:\raylib\w64devkit\bin;%PATH%"

echo Sedang mengkompilasi...
gcc main.c -O2 -Wall -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows -o %OUT%

if %ERRORLEVEL% neq 0 (
    echo.
    echo Ada error saat kompilasi! Cek log di atas.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo Kompilasi Selesai! Menjalankan program...
.\%OUT%

endlocal
