@echo off
REM Create .env if it doesn't exist
IF NOT EXIST .env (
    echo Creating virtual environment...
    python -m venv .env
)

:: Activate .env
call .env\Scripts\activate.bat

:: Ensure required package is installed
.env\Scripts\pip show build >nul 2>&1 || .env\Scripts\pip install build

:: Run build
.env\Scripts\python -m build --wheel

:: Pause to show result
pause
