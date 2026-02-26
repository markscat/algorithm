@echo off
setlocal enabledelayedexpansion

:: 1. 設定 Qt 的路徑 (請確保路徑與你的版本一致)
set QT_PATH=C:\Qt\6.9.2\mingw_64\bin
set PATH=%PATH%;%QT_PATH%

echo [Step 1] 正在尋找資料夾中的 .exe 檔案...

:: 2. 自動偵測第一個找到的 .exe 檔案
set "TARGET_EXE="
for %%f in (*.exe) do (
    if not "%%f"=="vc_redist.x64.exe" (
        set "TARGET_EXE=%%f"
        goto :found
    )
)

:found
if "%TARGET_EXE%"=="" (
    echo [錯誤] 找不到任何 .exe 檔案！請確保此腳本放在 Release 資料夾中。
    pause
    exit /b
)

echo [Step 2] 偵測到目標：%TARGET_EXE%

:: 3. 執行 windeployqt
echo [Step 3] 執行 windeployqt 打包中...
windeployqt.exe --release "%TARGET_EXE%"

:: 4. 執行 strip 瘦身 (移除除錯符號)
:: strip 通常在編譯器的 bin 資料夾內，windeployqt 的路徑通常也包含它
echo [Step 4] 執行 strip 瘦身處理...
strip "%TARGET_EXE%"

echo.
echo ==========================================
echo 打包完成！"%TARGET_EXE%" 及其依賴項已準備就緒。
echo ==========================================
pause
