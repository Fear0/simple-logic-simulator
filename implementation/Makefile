#include ../api/share/Makefile

# This works better with VS Code:

CXX = g++

SANITIZE := \
	-fsanitize=address \
	-fsanitize=leak \
	-fsanitize=undefined \
	-fsanitize=float-cast-overflow \
	-fno-omit-frame-pointer

CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -O0 \
	-Winit-self -Wold-style-cast -Wcast-qual -Wundef \
	-Woverloaded-virtual \
	-Wconversion \
	-Wlogical-op \
	-Wzero-as-null-pointer-constant \
	$(SANITIZE) \
	-I ../api/include 

LINK_FLAGS := $(SANITIZE) -L ../api/lib64 -lGraph -lLibrary -lParser -lUtils-STA

EXEC	:= eda-lab

LIB         := Analysis-EDA
LIB_SRCS	:= $(wildcard $(LIB)/*.cpp)
LIB_HDRS	:= $(wildcard $(LIB)/*.h)
LIB_OBJS	:= $(patsubst $(LIB)/%.cpp, $(LIB)/.obj/%.o, $(LIB_SRCS))
LIB_DEPS	:= $(patsubst %.o, %.d, $(LIB_OBJS))

SRCS	:= $(wildcard *.cpp)
HDRS	:= $(wildcard *.h)
OBJS	:= $(patsubst %.cpp, .obj/%.o, $(SRCS))
DEPS	:= $(patsubst %.o, %.d, $(OBJS))

# Default target
$(EXEC)	:

# Create all
all	: $(EXEC)

# Include existing dependancies
-include $(LIB_DEPS)
-include $(DEPS)

# Create dependancy files
.SUFFIXES += .d
.obj/%.d: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,.obj/%.o,$<)' $< -MF $@
$(LIB)/.obj/%.d: $(LIB)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst $(LIB)/%.cpp,($LIB).obj/%.o,$<)' $< -MF $@

# Compilation
.obj/%.o: %.cpp .obj/%.d
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(LIB)/.obj/%.o: $(LIB)/%.cpp $(LIB)/.obj/%.d
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
# Link LIB
$(LIB)/lib$(LIB).a	: $(LIB_OBJS)
	ar rcs $@ $(LIB_OBJS) 

# Link all object files to executable
$(EXEC)	: $(OBJS) $(LIB)/lib$(LIB).a
	$(CXX) $(OBJS) $(LINK_FLAGS) -L $(LIB) -l$(LIB) -o $@

# Documentation
.PHONY: doc
doc	: $(SRCS) $(HDRS)
	doxygen

# Debug makefile
.PHONY: test
test	:
	$(info $$LIB_SRCS [$(LIB_SRCS)])
	$(info $$LIB_HDRS [$(LIB_HDRS)])
	$(info $$LIB_OBJS [$(LIB_OBJS)])
	$(info $$LIB_DEPS [$(LIB_DEPS)])
	$(info $$SRCS [$(SRCS)])
	$(info $$HDRS [$(HDRS)])
	$(info $$OBJS [$(OBJS)])
	$(info $$DEPS [$(DEPS)])

# Clean up directory
.PONY: clean
clean	: 
	rm -f $(EXEC) 
	rm -rf $(LIB)/lib$(LIB).a
	rm -rf $(LIB)/.obj
	rm -rf .obj
	rm -rf doc
