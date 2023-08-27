# Inventory-Trend-Analysis-System
A system that allows a business to analyse trends in purchase habits of each of their sold products/services.

Visual Studio 2019 Settings (Copied and pasted from my VS2019)
---------------------------------------------------------------------------------

Run in Release Mode

VC++ Directories "Include Directories" :
$(VC_IncludePath);$(WindowsSDK_IncludePath);C:\Program Files\MySQL\MySQL Server 8.0\include;

C/C++ >> General >> "Additional Include Directories" :
C:\Program Files\MySQL\Connector C++ 8.0\include\jdbc;$(MSBuildThisFileDirectory)include\msvc;$(MSBuildThisFileDirectory)include;%(AdditionalIncludeDirectories);C:\Program Files\MySQL\Connector C++ 8.0\include;C:\Program Files\boost_1_83_0

Linker >> General >> "Additional Library Directories" :
$(wxLibOrDllDir);%(AdditionalLibraryDirectories);C:\Program Files\MySQL\MySQL Server 8.0\lib;C:\Program Files\MySQL\Connector C++ 8.0\lib64\vs14

Linker >> Input >> "Additional Dependencies" :
kernel32.lib;user32.lib;gdi32.lib;comdlg32.lib;winspool.lib;shell32.lib;shlwapi.lib;ole32.lib;oleaut32.lib;uuid.lib;advapi32.lib;version.lib;comctl32.lib;rpcrt4.lib;ws2_32.lib;wininet.lib;winmm.lib;%(AdditionalDependencies);mysqlcppconn.lib

---------------------------------------------------------------------------------

Uses wxWidgets and MYSqlServer

---------------------------------------------------------------------------------
