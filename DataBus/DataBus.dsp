# Microsoft Developer Studio Project File - Name="DataBus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DataBus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DataBus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DataBus.mak" CFG="DataBus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DataBus - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DataBus - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DataBus - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DataBus___Win32_Release"
# PROP BASE Intermediate_Dir "DataBus___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DataBus___Win32_Release"
# PROP Intermediate_Dir "DataBus___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATABUS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATABUS_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "DataBus - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DataBus___Win32_Debug"
# PROP BASE Intermediate_Dir "DataBus___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DataBus___Win32_Debug"
# PROP Intermediate_Dir "DataBus___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATABUS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../Databus/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DATABUS_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ws2_32.lib /nologo /dll /debug /machine:I386 /out:"../Debug/DataBus.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DataBus - Win32 Release"
# Name "DataBus - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\Any.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CDataArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CDataCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CDataChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CDataFlow.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CFlowIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CFlowOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CNetworkOperator.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CPeer.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CReceivingArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CSendingArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CTCPAcceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CTCPConnector.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CTCPReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CTCPSender.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CUDPReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CUDPSender.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DataBus.cpp
# End Source File
# Begin Source File

SOURCE=..\CompDll\DataDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DataDispatchor.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DataHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DataLogics.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DataReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DataWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DomainParticipant.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DomainParticipantFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\NetMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Publisher.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\socket_utility.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Subscriber.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Topic.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Include\Any.h
# End Source File
# Begin Source File

SOURCE=.\Include\CAcceptor.h
# End Source File
# Begin Source File

SOURCE=.\Include\CConnector.h
# End Source File
# Begin Source File

SOURCE=.\Include\CDataArray.h
# End Source File
# Begin Source File

SOURCE=.\Include\CDataCache.h
# End Source File
# Begin Source File

SOURCE=.\Include\CDataChannel.h
# End Source File
# Begin Source File

SOURCE=.\Include\CDataFlow.h
# End Source File
# Begin Source File

SOURCE=.\Include\CFlowIn.h
# End Source File
# Begin Source File

SOURCE=.\Include\CFlowOut.h
# End Source File
# Begin Source File

SOURCE=.\Include\CNetworkOperator.h
# End Source File
# Begin Source File

SOURCE=.\Include\CPeer.h
# End Source File
# Begin Source File

SOURCE=.\Include\CPrioSetting.h
# End Source File
# Begin Source File

SOURCE=.\Include\CPrioSettingTest.h
# End Source File
# Begin Source File

SOURCE=.\Include\CReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Include\CReceivingArray.h
# End Source File
# Begin Source File

SOURCE=.\Include\CSender.h
# End Source File
# Begin Source File

SOURCE=.\Include\CSendingArray.h
# End Source File
# Begin Source File

SOURCE=.\Include\CTCPAcceptor.h
# End Source File
# Begin Source File

SOURCE=.\Include\CTCPConnector.h
# End Source File
# Begin Source File

SOURCE=.\Include\CTCPReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Include\CTCPSender.h
# End Source File
# Begin Source File

SOURCE=.\Include\CUDPReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Include\CUDPSender.h
# End Source File
# Begin Source File

SOURCE=.\Include\Data.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataBus.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataDispatchor.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataHandler.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataLogics.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataProxy.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataReader.h
# End Source File
# Begin Source File

SOURCE=.\Include\DataWriter.h
# End Source File
# Begin Source File

SOURCE=.\Include\DomainParticipant.h
# End Source File
# Begin Source File

SOURCE=.\Include\DomainParticipantFactory.h
# End Source File
# Begin Source File

SOURCE=.\Include\NetMonitor.h
# End Source File
# Begin Source File

SOURCE=.\Include\Publisher.h
# End Source File
# Begin Source File

SOURCE=.\Include\socket_utility.h
# End Source File
# Begin Source File

SOURCE=.\Include\Subscriber.h
# End Source File
# Begin Source File

SOURCE=.\Include\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Include\Topic.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
