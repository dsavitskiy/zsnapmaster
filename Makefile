 
CXX := clang++
LD := clang++


TGT := zsnapmaster
INCPATH := \
	-I/usr/include/libzfs \
	-I/usr/include/libspl
	
DEFINES := \
	-DHAVE_IOCTL_IN_SYS_IOCTL_H
	
LIBS := -lzfs -lzpool -lnvpair -lc++
	
SRC := \
	zsm.cpp \
	zsm_options.cpp \
	zsm_create.cpp \
	zsm_destroy.cpp \
	zsm_list.cpp \
	zsnapmaster.cpp

CXXFLAGS := -stdlib=libc++ -std=c++11 $(DEFINES) $(INCPATH)

OBJ := $(addprefix tmp/, $(SRC:%.cpp=%.opp))
HEADER_DEPS	:= $(OBJ:.opp=.dpp)

.PHONY: all
all: dirs $(DEPS) $(TGT)

$(TGT): $(OBJ)
	@echo "link    : $(TGT)"
	@$(LD) $(LIBS) $(OBJ) -o $@ 	
	
tmp/%.opp: %.cpp
	@echo "compile : $(notdir $<)"
	@$(CXX) $(CXXFLAGS) $< -M -MP -MT $@ > tmp/$*.dpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: dirs
dirs:
	@mkdir -p tmp

.PHONY: clean
clean:
	@echo "clean   : $(TGT)"
	@rm -f tmp/*.opp tmp/*.dpp
	@rmdir tmp
	@rm -f $(TGT)
	
-include $(HEADER_DEPS)
	