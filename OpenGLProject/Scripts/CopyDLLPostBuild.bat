:: Batch to ensure a needed DLL is copied over the Visual Studio project folder so the library is correctly read at runtime
:: Called from Visual Studio: Solution Properties > Build Events > Post-Build Event > Command Line: call "pathToBatchFile" "batchParam %1" "batchParam %2"
:: batchParam %1 - Absolute path to DLL that needs to be copied over
:: batchParam %2 - Name of Visual Studio project folder

:: Set absolute path to DLL in a variable for more clarity
set Source=%2

:: Set relative path to Visual Studio project folder (stored in a folder at the same level as 'Scripts')
set Destination=%~dp0\..\%1%

echo CopyDLLPostBuild.bat - Copying DLL %Source% to VS Project folder %1

copy %Source% %Destination%