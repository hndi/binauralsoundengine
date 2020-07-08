#The Binaural Engine allows to put several audio sources, reflecting walls, and microhpones in
#threedimensional space and calculates the spreading of the sound waves. When configured right,
#it allows you to perceive real depth of sound.

#At the current state it can only process and output a raw audio format (for input files: mono, 32 bit
#floating point values, output files can have two channels [see below]). Use Audacity i.e. for conversion.

#The executable needs a text file as parameter, which can contain the following data in sperarate lines:

#The "speaker" plays a sound file at a threedimensional position in a desired volume. The position is
#measured in meters (like all other position values).
#Format: speaker=<soundfile>;<X-Position>;<Y-Position>;<Z-Position>;<Volume>
#Example:
speaker=speakers/test.raw;1.0;1.0;1.75;1.0

#The "microphone" receives sound waves and saves them as a mono audio file. It can be positioned
#in threedimensional space. Up to 16 single microphones are currently allowed.
#Format: microphone=<soundfile>;<X-Position>;<Y-Position>;<Z-Position>
#Example:
microphone=result.raw;0.0;0.0;1.7

#The "2chanmic" saves a sound file with two channels and needs two threedimensional positions
#as microphone positions.
#Format: 2chanmic=<soundfile>;<Mic 1 X>;<Mic 1 Y>;<Mic 1 Z>;<Mic 2 X>;<Mic 2 Y>;<Mic 2 Z>
#Example:
2chanmic=output/output.raw;-0.08;0.0;1.75;0.08;0.0;1.75

#A "head" is an object in threedimensional space that comes with two microphones as ears, can save a stereo
#file and can be rotated from 0 to 360 degrees. An ear distance of 0.18 meters should deliver the most
#realistic results.
#Format: head=<soundfile>;<X-Position>;<Y-Position>;<Z-Position>;<Angle>;<Ear Distance>
#Example:
head=output/output.raw;1.0;0.0;1.7;180;0.18

#A "wall" is a flat surface that reflects the sound to other walls and to microphones.
#The entire soundwave is reflected when the amount value is at 1, non of the sound is reflected when the 
#value is at 0. You can use any floating point value between 0 and 1. The wall needs two threedimensional
#positions while one axis has to be the same values. Use the same x-values for side walls, the same y-
#values for frontal walls and the same z-values for floors and ceilings. For realistic results, the
#amount value should be pretty high, between 0.8 and 0.99.
#Format: wall=<X-Position 1>;<Y-Position 1>;<Z-Position 1>;<X-Position 2>;<Y-Position 2>;<Z-Position 2>;<Amount>
#Example (for a floor):
wall=-5.0;-1.0;0.0;5.0;6.0;0.0;0.9


#A "room" creates 6 walls with identical reflection amounts. It requires two threedimensional positions which
#should describe the lower left near and the upper right far point of a volume, as well as a reflection amount. The reflection amount is a value between 0 and 1.
#Format: room=<X-Position 1>;<Y-Position 1>;<Z-Position 1>;room=<X-Position 2>;<Y-Position 2>;<Z-Position 2>;<Amount>
#Example:
room=-5.0;-1.0;0.0;5.0;6.0;3.0;0.9

#"nrbounces" is the number of reflection bounces between walls. The default value is 50. Smaller rooms
#require more bounces than large rooms. Note that te render time will increase with the number of bounces.
#Format: nrbounces=<number of bounces>

#"nrreflections" determines the number of total reflection calculations before the engine finally saves the
#microphone files and quits. The larger a room is, the more reflection passes are needed to sound well.
#The total render time will increase with a rising number of reflections.
#Format: nrreflections=<number of reflections>

#"saveeverynrofpasses", when not set to zero, saves the microphone audiofiles after a given number of
#reflection passes to offer the ability to hear the current result while still calculating and improving
#the result. The rendering will continue after saving. Note that the total render time increases a bit
#since the saving process takes some time.
#Format: saveeverynrofpasses=<number of passes>

#The "samplerate" value sets the sample rate of the input/output audio files. The default value is 44100.
#Format: samplerate=<sample rate>

#"additionaltime" sets an additional time to the output audio to allow echoes to complete. For a longer
#reflection (in in big rooms or small rooms with many bounces and reflection amounts) a longer additional
#audio time is required. The value is a floating point with a default value of 30 seconds.
#Format: additionaltime=<additional time in seconds>
