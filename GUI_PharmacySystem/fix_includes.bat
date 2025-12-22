@echo off
echo Fixing includes in all files...

rem Add using namespace std; to all .cpp files
for %%f in (*.cpp) do (
    findstr "using namespace std;" "%%f" >nul
    if errorlevel 1 (
        echo Adding namespace to %%f
        echo using namespace std; >> "%%f"
    )
)

rem Add vector include to all .cpp and .h files
for %%f in (*.cpp *.h) do (
    findstr "#include <vector>" "%%f" >nul
    if errorlevel 1 (
        echo Adding vector include to %%f
        sed -i "1s/^/#include <vector>\n/" "%%f"
    )
)

echo Done!
