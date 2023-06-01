LIBDIR := ../lib
INCDIR := ../include
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -Werror -I $(INCDIR) -g
LDFLAGS := -L $(LIBDIR) -lscore -linstrument -lenvelope -lwaveform -laudiofiles
SRCS    := $(sort $(wildcard *.cpp))
MAINSRCS    := $(shell egrep 'int main' $(wildcard *.cpp) /dev/null | awk -F: '{print $$1;}' | uniq)
COMMON_SRCS := $(filter-out $(MAINSRCS),$(SRCS))
COMMON_OBJECTS := $(patsubst %.cpp,%.o,$(COMMON_SRCS))
LIBRARY_FILES := $(LIBDIR)/libenvelope.a $(LIBDIR)/libwaveform.a $(LIBDIR)/libinstrument.a  $(LIBDIR)/libscore.a $(LIBDIR)/libaudiofiles.a
INCLUDE_FILES := $(wildcard ../include/*.h)
TARGETS := \
	test_envelope \
	test_waveform \
	test_instrument \
	test_staff_render \
	test_score_reader \
	test_score_writer

all: $(TARGETS)

test-all: test-waveform test-envelope test-instrument test-render test-score

test_score_reader: test_score_reader.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_score_writer: test_score_writer.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_staff_render: test_staff_render.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_instrument: test_instrument.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_envelope: test_envelope.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_waveform: test_waveform.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

#
test-envelope: ADSR-envelope.pdf AD-envelope.pdf AR-envelope.pdf

%-envelope.pdf: test_envelope show_graph.py
	./test_envelope $* > $*.csv
	./show_graph.py -d $*.csv
	-rm -f $*.csv


#
test-waveform: sine-waveform.pdf sawtooth-waveform.pdf square-waveform.pdf triangle-waveform.pdf

%-waveform.pdf: test_waveform show_graph.py
	./test_waveform $* > $*.csv
	./show_graph.py plot-waveform -d $*.csv
	-rm -f $*.csv

#
test-instrument: sine-ADSR-instrument.pdf square-ADSR-instrument.pdf sawtooth-AD-instrument.pdf triangle-AR-instrument.pdf \
	square-AD-instrument.pdf

%-instrument.pdf: test_instrument show_graph.py
	./test_instrument $(word 1,$(subst -, ,$*)) $(word 2,$(subst -, ,$*)) > $*.csv
	./show_graph.py plot-instrument -d $*.csv
	-rm -f $*.csv

#
test-render: scale-render.pdf

%-render.pdf: test_staff_render ../doc/%.score
	./test_staff_render 16 ../doc/$*.score > $*.csv
	./show_graph.py plot-render -d $*.csv
	-rm -f $*.csv

#
test-score: mary-had-a-little-lamb.wav the-bare-neccessities.wav notes.wav linus-lucy.wav

%.wav: test_score_reader ../doc/%.score
	./test_score_reader 16 ../doc/$*.score $@

test-score-writer: mary-had-a-little-lamb.score the-bare-neccessities.score notes.score linus-lucy.score

notes.score: test_score_writer ../doc/notes.score
	./test_score_writer ../doc/notes.score notes.score

# %.score: test_score_writer ../doc/%.score
# 	./test_score_writer ../doc/$*.score $@

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
