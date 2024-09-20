###########################################################################
## Makefile generated for component 'NTCM_Procedure'. 
## 
## Makefile     : NTCM_Procedure.mk
## Generated on : Tue Apr 26 16:40:18 2022
## Final product: ./NTCM_Procedure_rtwlib.lib
## Product type : static library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# MODELREF_LINK_RSPFILE   Linker command listing model reference link objects
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = NTCM_Procedure
MAKEFILE                  = NTCM_Procedure.mk
MATLAB_ROOT               = C:/PROGRA~1/MATLAB/R2021a
MATLAB_BIN                = C:/PROGRA~1/MATLAB/R2021a/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/win64
START_DIR                 = C:/Users/zurnmar/Documents/2_SW_Devops/Prova+21042022/NTCM_mbd
SOLVER                    = 
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
TGT_FCN_LIB               = ISO_C
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 0
RELATIVE_PATH_TO_ANCHOR   = ../../../../../..
MODELREF_LINK_RSPFILE     = NTCM_Procedure_ref.rsp
COMPILER_COMMAND_FILE     = NTCM_Procedure_comp.rsp
CMD_FILE                  = NTCM_Procedure.rsp
C_STANDARD_OPTS           = -fwrapv
CPP_STANDARD_OPTS         = -fwrapv
MODELLIB                  = NTCM_Procedure_rtwlib.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          MinGW64 | gmake (64-bit Windows)
# Supported Version(s):    6.x
# ToolchainInfo Version:   2021a
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# C_STANDARD_OPTS
# CPP_STANDARD_OPTS
# MINGW_ROOT
# MINGW_C_STANDARD_OPTS

#-----------
# MACROS
#-----------

WARN_FLAGS            = -Wall -W -Wwrite-strings -Winline -Wstrict-prototypes -Wnested-externs -Wpointer-arith -Wcast-align
WARN_FLAGS_MAX        = $(WARN_FLAGS) -Wcast-qual -Wshadow
CPP_WARN_FLAGS        = -Wall -W -Wwrite-strings -Winline -Wpointer-arith -Wcast-align
CPP_WARN_FLAGS_MAX    = $(CPP_WARN_FLAGS) -Wcast-qual -Wshadow
MW_EXTERNLIB_DIR      = $(MATLAB_ROOT)/extern/lib/win64/mingw64
SHELL                 = %SystemRoot%/system32/cmd.exe

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = -lws2_32

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: GNU C Compiler
CC_PATH = $(MINGW_ROOT)
CC = "$(CC_PATH)/gcc"

# Linker: GNU Linker
LD_PATH = $(MINGW_ROOT)
LD = "$(LD_PATH)/g++"

# C++ Compiler: GNU C++ Compiler
CPP_PATH = $(MINGW_ROOT)
CPP = "$(CPP_PATH)/g++"

# C++ Linker: GNU C++ Linker
CPP_LD_PATH = $(MINGW_ROOT)
CPP_LD = "$(CPP_LD_PATH)/g++"

# Archiver: GNU Archiver
AR_PATH = $(MINGW_ROOT)
AR = "$(AR_PATH)/ar"

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)/mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: GMAKE Utility
MAKE_PATH = %MATLAB%/bin/win64
MAKE = "$(MAKE_PATH)/gmake"


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -g
C_OUTPUT_FLAG       = -o
LDDEBUG             = -g
OUTPUT_FLAG         = -o
CPPDEBUG            = -g
CPP_OUTPUT_FLAG     = -o
CPPLDDEBUG          = -g
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG =
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @move
RUN                 =

#----------------------------------------
# "Faster Builds" Build Configuration
#----------------------------------------

ARFLAGS              = ruvs
CFLAGS               = -c $(MINGW_C_STANDARD_OPTS) -m64 \
                       -O0
CPPFLAGS             = -c $(CPP_STANDARD_OPTS) -m64 \
                       -O0
CPP_LDFLAGS          =  -static -m64
CPP_SHAREDLIB_LDFLAGS  = -shared -Wl,--no-undefined \
                         -Wl,--out-implib,$(basename $(PRODUCT)).lib
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              =  -static -m64
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -shared -Wl,--no-undefined \
                       -Wl,--out-implib,$(basename $(PRODUCT)).lib



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = ./NTCM_Procedure_rtwlib.lib
PRODUCT_TYPE = "static library"
BUILD_TYPE = "Model Reference Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_BUILD_ARGS = -DCLASSIC_INTERFACE=0 -DALLOCATIONFCN=0 -DTERMFCN=1 -DONESTEPFCN=1 -DMAT_FILE=0 -DMULTI_INSTANCE_CODE=0 -DINTEGER_CODE=0 -DMT=0
DEFINES_CUSTOM = -DCheckOn=1
DEFINES_OPTS = -DTID01EQ=0 -DPORTABLE_WORDSIZES
DEFINES_STANDARD = -DMODEL=NTCM_Procedure -DNUMST=1 -DNCSTATES=0 -DHAVESTDIO -DMODEL_HAS_DYNAMICALLY_LOADED_SFCNS=0

DEFINES = $(DEFINES_BUILD_ARGS) $(DEFINES_CUSTOM) $(DEFINES_OPTS) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/Check.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/CheckOutput.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ChekInput.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ComputeDoA.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ComputeLocalTime.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/Compute_IonoDelay.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/Compute_sTEC.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ElevetionCheck.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ErrorMnG.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/NTCM_Procedure.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/NTCM_Procedure_Core.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/getIonoPiercePoint.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/llh2xyz.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/llh2xyz_Rec.c $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/llh2xyz_Sat.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = Check.obj CheckOutput.obj ChekInput.obj ComputeDoA.obj ComputeLocalTime.obj Compute_IonoDelay.obj Compute_sTEC.obj ElevetionCheck.obj ErrorMnG.obj NTCM_Procedure.obj NTCM_Procedure_Core.obj getIonoPiercePoint.obj llh2xyz.obj llh2xyz_Rec.obj llh2xyz_Sat.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

MODELREF_LIBS = C:/Users/zurnmar/Documents/2_SW_Devops/Prova+21042022/NTCM_mbd/slprj/ert/NTCM_G/sil/hostobj/instrumented/NTCM_G_rtwlib.lib

LIBS = $(START_DIR)/slprj/ert/_sharedutils/sil/hostobj/instrumented/rtwshared.lib

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) $(INCLUDES) @$(COMPILER_COMMAND_FILE)

CFLAGS += $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) $(INCLUDES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS += $(CPPFLAGS_BASIC)

#---------------------
# MEX C++ Compiler
#---------------------

MEX_CPP_Compiler_BASIC =  @$(COMPILER_COMMAND_FILE)

MEX_CPPFLAGS += $(MEX_CPP_Compiler_BASIC)

#-----------------
# MEX Compiler
#-----------------

MEX_Compiler_BASIC =  @$(COMPILER_COMMAND_FILE)

MEX_CFLAGS += $(MEX_Compiler_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################


MINGW_C_STANDARD_OPTS = $(C_STANDARD_OPTS)


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute


all : build
	@echo "### Successfully generated all binary outputs."


build : prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@echo "### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS)  $(PRODUCT) @$(CMD_FILE)
	@echo "### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.obj : %.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : %.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(START_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(MATLAB_ROOT)/rtw/c/src/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(MATLAB_ROOT)/simulink/src/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(MATLAB_ROOT)/simulink/src/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


Check.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/Check.c
	$(CC) $(CFLAGS) -o "$@" "$<"


CheckOutput.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/CheckOutput.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ChekInput.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ChekInput.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ComputeDoA.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ComputeDoA.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ComputeLocalTime.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ComputeLocalTime.c
	$(CC) $(CFLAGS) -o "$@" "$<"


Compute_IonoDelay.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/Compute_IonoDelay.c
	$(CC) $(CFLAGS) -o "$@" "$<"


Compute_sTEC.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/Compute_sTEC.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ElevetionCheck.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ElevetionCheck.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ErrorMnG.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/ErrorMnG.c
	$(CC) $(CFLAGS) -o "$@" "$<"


NTCM_Procedure.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/NTCM_Procedure.c
	$(CC) $(CFLAGS) -o "$@" "$<"


NTCM_Procedure_Core.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/NTCM_Procedure_Core.c
	$(CC) $(CFLAGS) -o "$@" "$<"


getIonoPiercePoint.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/getIonoPiercePoint.c
	$(CC) $(CFLAGS) -o "$@" "$<"


llh2xyz.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/llh2xyz.c
	$(CC) $(CFLAGS) -o "$@" "$<"


llh2xyz_Rec.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/llh2xyz_Rec.c
	$(CC) $(CFLAGS) -o "$@" "$<"


llh2xyz_Sat.obj : $(START_DIR)/slprj/ert/NTCM_Procedure/sil/hostobj/instrumented/llh2xyz_Sat.c
	$(CC) $(CFLAGS) -o "$@" "$<"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@echo "### PRODUCT = $(PRODUCT)"
	@echo "### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@echo "### BUILD_TYPE = $(BUILD_TYPE)"
	@echo "### INCLUDES = $(INCLUDES)"
	@echo "### DEFINES = $(DEFINES)"
	@echo "### ALL_SRCS = $(ALL_SRCS)"
	@echo "### ALL_OBJS = $(ALL_OBJS)"
	@echo "### LIBS = $(LIBS)"
	@echo "### MODELREF_LIBS = $(MODELREF_LIBS)"
	@echo "### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@echo "### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@echo "### CFLAGS = $(CFLAGS)"
	@echo "### LDFLAGS = $(LDFLAGS)"
	@echo "### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@echo "### CPPFLAGS = $(CPPFLAGS)"
	@echo "### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@echo "### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@echo "### ARFLAGS = $(ARFLAGS)"
	@echo "### MEX_CFLAGS = $(MEX_CFLAGS)"
	@echo "### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@echo "### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@echo "### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@echo "### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@echo "### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@echo "### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	$(RM) $(subst /,\,$(PRODUCT))
	$(RM) $(subst /,\,$(ALL_OBJS))
	$(ECHO) "### Deleted all derived files."


