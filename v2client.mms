! header                                                                                                                                                   !auto
! Do not modify this file. It may be overwritten automatically.                                                                                            !auto
! includes                                                                                                                                                 !auto
INCLUDES= -                                                                                                                                                !auto
plib.h -                                                                                                                                                   !auto
tlib.h -                                                                                                                                                   !auto
v2client.h -                                                                                                                                               !auto
vmsconio.h -                                                                                                                                               !auto
[.glide]3DFX.H -                                                                                                                                           !auto
[.glide]FXDLL.H -                                                                                                                                          !auto
[.glide]FXGLOB.H -                                                                                                                                         !auto
[.glide]FXOS.H -                                                                                                                                           !auto
[.glide]SST1VID.H -                                                                                                                                        !auto
[.glide]glide.h -                                                                                                                                          !auto
[.glide]glidesys.h -                                                                                                                                       !auto
[.glide]glideutl.h                                                                                                                                         !auto
! sources                                                                                                                                                  !auto
SOURCES= -                                                                                                                                                 !auto
plib.c -                                                                                                                                                   !auto
tlib.c -                                                                                                                                                   !auto
v2client.c -                                                                                                                                               !auto
vmsconio.c                                                                                                                                                 !auto
! dependencies                                                                                                                                             !auto
.FIRST                                                                                                                                                     !auto
                                                                                                                                                           !auto
.LAST                                                                                                                                                      !auto
                                                                                                                                                           !auto
! compiler/linker options                                                                                                                                  !auto
.IF DEBUG .AND $(DEBUG) .EQ 1                                                                                                                              !auto
COMPILEFLAGS = /DEBUG/NOOP/LIST=$(MMS$TARGET_NAME)/OBJECT=$(MMS$TARGET)/INCLUDE_DIRECTORY=([.glide])/FLOAT=IEEE_FLOAT/DEFINE=(__BSD44_CURSES,__IEEE_FLOAT) !auto
LINKFLAGS = /DEBUG/MAP=$(MMS$TARGET_NAME)/EXECUTABLE=$(MMS$TARGET)                                                                                         !auto
.ELSE                                                                                                                                                      !auto
COMPILEFLAGS = /OBJECT=$(MMS$TARGET)/INCLUDE_DIRECTORY=([.glide])/FLOAT=IEEE_FLOAT/DEFINE=(__BSD44_CURSES,__IEEE_FLOAT)                                    !auto
LINKFLAGS = /EXECUTABLE=$(MMS$TARGET)                                                                                                                      !auto
.ENDIF                                                                                                                                                     !auto
! directives                                                                                                                                               !auto
.SILENT                                                                                                                                                    !auto
OUT_DIR = .$(OUTDIR).$(CONFIG)                                                                                                                             !auto
OBJ_DIR = $(OUT_DIR).obj                                                                                                                                   !auto
.SUFFIXES                                                                                                                                                  !auto
.SUFFIXES .OBJ .CPP .C .CLD .MSG .BLI .COB .PAS .BAS .F77 .F90 .FOR .B32 .CBL                                                                              !auto
.CPP.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    $(CXX) $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                   !auto
                                                                                                                                                           !auto
.C.OBJ                                                                                                                                                     !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    $(CC) $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                    !auto
                                                                                                                                                           !auto
.CLD.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    SET COMMAND/OBJECT=$(MMS$TARGET) $(MMS$SOURCE)                                                                                                         !auto
                                                                                                                                                           !auto
.MSG.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    MESSAGE /OBJECT=$(MMS$TARGET) $(MMS$SOURCE)                                                                                                            !auto
                                                                                                                                                           !auto
.BLI.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    BLISS $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                    !auto
                                                                                                                                                           !auto
.B32.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    BLISS $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                    !auto
                                                                                                                                                           !auto
.COB.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    COBOL $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                    !auto
                                                                                                                                                           !auto
.CBL.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    COBOL $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                    !auto
                                                                                                                                                           !auto
.PAS.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    PASCAL $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                   !auto
                                                                                                                                                           !auto
.BAS.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    BASIC $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                    !auto
                                                                                                                                                           !auto
.F77.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    FORTRAN $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                  !auto
                                                                                                                                                           !auto
.F90.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    FORTRAN $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                  !auto
                                                                                                                                                           !auto
.FOR.OBJ                                                                                                                                                   !auto
    pipe create/dir $(DIR $(MMS$TARGET)) | copy SYS$INPUT nl:                                                                                              !auto
    FORTRAN $(COMPILEFLAGS) $(MMS$SOURCE)                                                                                                                  !auto
                                                                                                                                                           !auto
.DEFAULT                                                                                                                                                   !auto
    ! Source $(MMS$TARGET) not yet added                                                                                                                   !auto
                                                                                                                                                           !auto
! main target                                                                                                                                              !auto
[$(OUT_DIR)]$(NAME).EXE :  -                                                                                                                               !auto
[$(OBJ_DIR)]plib.obj -                                                                                                                                     !auto
[$(OBJ_DIR)]tlib.obj -                                                                                                                                     !auto
[$(OBJ_DIR)]v2client.obj -                                                                                                                                 !auto
[$(OBJ_DIR)]vmsconio.obj                                                                                                                                   !auto
    CXXLINK $(LINKFLAGS) $(MMS$SOURCE_LIST),[]$(NAME)/OPT                                                                                                  !auto
                                                                                                                                                           !auto
! objects                                                                                                                                                  !auto
[$(OBJ_DIR)]plib.obj : plib.c $(INCLUDES)                                                                                                                  !auto
[$(OBJ_DIR)]tlib.obj : tlib.c $(INCLUDES)                                                                                                                  !auto
[$(OBJ_DIR)]v2client.obj : v2client.c $(INCLUDES)                                                                                                          !auto
[$(OBJ_DIR)]vmsconio.obj : vmsconio.c $(INCLUDES)                                                                                                          !auto
                                                                                                                                                          