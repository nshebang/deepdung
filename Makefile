# platform should be PLATFORM_LINUX or PLATFORM_WINDOWS
PLATFORM	?= PLATFORM_LINUX
# build type should be BUILD_DEBUG or BUILD_RELEASE
BUILD_TYPE	?= BUILD_DEBUG

# name of the project
PROJECT		?= deepdung

# set this to TRUE to include wayland libraries instead of lX11
LINUX_USE_WAYLAND ?= FALSE

# the binary filename
BIN = $(PROJECT)
# full path to the binary file
BIN_PATH = bin/$(BIN)

CXX = g++
RAYLIB_INCLUDE_PATH = lib/raylib/linux/include
RAYLIB_LIB_PATH = lib/raylib/linux/lib

LDLIBS = $(RAYLIB_LIB_PATH)/libraylib.a
LDLIBS += -lGL -lm -lpthread -ldl -lrt

ifeq ($(PLATFORM), PLATFORM_LINUX)

	RAYLIB_INCLUDE_PATH = lib/raylib/linux/include
	RAYLIB_LIB_PATH 	= lib/raylib/linux/lib
	
	ifeq ($(LINUX_USE_WAYLAND), FALSE)
		LDLIBS 			+= -lX11
	else ifeq ($(LINUX_USE_WAYLAND), TRUE)
		LDLIBS			+= -lwayland-client -lwayland-cursor
		LDLIBS			+= -lwayland-egl -lxkbcommon
	endif

	ifeq ($(BUILD_TYPE), BUILD_DEBUG)
		BIN_PATH 		= bin/linux/debug/$(BIN)
	else ifeq ($(BUILD_TYPE), BUILD_RELEASE)
		BIN_PATH		= bin/linux/release/$(BIN)
	endif

else ifeq ($(PLATFORM), PLATFORM_WINDOWS)

	BIN			= $(PROJECT).exe
	CXX 			= x86_64-w64-mingw32-g++
	RAYLIB_INCLUDE_PATH	= lib/raylib/windows/include
	RAYLIB_LIB_PATH 	= lib/raylib/windows/lib
	LDLIBS			= $(RAYLIB_LIB_PATH)/libraylib.a
	LDLIBS 		+= -lopengl32 -lgdi32 -lwinmm
	LDLIBS		+= -static -lpthread -static-libgcc -static-libstdc++

	ifeq ($(BUILD_TYPE), BUILD_DEBUG)
		BIN_PATH 		= ./bin/windows/debug/$(BIN)
	else ifeq ($(BUILD_TYPE), BUILD_RELEASE)
		BIN_PATH		= ./bin/windows/release/$(BIN)
	endif

endif

CXXFLAGS = -std=c++17 -Wall
INCLUDE_PATHS = -I$(RAYLIB_INCLUDE_PATH)

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,src/%.o,$(SRCS))

all: $(BIN)

$(BIN): $(OBJS) dirs assets
	$(CXX) -o $(BIN_PATH) $(OBJS) $(CXXFLAGS) $(INCLUDE_PATHS) $(LDLIBS)

dirs:
	mkdir -p bin/linux/debug/res
	mkdir -p bin/linux/release/res
	mkdir -p bin/windows/debug/res
	mkdir -p bin/windows/release/res

assets:
	rm -rf $(dir $(BIN_PATH))res/
	cp -rf res/ $(dir $(BIN_PATH))res/

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDE_PATHS)

clean:
	rm -f src/*.o
