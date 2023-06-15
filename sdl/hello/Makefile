LIBDIR := ../lib
INCDIR := ../include
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -Werror -I $(INCDIR) -g
# -lscore -linstrument -lenvelope -lwaveform -laudiofiles -lmidifile
LDFLAGS := -L $(LIBDIR) -lSDL2 -lSDL2_mixer
SRCS    := $(sort $(wildcard *.cpp))
MAINSRCS    := $(shell egrep 'int main' $(wildcard *.cpp) /dev/null | awk -F: '{print $$1;}' | uniq)
COMMON_SRCS := $(filter-out $(MAINSRCS),$(SRCS))
COMMON_OBJECTS := $(patsubst %.cpp,%.o,$(COMMON_SRCS))
# $(LIBDIR)/libenvelope.a $(LIBDIR)/libwaveform.a $(LIBDIR)/libinstrument.a  $(LIBDIR)/libscore.a $(LIBDIR)/libaudiofiles.a $(LIBDIR)/libmidifile.a
LIBRARY_FILES :=
INCLUDE_FILES := $(wildcard ../include/*.h)
TARGETS := \
	main main_event main_mixer

all: $(TARGETS)

main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

main_event: main_event.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

main_mixer: main_mixer.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)




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

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(COMMON_SRCS))))
include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(MAINSRCS))))
#
#
######################################

######################################
#
#
clean:
	-rm -f $(TARGETS)
	-rm -f *.o
	-rm -f *~ .*~
	-rm -rf $(DEPDIR)
	-rm -f *.wav
	-rm -f *.pdf
#
#
######################################
