:: Batch to ensure the needed DLL is copied over the folder where the executable is stored, so the library is correctly read at runtime
:: Called from Visual Studio: Solution Properties > Build Events > Post-Build Event > Command Line: call "pathToBatchFile" "batchParam %1" "batchParam %2"
:: batchParam %1 - Absolute path to executable location that needs a DLL to be copied over
:: batchParam %2 - Absolute path to DLL

:: Path to location needing DLL
set Destination=%1

:: Path to DLL definition
set Source=%2

echo CopyDLLPostBuild.bat - Copying DLL %Source% to %Destination%

copy %Source% %Destination%