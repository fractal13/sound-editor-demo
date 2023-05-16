CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -Werror
LDFLAGS :=

all: C4 my_first my_second mary_song mary_refactor

wave: C4 my_first my_second mary_song mary_refactor
	./C4
	./my_first
	./my_second
	./mary_song
	./mary_refactor

mary_refactor.wav: mary_refactor
	./mary_refactor

C4: C4.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

my_first: my_first.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

my_second: my_second.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mary_song: mary_song.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

mary_refactor: mary_refactor.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

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
