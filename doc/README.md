Capabilities
------------

    - Create instruments
    - Visualize an instrument
    - Create songs
    - Visualize a song
    - Store song in file
    - Read song file format to store notes/instruments/etc into a song description object.
    - Generate digital audio to approximate the sounds from the song. (half-check)
    - Mixer(s)
    - Wave tables
    - Save the sounds to a file. (check)
    - Alternate file formats.


Create Instruments
------------------

Describe envelopes, waveforms, etc. necessary for an instrument.
Some instruments will be completely hard coded, and the user can access them.
Other instruments will be configurable by the user, and the user can access multiple versions of them.

Visualize an instrument
-----------------------

Some UI display of the envelopes and waveforms available for an instrument.

Create Songs
------------

User can describe the meter, instruments, and notes that go into a song.
The song can be built up incrementally.

The user can use high-level ideas to build song elements.

Algorithms can build songs.


Visualize a song
----------------

Some UI display of the instruments and notes of a song.


Song Files
----------

Either create a sound file format, or learn a simple one that can be read.

Requirements:

    - Describe meter (quarter notes/minute)
    - Describe note start time, frequency, duration, and instrument.
    - Allow for multiple simultaneous notes.
    
Not necessarily required, but might be cool:

    - non-standard note duration
    - sliding notes
    - other stuff

Generating Audio
----------------

Use generators to build wave tables for instrument sounds.  Sine, Sawtooth, etc.
Use envelopes to control amplitude of individual notes. ADSR, etc. Linear, exponential, etc.
Use mixers to join multiple simultaneous notes.

Assumptions/Questions:

    - wave table generators and envelopes should be influenced by the instrument.
    - how do mixers handle relative volume/loudness coupled with overall volume?

Maybe use envelopes to control frequencies???

Mixer(s)
--------

Mix multiple staff output into stream.
Mix multiple notes from one instrument into stream.
Mix left/right stereo.


Wave tables
-----------

Use wave tables instead of infinite repeating functions.
    
Saving Audio to Files
----------------------

Initially use the WAV file format with PCM audio. 
Be able to use many of the header configurable features, such as sample bit size,
number of channels, etc.


Alternate file formats
----------------------

Use 3rd party libraries to read/write other audio file formats.

