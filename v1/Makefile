all: build
	make -C test-programs test-all

build:
	make -C library-waveform install
	make -C library-envelope install
	make -C library-instrument install
	make -C library-midifiles install
	make -C library-score install
	make -C library-audiofiles install
	make -C test-programs

clean:
	make -C library-waveform clean
	make -C library-envelope  clean
	make -C library-instrument clean
	make -C library-score clean
	make -C library-audiofiles clean
	make -C test-programs clean


test: build
	make -C test-programs test-midi-reader
