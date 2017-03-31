@echo off
md ..\Project\Release

echo turn executable files from axf to bin by tool names fromelf in filefordler of D:\Keil_v5\ARM\ARMCC\bin\ .
D:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o ..\Project\Objects\ManagementBoardHwTest.bin ..\Project\Objects\ManagementBoardHwTest.axf

echo copy release files to release folder.
copy ..\Project\Objects\ManagementBoardHwTest.bin ..\Project\Release\ManagementBoardHwTest-V01-00-170331.bin
copy ..\Project\Objects\ManagementBoardHwTest.hex ..\Project\Release\ManagementBoardHwTest-V01-00-170331.hex
