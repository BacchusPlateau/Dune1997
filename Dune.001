# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Dune - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Dune - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Dune - Win32 Release" && "$(CFG)" != "Dune - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dune.mak" CFG="Dune - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dune - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Dune - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Dune - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Dune - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Dune.exe"

CLEAN : 
	-@erase "$(INTDIR)\Cdib.obj"
	-@erase "$(INTDIR)\Cdirdraw.obj"
	-@erase "$(INTDIR)\Cdirsnd.obj"
	-@erase "$(INTDIR)\Cmydd.obj"
	-@erase "$(INTDIR)\Cwave.obj"
	-@erase "$(INTDIR)\Dune.obj"
	-@erase "$(INTDIR)\dune.res"
	-@erase "$(OUTDIR)\Dune.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Dune.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dune.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Dune.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Dune.pdb" /machine:I386 /out:"$(OUTDIR)/Dune.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Cdib.obj" \
	"$(INTDIR)\Cdirdraw.obj" \
	"$(INTDIR)\Cdirsnd.obj" \
	"$(INTDIR)\Cmydd.obj" \
	"$(INTDIR)\Cwave.obj" \
	"$(INTDIR)\Dune.obj" \
	"$(INTDIR)\dune.res" \
	"..\..\Gdk\sdk\lib\ddraw.lib" \
	"..\..\Gdk\sdk\lib\dsound.lib" \
	"..\..\Msdev\Lib\Winmm.lib"

"$(OUTDIR)\Dune.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Dune.exe"

CLEAN : 
	-@erase "$(INTDIR)\Cdib.obj"
	-@erase "$(INTDIR)\Cdirdraw.obj"
	-@erase "$(INTDIR)\Cdirsnd.obj"
	-@erase "$(INTDIR)\Cmydd.obj"
	-@erase "$(INTDIR)\Cwave.obj"
	-@erase "$(INTDIR)\Dune.obj"
	-@erase "$(INTDIR)\dune.res"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Dune.exe"
	-@erase "$(OUTDIR)\Dune.ilk"
	-@erase "$(OUTDIR)\Dune.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Dune.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dune.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Dune.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Dune.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Dune.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Cdib.obj" \
	"$(INTDIR)\Cdirdraw.obj" \
	"$(INTDIR)\Cdirsnd.obj" \
	"$(INTDIR)\Cmydd.obj" \
	"$(INTDIR)\Cwave.obj" \
	"$(INTDIR)\Dune.obj" \
	"$(INTDIR)\dune.res" \
	"..\..\Gdk\sdk\lib\ddraw.lib" \
	"..\..\Gdk\sdk\lib\dsound.lib" \
	"..\..\Msdev\Lib\Winmm.lib"

"$(OUTDIR)\Dune.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Dune - Win32 Release"
# Name "Dune - Win32 Debug"

!IF  "$(CFG)" == "Dune - Win32 Release"

!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Dune.cpp

!IF  "$(CFG)" == "Dune - Win32 Release"

DEP_CPP_DUNE_=\
	".\Cdib.h"\
	".\Cdirdraw.h"\
	".\Cdirsnd.h"\
	".\Cmydd.h"\
	".\Cwave.h"\
	{$(INCLUDE)}"\d3dtypes.h"\
	{$(INCLUDE)}"\ddraw.h"\
	{$(INCLUDE)}"\dsound.h"\
	
NODEP_CPP_DUNE_=\
	"..\..\DXSDK\sdk\inc\subwtype.h"\
	

"$(INTDIR)\Dune.obj" : $(SOURCE) $(DEP_CPP_DUNE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

DEP_CPP_DUNE_=\
	".\Cdib.h"\
	".\Cdirdraw.h"\
	".\Cdirsnd.h"\
	".\Cmydd.h"\
	".\Cwave.h"\
	{$(INCLUDE)}"\d3dtypes.h"\
	{$(INCLUDE)}"\ddraw.h"\
	{$(INCLUDE)}"\dsound.h"\
	
NODEP_CPP_DUNE_=\
	"..\..\DXSDK\sdk\inc\subwtype.h"\
	

"$(INTDIR)\Dune.obj" : $(SOURCE) $(DEP_CPP_DUNE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dune.rc
DEP_RSC_DUNE_R=\
	".\Dune.ico"\
	

"$(INTDIR)\dune.res" : $(SOURCE) $(DEP_RSC_DUNE_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Gdk\sdk\lib\ddraw.lib

!IF  "$(CFG)" == "Dune - Win32 Release"

!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cdirdraw.cpp
DEP_CPP_CDIRD=\
	".\Cdib.h"\
	".\Cdirdraw.h"\
	{$(INCLUDE)}"\ddraw.h"\
	

"$(INTDIR)\Cdirdraw.obj" : $(SOURCE) $(DEP_CPP_CDIRD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cdib.cpp
DEP_CPP_CDIB_=\
	".\Cdib.h"\
	

"$(INTDIR)\Cdib.obj" : $(SOURCE) $(DEP_CPP_CDIB_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cmydd.cpp

!IF  "$(CFG)" == "Dune - Win32 Release"

DEP_CPP_CMYDD=\
	".\Cdirdraw.h"\
	".\Cmydd.h"\
	

"$(INTDIR)\Cmydd.obj" : $(SOURCE) $(DEP_CPP_CMYDD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

DEP_CPP_CMYDD=\
	".\Cdib.h"\
	".\Cdirdraw.h"\
	".\Cmydd.h"\
	{$(INCLUDE)}"\ddraw.h"\
	

"$(INTDIR)\Cmydd.obj" : $(SOURCE) $(DEP_CPP_CMYDD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cdirsnd.cpp
DEP_CPP_CDIRS=\
	".\Cdirsnd.h"\
	".\Cwave.h"\
	{$(INCLUDE)}"\d3dtypes.h"\
	{$(INCLUDE)}"\ddraw.h"\
	{$(INCLUDE)}"\dsound.h"\
	
NODEP_CPP_CDIRS=\
	"..\..\DXSDK\sdk\inc\subwtype.h"\
	

"$(INTDIR)\Cdirsnd.obj" : $(SOURCE) $(DEP_CPP_CDIRS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cwave.cpp
DEP_CPP_CWAVE=\
	".\Cwave.h"\
	

"$(INTDIR)\Cwave.obj" : $(SOURCE) $(DEP_CPP_CWAVE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\Gdk\sdk\lib\dsound.lib

!IF  "$(CFG)" == "Dune - Win32 Release"

!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Msdev\Lib\Winmm.lib

!IF  "$(CFG)" == "Dune - Win32 Release"

!ELSEIF  "$(CFG)" == "Dune - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
