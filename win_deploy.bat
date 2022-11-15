@echo off

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 定义Qt目录
set "QT_DIR=D:/programes/Qt62/6.2.4/mingw_64/bin"
set "QT_TOOLS_DIR=D:/programs/Qt62/Tools/mingw1120_64/bin"
:: 定义Inno Setup目录
set "INNO_SETUP_DIR=C:/Program Files (x86)/Inno Setup 6"

set "OPENCV_DIR=C:/opencv/x64/mingw/bin"
:: 定义版本号
set "HELLOQT_VERSION=0.0.1"
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 设置环境变量
set "PATH=%QT_DIR%;%QT_TOOLS_DIR%;%INNO_SETUP_DIR%;%PATH%"
:: 编译
del .qmake.stash Makefile
if exist ".\build_debug" (
    rmdir /Q /S .\build_debug
)
if exist ".\build_release" (
    rmdir /Q /S .\build_release
)
qmake helloqt.pro -spec win32-g++
mingw32-make -j8
:: clean打包目录
if exist ".\InnoSetup\build" (
    rmdir /Q /S .\InnoSetup\build
)
:: 配置打包信息
copy /y .\InnoSetup\build_setup.iss .\InnoSetup\build_temp_setup.iss
.\tools\sed\sed.exe -i "s/#VERSION#/%HELLOQT_VERSION%/g" .\InnoSetup\build_temp_setup.iss
.\tools\sed\sed.exe -i "s/#VERSIONINFOVERSION#/%HELLOQT_VERSION%.000/g" .\InnoSetup\build_temp_setup.iss
del /f /q /a .\sed*
:: 构建打包目录
xcopy /y .\build_release\out\helloqt.exe .\InnoSetup\build\
xcopy /y models\face2.xml .\InnoSetup\build\
:: 使用windeployqt拷贝依赖dll库到打包目录
windeployqt --dir .\InnoSetup\build --no-translations --compiler-runtime .\InnoSetup\build\helloqt.exe
xcopy /y "%OPENCV_DIR%\libopencv_imgproc455.dll" ".\InnoSetup\build\"
xcopy /y "%OPENCV_DIR%\libopencv_core455.dll" ".\InnoSetup\build\"
xcopy /y "%OPENCV_DIR%\libopencv_videoio455.dll" ".\InnoSetup\build\"
xcopy /y "%OPENCV_DIR%\libopencv_objdetect455.dll" ".\InnoSetup\build\"

xcopy /y "%QT_TOOLS_DIR%\lib*.dll" ".\InnoSetup\build\"

:: 打包
echo "wait inno build setup..."
iscc /q ".\InnoSetup\build_temp_setup.iss"
del .\InnoSetup\build_temp_setup.iss
echo "build success!"
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
