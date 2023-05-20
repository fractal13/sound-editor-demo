File format to describe a musical score
---------------------------------------

SCORE header
STAFF clause
STAFF clause
...

ASCII text file with the following lines:

    - Line 1: SCORE beats_per_bar beat_note_value beats_per_minute
    - Staff begin line: STAFF staff_name instrument_name
    - Staff note line: start_time duration_note_octave

Line 1 must be the SCORE line.
A staff note line must come inside a STAFF clause.
A staff clause begins with a STAFF begin line and ends with
the next STAFF begin line, or the end of file.

start_time is number of whole notes from the beginning of the
staff when the note starts to play.

duration_note_octave is a 3 element string, without any whitespace.

    - duration is the note duration as described below
    - note is a 1 or 2 character note descriptor as described below
    - octave is an integer number as described below.



Symbol | Note Duration Meaning
-------+----------------------
q      | quarter note
h      | half note
w      | whole note
e      | eighth note
x.     | dotted x note (1.5 times x's duration)


c = 2^(1/12) # The 12th root of 2
o = octave number

Symbol | Note within the Octave | Frequency
-------+------------------------+----------
A      | A                      | 27.5 * 2^o
A#     | A-sharp                | A * c
Bb     | B-flat                 | A * c
B      | B                      | A * c^2
C      | C                      | A * c^-9
C#     | C-sharp                | A * c^-8
Db     | D-flat                 | A * c^-8
D      | D                      | A * c^-7
D#     | D-sharp                | A * c^-6
Eb     | E-flat                 | A * c^-6
E      | E                      | A * c^-5
F      | F                      | A * c^-4
F#     | F-sharp                | A * c^-3
Gb     | G-flat                 | A * c^-3
G      | G                      | A * c^-2
G#     | G-sharp                | A * c^-1
Ab     | A-flat                 | A * c^-1

Symbol | A Frequency
-------+------------
0      | 27.5 Hz
1      | 55 Hz
2      | 110 Hz
3      | 220 Hz
4      | 440 Hz
5      | 880 Hz
6      | 1760 Hz
7      | 3520 Hz
8      | 7040 Hz
9      | 14080 Hz




Header information:

  - time signature
  - tempo

ASCII TEXT FORMAT:




Staff information:

STAFF instrument_name 





Staff Note
----------


Duration: 

    - q quarter note
    - h half note
    - w whole note
    - e eighth note
    - x. dotted x note (1.5 times duration)

Name/frequency

note name:

    - A
    - A#,Bb
    - B
    - C
    - C#,Db
    - D
    - D#,Eb
    - E
    - F
    - F#,Gb
    - G
    - G#,Ab

note octave:

    - 0 (C0 is 16.351 Hz, A0 is 27.5 Hz)
    - 1 (C1 is 32.703 Hz, A1 is 55 Hz)
    - 2 (C2 is 65.406 Hz, A2 is 110 Hz)
    - 3 (C3 is 130.813 Hz, A3 is 220 Hz)
    - 4 (C4 is 261.626 Hz, A4 is 440 Hz)
    - 5 (C5 is 523.251 Hz, A5 is 880 Hz)
    - 6 (C6 is 1046.502 Hz, A6 is 1760 Hz)
    - 7 (C7 is 2093.005 Hz, A7 is 3520 Hz)
    - 8 (C8 is 4186.009 Hz, A8 is 7040 Hz)
    - 9 (C9 is 8372.018 Hz, A9 is 14080 Hz)

start time:

The number of whole notes since the beginning of the staff/score
when the note is played. Floating point number.


Examples:

1.75 q C 4
3.25 w. Bb 3


FUTURE:

INSTRUMENT descriptions
ENVELOPE descriptions
WAVETABLE descriptions
MIXING descriptions
