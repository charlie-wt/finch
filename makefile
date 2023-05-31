# choose these bits ####################################################################

# basic options
CXX=g++
BASIC_FLAGS=-Wall -Werror -Wextra -Wpedantic -std=c++17 -MD -DNCURSES_WIDECHAR
SRC_DIR=.

# external dependencies
INC_DIR=../../Libraries
INC_NAMES=
LIB_NAMES=ncursesw

# debug-mode options
DEBUG_FLAGS=-g3 -DDEBUG
DEBUG_BUILD_DIR=.debug
DEBUG_EXE=termvis

# release-mode options
RELEASE_FLAGS=-O3
RELEASE_BUILD_DIR=.release
RELEASE_EXE=$(addprefix $(RELEASE_BUILD_DIR)/, termvis)

# ignore the rest ######################################################################



### other macros ###
SRC_NAMES = $(wildcard *.cc)
DBG_OBJ_NAMES = $(addprefix $(DEBUG_BUILD_DIR)/, $(SRC_NAMES:.cc=.o))
REL_OBJ_NAMES = $(addprefix $(RELEASE_BUILD_DIR)/, $(SRC_NAMES:.cc=.o))
DBG_DEP_NAMES = $(addprefix $(DEBUG_BUILD_DIR)/, $(SRC_NAMES:.cc=.d))
REL_DEP_NAMES = $(addprefix $(RELEASE_BUILD_DIR)/, $(SRC_NAMES:.cc=.d))

INC_FILES=$(addprefix $(INC_DIR)/, $(INC_NAMES))
INC_FLAGS=$(addprefix -I, $(INC_FILES))

LIB_FLAGS=$(addprefix -l, $(LIB_NAMES))

CXXFLAGS=$(BASIC_FLAGS) $(INC_FLAGS)


### rules ###
all: debug # `make` defaults to a debug build (`make debug`)

# debug
debug: BASIC_FLAGS += $(DEBUG_FLAGS)
debug: prep debug-exe

debug-exe: $(DBG_OBJ_NAMES)
	$(CXX) -o $(DEBUG_EXE) $^ $(LIB_FLAGS)

$(DEBUG_BUILD_DIR)/%.o: $(SRC_DIR)/%.cc $(INC_FILES)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# release
release: BASIC_FLAGS += $(RELEASE_FLAGS)
release: prep release-exe

release-exe: $(REL_OBJ_NAMES)
	$(CXX) -o $(RELEASE_EXE) $^ $(LIB_FLAGS)

$(RELEASE_BUILD_DIR)/%.o: $(SRC_DIR)/%.cc $(INC_FILES)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# other
prep:
	mkdir -p $(DEBUG_BUILD_DIR) $(RELEASE_BUILD_DIR)

.PHONY: clean
clean:
	$(RM) $(DEBUG_EXE)
	$(RM) $(RELEASE_EXE)
	$(RM) $(DBG_OBJ_NAMES)
	$(RM) $(REL_OBJ_NAMES)
	$(RM) $(DBG_DEP_NAMES)
	$(RM) $(REL_DEP_NAMES)

# this, along with the `-MD` in `BASIC_FLAGS` above, lets make detect changes in header
# files. since the filenames in {DBG,REL}_OBJ_NAMES contain their directory, we can
# simply include both here and it'll all work out.
-include $(DBG_OBJ_NAMES:.o=.d)
-include $(REL_OBJ_NAMES:.o=.d)