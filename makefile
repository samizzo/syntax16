CPP = wcc386
BUILD = debug

LINK_FLAGS_DEBUG = debug all
LINK_FLAGS_RELEASE =
LINK_FLAGS = $(LINK_FLAGS_$(BUILD))

CPP_FLAGS_DEBUG = -wx -we -fpi87 -j -zu -d2 -5r -dDEBUG -db -mf
CPP_FLAGS_RELEASE = -wx -we -oxl+ -j -s -zu -fpi87 -5r -DRELEASE -mf
CPP_FLAGS = $(CPP_FLAGS_$(BUILD))

OUTPUT_DIR = $(BUILD)

OBJ_FILES = $(OUTPUT_DIR)\main.obj $(OUTPUT_DIR)\video.obj $(OUTPUT_DIR)\kb.obj $(OUTPUT_DIR)\log.obj $(OUTPUT_DIR)\image.obj $(OUTPUT_DIR)\array.obj $(OUTPUT_DIR)\polytun.obj $(OUTPUT_DIR)\dots.obj
EXE_FILE = $(OUTPUT_DIR)\test.exe

default: make_dirs $(EXE_FILE)

$(EXE_FILE): $(OBJ_FILES) makefile
	%write link.lnk name $@
	%write link.lnk system pmodew
	%write link.lnk $(LINK_FLAGS)
	%write link.lnk file {$(OBJ_FILES)}
	%write link.lnk option map
	wlink @link.lnk

clean: .SYMBOLIC
	@del $(OUTPUT_DIR)\*.*
	@if exist $(OUTPUT_DIR) rmdir $(OUTPUT_DIR)

make_dirs: .SYMBOLIC
	@if not exist $(OUTPUT_DIR) mkdir $(OUTPUT_DIR)

.cpp.obj: .AUTODEPEND
	$(CPP) $(CPP_FLAGS) $[* -fo=$(OUTPUT_DIR)\$[&

.c.obj: .AUTODEPEND
	$(CPP) $(CPP_FLAGS) $[* -fo=$(OUTPUT_DIR)\$[&
