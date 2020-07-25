The Binaural Engine allows to put several audio sources, reflecting walls, and microhpones in
threedimensional space and calculates the spreading of the sound waves. When configured right,
it allows you to perceive real depth of sound when you listen to the result via headphones.

The executable needs a text file as parameter, which can contain the data listed and described below.

At the current state it can only process and output a raw audio format (for input files: mono, 32 bit
floating point values, output files can have two channels [see below]).

I recommend using audacity for conversion, Download at: https://www.audacityteam.org/
To import a resulting RAW file, go to File -> Import -> Raw Data. Select your file and open it. Select
'32-bit float' and '2 Channels (Stereo)' or '1 Channel (Mono)', depending on the content of the
sound file.
If you want to convert an MP3, WAV (etc) to a RAW file the engine can process, open it in audacity,
convert it to mono (Click on the triangle on the left of your track and then 'Split Stereo to Mono'),
delete one of the mono tracks, select the part you want do use in the Binaural Engine and then
File->Export Selected Audio. Select 'Other uncompressed files', below on 'RAW (header-less)'
and 32-bit-float, then save.

------ The following data can be put into the text file that is processed by the filter ------

The "speaker" plays a sound file at a threedimensional position in a desired volume. The position is
measured in meters (like all other position values). The start delay (todo!) is optional as well as the
keyframes (see below).
Format: speaker=<soundfile>;<X-Position>;<Y-Position>;<Z-Position>;<Volume>;[Start delay];[Keyframes]
Example:
speaker=test.raw;1.0;1.0;1.75;1.0

Speaker keyframes: These allow movement of the speaker object and are a list of timestamps and
positions relative to the origin of the speaker. The engine interpolates the positions linearly.
A keyframe requires a time stamp and an x, y, z position, separated by a comma (not semicolon!).
Each keyframe is separated by a slash. Note that the rendering time increases when using movement
since the position has to be recalculated for every sound sample.
This example moves the speaker 3 meters to the right, 3 to the front and then 3 to the top:
speaker=test.raw;1.0;1.0;1.75;1.0;;1.0,3.0,0.0,0.0/3.5,3.0,3.0,0.0/6.3,3.0,3.0,3.0

The "microphone" receives sound waves and saves them as a mono audio file. It can be positioned
in threedimensional space. Up to 16 single microphones are currently allowed.
Format: microphone=<soundfile>;<X-Position>;<Y-Position>;<Z-Position>
Example:
microphone=result.raw;0.0;0.0;1.7

The "2chanmic" saves a sound file with two channels and needs two threedimensional positions
as microphone positions.
Format: 2chanmic=<soundfile>;<Mic 1 X>;<Mic 1 Y>;<Mic 1 Z>;<Mic 2 X>;<Mic 2 Y>;<Mic 2 Z>
Example:
2chanmic=output/output.raw;-0.08;0.0;1.75;0.08;0.0;1.75

A "head" is an object in threedimensional space that comes with two microphones as ears, can save a stereo
file and can be rotated from 0 to 360 degrees. An ear distance of 0.18 meters should deliver the most
realistic results.
Format: head=<soundfile>;<X-Position>;<Y-Position>;<Z-Position>;<Angle>;<Ear Distance>
Example:
head=output/output.raw;1.0;0.0;1.7;180;0.18

A "wall" is a flat surface that reflects the sound to other walls and to microphones.
The entire soundwave is reflected when the amount value is at 1, non of the sound is reflected when the 
value is at 0. You can use any floating point value between 0 and 1. The wall needs two threedimensional
positions while one axis has to be the same values. Use the same x-values for side walls, the same y-
values for frontal walls and the same z-values for floors and ceilings. For realistic results, the
amount value should be pretty high, between 0.8 and 0.99.
Format: wall=<X-Position 1>;<Y-Position 1>;<Z-Position 1>;<X-Position 2>;<Y-Position 2>;<Z-Position 2>;<Amount>;[modifier]
Example (for a floor):
wall=-5.0;-1.0;0.0;5.0;6.0;0.0;0.9

The optional modifier value for a wall  is used if you don't want to have a perfectly straight wall,
which can lead to unrealistic smooth reverb in very big rooms or on big surfaces like house walls or
mountains. The surface can be modified by three different shapes: sine, square and triangle. You can
modify the length of these shapes and the depth (in meters). The pattern can be shifted by a given
amount of meters.
Format: <type>,<langth>,<depth>,<shift>
Example: wall=-5.0;-1.0;0.0;5.0;6.0;0.0;0.9;square,10,3,0

A "room" creates 6 walls with identical reflection amounts. It requires two threedimensional positions which
should describe the lower left near and the upper right far point of a volume, as well as a reflection amount.
The reflection amount is a value between 0 and 1.
Format: room=<X-Position 1>;<Y-Position 1>;<Z-Position 1>;room=<X-Position 2>;<Y-Position 2>;<Z-Position 2>;<Amount>
Example:
room=-5.0;-1.0;0.0;5.0;6.0;3.0;0.9

The "rndspawnspeaker" is an advanced speaker with a lot of parameters that spawns with random delays in
random locations and plays in varying speeds. It can be used for water drops, birds, leaf sounds and so on.
It requires two positions that describe the corners of a cube in which the speakers spawn, a maximum
playback time (in seconds), the volume, the min/max pause time between spawns and the optional min/max
playback speed (1 is the default value).
Format: randomspawnspeaker=<file>;<X-Pos 1>;<Y-Pos 1>;<Z-Pos 1>;<X-Pos 2>;<Y-Pos 2>;<Z-Pos 2>;<Volume>;<Total Time>;<Min Pause>;<Max Pause>;[Min Speed];[Max Speed]
Example:
randomspawnspeaker=test.raw;-10;0;0;10;20;5;1;60;3;6;0.8;1.2

"nrbounces" is the number of reflection bounces between walls. The default value is 50. Smaller rooms
require more bounces than large rooms. Note that te render time will increase with the number of bounces.
Format: nrbounces=<number of bounces>

"nrreflections" determines the number of total reflection calculations before the engine finally saves the
microphone files and quits. The larger a room is, the more reflection passes are needed to sound well.
The total render time will increase with a rising number of reflections.
Format: nrreflections=<number of reflections>

"saveeverynrofpasses", when not set to zero, saves the microphone audiofiles after a given number of
reflection passes to offer the ability to hear the current result while still calculating and improving
the result. The rendering will continue after saving. Note that the total render time increases a bit
since the saving process takes some time.
Format: saveeverynrofpasses=<number of passes>

The "samplerate" value sets the sample rate of the input/output audio files. The default value is 44100.
Format: samplerate=<sample rate>

"additionaltime" sets an additional time to the output audio to allow echoes to complete. For a longer
reflection (in in big rooms or small rooms with many bounces and reflection amounts) a longer additional
audio time is required. The value is a floating point with a default value of 30 seconds.
Format: additionaltime=<additional time in seconds>
