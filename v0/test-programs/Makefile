LIBDIR := ../lib
INCDIR := ../include
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -Werror -I $(INCDIR) -g
LDFLAGS := -L $(LIBDIR) -lscore -linstrument -lenvelope -lwaveform
SRCS    := $(sort $(wildcard *.cpp))
MAINSRCS    := $(shell egrep 'int main' $(wildcard *.cpp) /dev/null | awk -F: '{print $$1;}' | uniq)
COMMON_SRCS := $(filter-out $(MAINSRCS),$(SRCS))
COMMON_OBJECTS := $(patsubst %.cpp,%.o,$(COMMON_SRCS))
LIBRARY_FILES := $(LIBDIR)/libenvelope.a $(LIBDIR)/libwaveform.a $(LIBDIR)/libinstrument.a  $(LIBDIR)/libscore.a
INCLUDE_FILES := $(wildcard ../include/*.h)

all: \
	test_adsr_envelope test_ad_envelope test_ar_envelope \
	test_sine_waveform test_waveform \
	test_instrument \
	test_score_reader

test-score: test_score_reader
	./test_score_reader

test_score_reader: test_score_reader.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_instrument: test_instrument.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_adsr_envelope: test_adsr_envelope.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_ad_envelope: test_ad_envelope.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

test_ar_envelope: test_ar_envelope.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)


adsr-envelope.pdf: test_adsr_envelope show_graph.py
	./test_adsr_envelope > adsr.csv
	./show_graph.py -d adsr.csv
	-rm -f adsr.csv

ad-envelope.pdf: test_ad_envelope show_graph.py
	./test_ad_envelope > ad.csv
	./show_graph.py -d ad.csv
	-rm -f ad.csv

ar-envelope.pdf: test_ar_envelope show_graph.py
	./test_ar_envelope > ar.csv
	./show_graph.py -d ar.csv
	-rm -f ar.csv

test_waveform: test_waveform.o $(COMMON_OBJECTS) $(LIBRARY_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(COMMON_OBJECTS) $(LDFLAGS)

sine-waveform.pdf: test_waveform show_graph.py
	./test_waveform sine > sine.csv
	./show_graph.py plot-waveform -d sine.csv
	-rm -f sine.csv

sawtooth-waveform.pdf: test_waveform show_graph.py
	./test_waveform sawtooth > sawtooth.csv
	./show_graph.py plot-waveform -d sawtooth.csv
	-rm -f sawtooth.csv

square-waveform.pdf: test_waveform show_graph.py
	./test_waveform square > square.csv
	./show_graph.py plot-waveform -d square.csv
	-rm -f square.csv

triangle-waveform.pdf: test_waveform show_graph.py
	./test_waveform triangle > triangle.csv
	./show_graph.py plot-waveform -d triangle.csv
	-rm -f triangle.csv

test-waveform: sine-waveform.pdf sawtooth-waveform.pdf square-waveform.pdf triangle-waveform.pdf

sine-adsr-instrument.pdf: test_instrument show_graph.py
	./test_instrument > sine-adsr.csv
	./show_graph.py plot-instrument -d sine-adsr.csv
	-rm -f sine-adsr.csv

square-adsr-instrument.pdf: test_instrument show_graph.py
	./test_instrument square ADSR > square-adsr.csv
	./show_graph.py plot-instrument -d square-adsr.csv
	-rm -f square-adsr.csv

sawtooth-ad-instrument.pdf: test_instrument show_graph.py
	./test_instrument sawtooth AD > sawtooth-ad.csv
	./show_graph.py plot-instrument -d sawtooth-ad.csv
	-rm -f sawtooth-ad.csv

triangle-ar-instrument.pdf: test_instrument show_graph.py
	./test_instrument triangle AR > triangle-ar.csv
	./show_graph.py plot-instrument -d triangle-ar.csv
	-rm -f triangle-ar.csv

test-instrument: sine-adsr-instrument.pdf square-adsr-instrument.pdf sawtooth-ad-instrument.pdf triangle-ar-instrument.pdf



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
	-rm -f *~
	-rm -rf $(DEPDIR)
#
#
######################################
