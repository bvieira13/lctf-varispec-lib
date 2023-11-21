# Microsoft Developer Studio Generated NMAKE File, Based on VsGui.dsp
!IF "$(CFG)" == ""
CFG=VsGui - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VsGui - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VsGui - Win32 Release" && "$(CFG)" != "VsGui - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VsGui.mak" CFG="VsGui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VsGui - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VsGui - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "VsGui - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\VsGui.exe" "$(OUTDIR)\VsGui.pch" "$(OUTDIR)\VsGui.bsc"

!ELSE 

ALL : "VsDrvr - Win32 Release" "$(OUTDIR)\VsGui.exe" "$(OUTDIR)\VsGui.pch" "$(OUTDIR)\VsGui.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"VsDrvr - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ComPortUtil.obj"
	-@erase "$(INTDIR)\ComPortUtil.sbr"
	-@erase "$(INTDIR)\RegistryAccess.obj"
	-@erase "$(INTDIR)\RegistryAccess.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VsConfigDlg.obj"
	-@erase "$(INTDIR)\VsConfigDlg.sbr"
	-@erase "$(INTDIR)\VsGui.obj"
	-@erase "$(INTDIR)\VsGui.pch"
	-@erase "$(INTDIR)\VsGui.res"
	-@erase "$(INTDIR)\VsGui.sbr"
	-@erase "$(INTDIR)\VsGuiDlg.obj"
	-@erase "$(INTDIR)\VsGuiDlg.sbr"
	-@erase "$(INTDIR)\VsSweep.obj"
	-@erase "$(INTDIR)\VsSweep.sbr"
	-@erase "$(OUTDIR)\VsGui.bsc"
	-@erase "$(OUTDIR)\VsGui.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\varispec" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "USESTDAFX" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VsGui.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VsGui.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ComPortUtil.sbr" \
	"$(INTDIR)\RegistryAccess.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\VsConfigDlg.sbr" \
	"$(INTDIR)\VsGui.sbr" \
	"$(INTDIR)\VsGuiDlg.sbr" \
	"$(INTDIR)\VsSweep.sbr"

"$(OUTDIR)\VsGui.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=vsdrvr.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\VsGui.pdb" /machine:I386 /out:"$(OUTDIR)\VsGui.exe" /libpath:"..\varispec\release" 
LINK32_OBJS= \
	"$(INTDIR)\ComPortUtil.obj" \
	"$(INTDIR)\RegistryAccess.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VsConfigDlg.obj" \
	"$(INTDIR)\VsGui.obj" \
	"$(INTDIR)\VsGuiDlg.obj" \
	"$(INTDIR)\VsSweep.obj" \
	"$(INTDIR)\VsGui.res" \
	"..\VariSpec\Release\VsDrvr.lib"

"$(OUTDIR)\VsGui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\VsGui.exe" "$(OUTDIR)\VsGui.pch"

!ELSE 

ALL : "VsDrvr - Win32 Debug" "$(OUTDIR)\VsGui.exe" "$(OUTDIR)\VsGui.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"VsDrvr - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ComPortUtil.obj"
	-@erase "$(INTDIR)\RegistryAccess.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VsConfigDlg.obj"
	-@erase "$(INTDIR)\VsGui.obj"
	-@erase "$(INTDIR)\VsGui.pch"
	-@erase "$(INTDIR)\VsGui.res"
	-@erase "$(INTDIR)\VsGuiDlg.obj"
	-@erase "$(INTDIR)\VsSweep.obj"
	-@erase "$(OUTDIR)\VsGui.exe"
	-@erase "$(OUTDIR)\VsGui.ilk"
	-@erase "$(OUTDIR)\VsGui.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\varispec" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "USESTDAFX" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\VsGui.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VsGui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=vsdrvr.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\VsGui.pdb" /debug /machine:I386 /out:"$(OUTDIR)\VsGui.exe" /pdbtype:sept /libpath:"..\varispec\debug" 
LINK32_OBJS= \
	"$(INTDIR)\ComPortUtil.obj" \
	"$(INTDIR)\RegistryAccess.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\VsConfigDlg.obj" \
	"$(INTDIR)\VsGui.obj" \
	"$(INTDIR)\VsGuiDlg.obj" \
	"$(INTDIR)\VsSweep.obj" \
	"$(INTDIR)\VsGui.res" \
	"..\VariSpec\Debug\VsDrvr.lib"

"$(OUTDIR)\VsGui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("VsGui.dep")
!INCLUDE "VsGui.dep"
!ELSE 
!MESSAGE Warning: cannot find "VsGui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VsGui - Win32 Release" || "$(CFG)" == "VsGui - Win32 Debug"
SOURCE=.\ComPortUtil.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"


"$(INTDIR)\ComPortUtil.obj"	"$(INTDIR)\ComPortUtil.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"


"$(INTDIR)\ComPortUtil.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RegistryAccess.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"


"$(INTDIR)\RegistryAccess.obj"	"$(INTDIR)\RegistryAccess.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"


"$(INTDIR)\RegistryAccess.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\varispec" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "USESTDAFX" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\VsGui.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\VsGui.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\varispec" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "USESTDAFX" /Fp"$(INTDIR)\VsGui.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\VsGui.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\VsConfigDlg.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"


"$(INTDIR)\VsConfigDlg.obj"	"$(INTDIR)\VsConfigDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"


"$(INTDIR)\VsConfigDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\VsGui.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"


"$(INTDIR)\VsGui.obj"	"$(INTDIR)\VsGui.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"


"$(INTDIR)\VsGui.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\VsGui.rc

"$(INTDIR)\VsGui.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\VsGuiDlg.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"


"$(INTDIR)\VsGuiDlg.obj"	"$(INTDIR)\VsGuiDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"


"$(INTDIR)\VsGuiDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\VsSweep.cpp

!IF  "$(CFG)" == "VsGui - Win32 Release"


"$(INTDIR)\VsSweep.obj"	"$(INTDIR)\VsSweep.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"


"$(INTDIR)\VsSweep.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

!IF  "$(CFG)" == "VsGui - Win32 Release"

"VsDrvr - Win32 Release" : 
   cd "\CriHwDrivers\VariSpec"
   $(MAKE) /$(MAKEFLAGS) /F .\VsDrvr.mak CFG="VsDrvr - Win32 Release" 
   cd "..\VsGui"

"VsDrvr - Win32 ReleaseCLEAN" : 
   cd "\CriHwDrivers\VariSpec"
   $(MAKE) /$(MAKEFLAGS) /F .\VsDrvr.mak CFG="VsDrvr - Win32 Release" RECURSE=1 CLEAN 
   cd "..\VsGui"

!ELSEIF  "$(CFG)" == "VsGui - Win32 Debug"

"VsDrvr - Win32 Debug" : 
   cd "\CriHwDrivers\VariSpec"
   $(MAKE) /$(MAKEFLAGS) /F .\VsDrvr.mak CFG="VsDrvr - Win32 Debug" 
   cd "..\VsGui"

"VsDrvr - Win32 DebugCLEAN" : 
   cd "\CriHwDrivers\VariSpec"
   $(MAKE) /$(MAKEFLAGS) /F .\VsDrvr.mak CFG="VsDrvr - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\VsGui"

!ENDIF 


!ENDIF 

