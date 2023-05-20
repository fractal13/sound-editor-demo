LIBRARY_FILES := ../library/libsynth.a
INCLUDE_DIRS := -I ../library
LIBRARY_DIRS := -L ../library

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -Werror -g $(INCLUDE_DIRS)
LDFLAGS := $(LIBRARY_DIRS) -lsynth

all: mary_lib mary_score mary_file

mary_lib: mary_lib.o $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mary_lib.wav: mary_lib
	./mary_lib

mary_score: mary_score.o $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mary_score.wav: mary_score
	./mary_score

mary_file: mary_file.o $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mary_file.wav: mary_file ../doc/mary-had-a-little-lamb.score
	./mary_file

play: mary_file.wav
	play mary_file.wav

######################################
# automatic dependency generation
#
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
#
#
######################################

######################################
#
#
clean:
	-rm -f $(TARGETS)
	-rm -f *.o
	-rm -f *~
	-rm -rf $(DEPDIR)
#
#
######################################
