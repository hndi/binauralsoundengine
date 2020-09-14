/*

This binaural sound filter renders binaural output files out of a 3D scene that contains
"speakers" (sound sources) and reflecting walls.

The BinauralEngine class receives and manages Speaker, Microphone, and Wall objects and performs
the desired calculations. Just create a BinauralEngine object, add speakers with addSpeaker,
walls with addWall, microphones with addMicrophone, change the default settings by changing
the .settings values of the object. Use the start method to start the engine (and let it start
its speakers/microphones), the update function once (at the current state - later periodically)
to let it do the calculations, and the stop function to stop the process and write the output files.

I published this under the GNU GPL v3.0, read the details here:
https://www.gnu.org/licenses/gpl-3.0.en.html
Feel free to use the code for your projects, add new features, fix bugs etc.

I am happy about feedback, ideas and code improvements. You can find the project on github:
https://github.com/hndi/binauralsoundengine

To compile it, g++ worked pretty well when adding -std=c++11 to the command line.

Ideas for future versions:
* Threads! I didn't figure out how to avoid memory collisions while maintaining a high rendering speed yet
* Right now, reflection spots are selected on random walls. It would be better if rays were shooting out
of speakers and try to hit other walls and reflect from there. This would allow blocking walls, corners and
indirect reflections.
* Reduction of higher sound frequencies over distance and by reflection. In the real world, high frequencies
are reduced, which is not yet happening in this softare, which can lead to unrealistic high reverb sounds.
Didn't figure out a satisfying and high performance way to do that yet.
* Encoding/Decoding of MP3 and other formats.
* The possibility to use this program as a filter in sound software or inside any other software. Or at least
an external GUI launcher program that comfortably lets you create and edit the 3D scene.

*/


#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <stdint.h>
#include <math.h>

#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ios;
using std::streampos;
using std::ifstream;

static const float MPS = 343.0; //Speed of sound in air: 343 meters per second

double floatRand() {
    return (double)(((uint32_t)(rand() & 0x7FFF) << 15) | (rand() & 0x7FFF)) / 0x3FFFFFFF;
}

/* The Pos class just conains threedimensional positions and offers a function that allows to calculate the
distance beween two pos objects and the time distance in samples, depending on the speed of sound and the
sound sample rate.
*/
class Pos {
    public:
        float x = 0;
        float y = 0;
        float z = 0;

        void set(double xPos, double yPos, double zPos);
        float calcDistance(Pos p);
        static float calcDistance(Pos p1, Pos p2);
        static int32_t distanceToSamples(float dist, uint32_t sampleCnt);
};

void Pos::set(double xPos, double yPos, double zPos) {
    x = xPos;
    y = yPos;
    z = zPos;
}

float Pos::calcDistance(Pos p) {
    return sqrt(pow(x - p.x, 2.0) + pow(y - p.y, 2.0) + pow(z - p.z, 2.0));
}


float Pos::calcDistance(Pos p1, Pos p2) {
    return sqrt(pow(p2.x - p1.x, 2.0) + pow(p2.y - p1.y, 2.0) + pow(p2.z - p1.z, 2.0));
}

int32_t Pos::distanceToSamples(float dist, uint32_t sampleCnt) {
    return round(dist / MPS * sampleCnt);
}

/* The Microphone class is used by the BinauralEngine class and contains two float buffers. One for
sound samples that directly come from Speaker objects (drctBuffer) and one (reflBuffer) for indirect sound
samples that come from Wall objects (reflections). The different kind of samples are calculated and written
into the buffer by the BinauralEngine class (function calcSamples).
A Microphone can be used alone as well, just write into the drctBuffer.
You need to provide an output file in the constructor or later via setOutputfile, or you can provide a second
Microphone object via addSecondChanMic, which is then used as a second channel and written in the sound file.
If you create microphone that is later supposed to be assigned as second channel for another microhpone, just
leave the outputFile empty and pass its pointer to the primary microphone (that has an outputfile).
At the current state, the microphone just supports RAW files with 32-bit-floating-point samples.

Use the start function to initialize everything, including memory allocation, and stop to stop everything and
clear the memory. If you wish to write the current memory to a file, just use writeFile or set writeFile
to true in the stop function
*/

class Microphone {
    public:
        static const uint8_t MIC_IDLE = 0;
        static const uint8_t MIC_RECORDING = 1;
        static const uint8_t MIC_FAILED = 2;

        Microphone();
        Microphone(string outputFile, double x, double y, double z);
        ~Microphone();
        void setPos(double x, double y, double z);
        void setOutputFile(string outputFile);
        void reset();
        int8_t start(uint32_t sampleCnt, double reflectArea = 0.0);
        void stop(bool writeFile);
        uint8_t writeFile();
        void addSecondChanMic(Microphone *mic2);
        void removeSecondChanMic();

        Pos pos;
        float *drctBuffer = nullptr;
        float *reflBuffer = nullptr;
        uint32_t sampleCount = 0;
        uint32_t totalReflections;
        double totalReflectArea;

    private:
        string fileName = "";
        uint8_t status = MIC_IDLE;
        Microphone *secondChanMic;
};

Microphone::Microphone() {
    reset();
}

Microphone::Microphone(string outputFile, double x, double y, double z) {
    reset();
    setPos(x, y, z);
    setOutputFile(outputFile);
}

Microphone::~Microphone() {
    reset();
}

int8_t Microphone::start(uint32_t sampleCnt, double reflectArea) {
    totalReflectArea = reflectArea;
    if (status != MIC_RECORDING) {

        uint32_t sizeOfBuffer = sampleCnt * sizeof(float);
        reflBuffer = (float *)malloc(sizeOfBuffer);
        drctBuffer = (float *)malloc(sizeOfBuffer);
        sampleCount = sampleCnt;

        for (uint32_t i = 0; i < sampleCount; i++) {
            drctBuffer[i] = 0.0;
            reflBuffer[i] = 0.0;
        }
        totalReflections = 0;
        status = MIC_RECORDING;
        return status;
    }
    return status;
}

void Microphone::stop(bool wf) {
    if (wf == true) {
        writeFile();
    }
    reset();
}


void Microphone::reset() {
    if (reflBuffer != nullptr) {
        free(reflBuffer);
        reflBuffer = nullptr;
    }
    if (drctBuffer != nullptr) {
        free(drctBuffer);
        drctBuffer = nullptr;
    }
    secondChanMic = nullptr;
    status = MIC_IDLE;
}

void Microphone::setPos(double x, double y, double z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

void Microphone::setOutputFile(string outputFile) {
    fileName = outputFile;
}

void Microphone::addSecondChanMic(Microphone* mic2) {
    secondChanMic = mic2;
}

void Microphone::removeSecondChanMic() {
    secondChanMic = nullptr;
}

uint8_t Microphone::writeFile() {

    if (status == MIC_RECORDING && fileName != "") {
        ofstream out(fileName.c_str(), ios::binary);

        /* just tried this value and got the best result. Todo: use Settings to be able to change it.
        The value adjusts the relation between direct and indirect samples */
        float reflectAreaFactor = 3.5;

        /* Here the the volume of reflection samples is adjusted to maintain the same volume for indirect
        samples, independent from the number of reflection passes. Otherwise reflections would get louder
        by the number of passes. */
        float reflectorFactor = 1.0 / pow(totalReflections, 0.5) * reflectAreaFactor;
        if (totalReflectArea == 0 || totalReflections == 0) {
            reflectAreaFactor = 1.0;
            reflectorFactor = 1.0;
        }

        uint32_t bufferSize = 10240;
        uint32_t stepSize = 1;
        if (secondChanMic != nullptr) {
            bufferSize *= 2;
            stepSize = 2;
        }

        float buffer[bufferSize];
        for (uint32_t buffPos = 0; buffPos < sampleCount - bufferSize / stepSize; buffPos += bufferSize / stepSize) {
            if (buffPos + bufferSize / stepSize > sampleCount) {
                /* Somewhere in this part there is still a bug that cuts off the end of the sound file
                when the last part of the buffer is written */
                bufferSize = sampleCount - buffPos / stepSize;
            }
            for (uint32_t x = 0; x < bufferSize / stepSize; x++) {
                buffer[x * stepSize] = drctBuffer[buffPos + x] + (reflBuffer[buffPos + x] * reflectorFactor);
                if (buffer[x * stepSize] > 5) cout << "BUFF: " << buffer[x * stepSize] << '\t';
                if (secondChanMic != nullptr) {
                    buffer[x * stepSize + 1] = secondChanMic->drctBuffer[buffPos + x] + (secondChanMic->reflBuffer[buffPos + x] * reflectorFactor);
                }
            }

            /* Write the float buffer into the file */
            const char *tmp = (char *)&buffer;
            out.write(tmp, bufferSize * sizeof(float));
        }

        out.close();
    }
    return 0;
}

/* The Speaker class can load a sound file (only RAW format with one channel and 32-bit-float values is
curcurrently supported) and keeps it in the memory as float array pointer. (called samples) It is used by
the BinauralEngine class as source for sound samples and needs to be positioned in threedimensional space
via the pos object (that is set by the addSpeaker function by the BinauralEngine Class).
The Speaker can be used alone if you just wish to load a file and hold it in memory.
Use the constructor to pass a file name or use setFileName later. Use start() to load the file and check its
return value for success. Use the stop function to clear the memory, or just delete the object.
If you wish to read the samples, just read the values of samples[x], the total count of samples is saved in
sampleCount.
*/

class Speaker {
    public:
        static const int8_t STATUS_NO_FILE = 0;
        static const int8_t STATUS_FILE_LOADED = 1;
        static const int8_t STATUS_FILE_FAILED = 2;


        Speaker();
        Speaker(string fileName);
        ~Speaker();

        void setFileName(string fileName);
        int8_t status();
        int8_t start();
        int32_t getSize();
        void stop();

        Pos pos;
        float volume = 1.0;
        float *samples = nullptr;
        uint32_t sampleCount;
    private:
        string fileName;


        int8_t fileStatus = STATUS_NO_FILE;
};

Speaker::Speaker() {
}

Speaker::Speaker(string fileName) {
    setFileName(fileName);
}

Speaker::~Speaker() {
    stop();
}

int8_t Speaker::status() {
    return fileStatus;
}

void Speaker::setFileName(string fn) {
    fileName = fn;
}

int8_t Speaker::start() {
    streampos size;
    ifstream file(fileName.c_str(), ios::in|ios::binary|ios::ate);

    if (samples != nullptr) {
        free(samples);
        samples = nullptr;
    }

    if (file.is_open())  {
        size = file.tellg();
        file.seekg (0, ios::beg);
        sampleCount = size / sizeof(float);
        samples = (float *)malloc(sizeof(float) * (sampleCount + 1));
        file.read((char *)samples, size);
        file.close();
        fileStatus = STATUS_FILE_LOADED;
        for (uint32_t i = 0; i < sampleCount; i++) {
            samples[i] *= volume;
        }
    } else {
        fileStatus = STATUS_FILE_FAILED;
    }
    return fileStatus;
}

void Speaker::stop() {
    if (samples != nullptr) {
        free(samples);
        samples = nullptr;
    }
    fileStatus = STATUS_NO_FILE;
}

int32_t Speaker::getSize() {
    if (fileStatus == STATUS_FILE_LOADED) {
        return sampleCount;
    } else {
        return -1;
    }
}

/* A Wall modifier is used to modify the shape of the wall to improve the realism of
reflections of walls. It is used by the Wall class. */
class WallModifier {
    public:
        static const uint8_t MOD_NO_MOD = 0;
        static const uint8_t MOD_SINE = 1;
        static const uint8_t MOD_TRIANGLE = 2;
        static const uint8_t MOD_SQUARE = 3;

        double getModifiedAmplitude(double position);
        void configByString(string cfg);

        uint8_t usedModifier = MOD_NO_MOD;
        double waveLength = 1.0;
        double amplitude = 1.0;
        double shift = 0.0;
};

/* This function returns a modified amplitude, depending on the modifier settings.
This result is used to find the desired reflector position on the wall in
BinauralEngine::calcSamples() */
double WallModifier::getModifiedAmplitude(double position) {
    double tmp;

    switch (usedModifier) {
        case MOD_NO_MOD:
            return 0.0;
        case MOD_SINE:
            return sin((position + shift / 2.0) * M_PI * 4.0 / waveLength) * amplitude / 2;
        case MOD_SQUARE:
            if ((uint32_t)((position + shift) / (waveLength / 2.0)) % 2 == 0) {
                return amplitude * .5;
            } else {
                return amplitude * -.5;
            }
        case MOD_TRIANGLE:
            position = abs(position);
            tmp = (position + shift) / (waveLength / 2);
            if ((uint32_t)tmp % 2 == 0) {
                return (tmp - (uint32_t)tmp) * amplitude - amplitude / 2;
            } else {
                return (1.0 - (tmp - (uint32_t)tmp)) * amplitude  - amplitude / 2;
            }
    }

    return 0.0;
}

/* This sets up the wall modifiers by a string saved in a config file. The format is
"type,wavelength,amplitude,shift" while type can be sine, square, triangle and the other
values are measured in meters */
void WallModifier::configByString(string cfg) {
    uint16_t posV2, oldPosV2 = 0;
    string currValue;
    for (uint8_t valNr = 0; valNr < 4; valNr++) {
        posV2 = cfg.find(",", oldPosV2);
        if (posV2 == -1) {
            currValue = cfg.substr(oldPosV2, cfg.length() - oldPosV2);
        } else {
            currValue = cfg.substr(oldPosV2, posV2 - oldPosV2);
        }
        switch (valNr) {
        case 0:
            usedModifier = MOD_NO_MOD;
            if (currValue == "sine") {
                usedModifier = MOD_SINE;
            }
            if (currValue == "square") {
                usedModifier = MOD_SQUARE;
            }
            if (currValue == "triangle") {
                usedModifier = MOD_TRIANGLE;
            }
            break;
        case 1:
            waveLength = atof(currValue.c_str());
            break;
        case 2:
            amplitude = atof(currValue.c_str());
            break;
        case 3:
            shift = atof(currValue.c_str());
            break;
        }
        oldPosV2 = posV2 + 1;
        if (posV2 == -1) {
            break;
        }
    }
}


/* The Wall class just contains two 3D spots, saved as Pos objects and a function to calculate the total area
of the wall. Walls are supposed to be flat, so the values of one axis should be equal (i.e. if you have a floor,
the z values have to be the same).
This objects of this class are used by the BinauralEngine to know where reflective walls should be placed in
space and find reflection spots for sound waves.
*/
class Wall {
    public:
        static const uint8_t TYPE_UNKNOWN = 0;
        static const uint8_t TYPE_TOP = 1;
        static const uint8_t TYPE_SIDE = 2;
        static const uint8_t TYPE_FRONT = 3;

        Pos posStart;
        Pos posEnd;
        double area = 0.0;
        double reflectionAmount = 0.9;
        WallModifier modifier;

        double calcArea();
        uint8_t getWallType();
};

double Wall::calcArea() {
    area = abs(posEnd.x - posStart.x) * abs(posEnd.y - posStart.y)
            + abs(posEnd.x - posStart.x) * abs(posEnd.z - posStart.z)
            + abs(posEnd.y - posStart.y) * abs(posEnd.z - posStart.z);
    return area;
}

uint8_t Wall::getWallType() {
    if (abs(posStart.z - posEnd.z) < .1) {
        return TYPE_TOP;
    }
    if (abs(posStart.y - posEnd.y) < .1) {
        return TYPE_SIDE;
    }
    if (abs(posStart.x - posEnd.x) < .1) {
        return TYPE_FRONT;
    }
    return TYPE_UNKNOWN;
}

/* This is an advanced speaker, used by the BinauralEngine class. It can spawn at random locations */
class RandomSpawnSpeaker : public Speaker {
    public:
        static const uint16_t RNDSPKR_DISABLED = 1;
        static const uint16_t RNDSPKR_NO_DIRECT = 2;
        static const uint16_t RNDSPKR_NO_REFLECTION = 4;

        //when i take value over 900 the program crashes right at the beginning. stack full for some reason?
        static const uint16_t MAX_SPAWNS = 400;
        Pos pos2;
        double maxTime = 0.0;
        double minPause = 0.0;
        double maxPause = 0.0;
        double minSpeed = 1.0;
        double maxSpeed = 1.0;
        uint16_t nrSpawns = 0;
        uint16_t flags = 0;

        /* This is the list of spawn points and times */
        struct Spawns {
            Pos pos;
            double speed;
            uint32_t startSample;
        } spawns[MAX_SPAWNS];
};

/* The BinauralEngine class is the heart of this project. It uses the Speaker, Microhpone, Pos, Wall class, keeps
pointers to those objects, manages their memory, manages opening and closing of sound files, threads (todo!)
and finally calculates the sound waves.

Just create an object of this class and use addSpeaker, addMicrohpone, addWall functions to add your desired
objects. The addMicrohpone returns a pointer to a Microphone object that can be passed to another call of
addMicrohpone, if you wish to get a two channel audio file by the microhpone object.

If you want to move speakers, create Keyframes by allocating *keyFrames, set useKeyframes to true, store
the number, then save the absolute position and time stamp in each keyframe. BinauralEngine::start() will do the rest for you.
::reset() will free the memory

You can use the variables of .settings if you want to change the default settings.

The details are described below.
*/

class BinauralEngine {
    public:
        static const int16_t MAX_SPEAKERS = 400;
        static const int16_t MAX_MICROPHONES = 16;
        static const int16_t MAX_WALLS = 100;

        static const int8_t STATUS_IDLE = 0;
        static const int8_t STATUS_PLAYING = 1;
        static const int8_t STATUS_NOT_PLAYING = 2;

        /* The Speaker class gets some additional information for the target microphones
        to calculate direct samples faster (in function calcSamples) */
        class SpeakerBE : public Speaker {
            public:
                static const uint16_t SPKR_DISABLED = 1;
                static const uint16_t SPKR_NO_DIRECT = 2;
                static const uint16_t SPKR_NO_REFLECTION = 4;

                int16_t micNr[MAX_MICROPHONES];
                float micDist[MAX_MICROPHONES];
                float micPowDist[MAX_MICROPHONES];
                uint32_t micSampleDist[MAX_MICROPHONES];
                int16_t micCnt;
                uint16_t flags = 0;
                double damper[MAX_MICROPHONES];
                double damperStrength[MAX_MICROPHONES];

                /* These structs contain movement information and keyframes for speakers */
                struct Movement {
                    struct  KeyFrames {
                        Pos pos;
                        Pos deltaPerSample; // Increase the position of the speaker in each sample by this value
                        double time = 0.0; // Time stamp of the keyframe
                        uint32_t sampleNr = 0; // Sample of the keyframe - calculated by start()
                    } *keyFrames = nullptr;

                    bool useMovement = false;
                    uint16_t nrKeyFrames = 0;
                } movement;
        };

        /* These are the settings with default values */
        struct BESettings {
            uint32_t sampleRate = 44100;
            float additionalTime = 30.0; // This is used to add some audio time to allow sound delay
            uint16_t nrBounces = 50; //Number of sound wave bounces from wall to wall
            uint32_t maxReflectionPasses = 100; //Number of reflection passes before the engine quits
            uint16_t saveMicsEveryNrOfPasses = 0; //The engine saves the mic files every given nr of passes
            uint32_t randomSeed = 1;
            float airDamping = 100; /* The amount of high frequency reduction over long distances. Tried out
                                    different values,75-170 seems realistic, 100 was the most plausible val*/
        } settings;

        BinauralEngine();
        ~BinauralEngine();
        SpeakerBE * addSpeaker(string fileName, float x, float y, float z);
        SpeakerBE * addSpeaker(string fileName, float x, float y, float z, float volume);
        RandomSpawnSpeaker * addRndSpeaker(string fileName, double x, double y, double z,
                                                    double x2, double y2, double z2, float volume,
                                                    double totalTime, double minPause, double maxPause,
                                                    double minSpeed, double maxSpeed, uint16_t flags);
        void addSpeakerKeyFramesByString(SpeakerBE *speaker, string keyFrames);
        Wall * addWall(float x1, float y1, float z1, float x2, float y2, float z2, float amount);
        void setSpeakerPos(int16_t speaker_nr, float x, float y, float z);
        Microphone * addMicrophone(string fileName, float x, float y, float z);
        Microphone * addMicrophone(Microphone *secondChan, float x, float y, float z);
        void setMicrophonePos(uint8_t micNr, float x, float y, float z);
        int8_t start();
        void stop(bool writeMicFiles);
        void reset();
        int8_t getStatus();
        void calcMicLists();
        uint8_t update();
        void printSetup(uint8_t width, uint8_t height, uint8_t view);

        uint32_t currPos = 0;
        uint32_t totalSamples = 0;

    private:
        static const uint8_t TASK_DIRECT = 1;
        static const uint8_t TASK_REFLECTION = 2;

        /* Later, the program should support threads. for that purpose, ThreadData is passed to calcSamples.
        Right now, this is just used to tell calcSamples what it sould do */
        struct ThreadData {
            uint8_t threadNr;
            uint8_t task;
        };

        /*The reflector is a single 3D point that will be positioned on a wall and contains some information
        of the cumulated distance/reflection amount from speakers, other reflectors and to microphones
        to perform a very quick calculation later on */
        struct Reflector {
            Pos pos;
            double amount;
            double totalDistance = 0.0;
            uint32_t totalSamples = 0;
            double currTotalDistance = 0.0;
            uint32_t currTotalSamples = 0;
            double reflAmount;
            double totalReflAmount;
            struct TargetMics {
                double micDist;
                uint32_t micSampleDist;
                double currMicDist;
                uint32_t currMicSampleDist;
                double finalFactor;

                /* The high frequency dampers act like frequency crossovers and reduce high frequencies depending on the
                damperStrength. This is not perfect but the best working solution i figured out yet. */
                double damper;
                double damperStrength;
            }tMics[MAX_MICROPHONES];
        };

        void calcSamples(ThreadData *td);
        void calcDirectStaticSpeakerSamples(SpeakerBE *speaker);
        void calcDirectMovingSpeakerSamples(SpeakerBE *speaker);
        void calcDirectRndSpeakerSamples(RandomSpawnSpeaker *speaker);
        void calcReflStaticSpeakerSamples(SpeakerBE *speaker, Reflector *refl, uint16_t &nrReflections);
        void calcReflMovingSpeakerSamples(SpeakerBE *speaker, Reflector *refl, uint16_t &nrReflections);
        void calcReflRndSpeakerSamples(RandomSpawnSpeaker *speaker, Reflector *refl, uint16_t &nrReflections);


        int16_t findRandomWallByArea(int16_t exceptWallNr);

        SpeakerBE *speakers[MAX_SPEAKERS];
        Microphone *mics[MAX_MICROPHONES];
        Wall *walls[MAX_WALLS];
        RandomSpawnSpeaker *rndSpeakers[MAX_SPEAKERS];

        bool running = false;
        bool sampleOverflowWarning;
        int16_t actualSpeakers = 0;
        int16_t actualMicrophones = 0;
        int16_t actualWalls = 0;
        uint16_t actualRndSpeakers = 0;
        double totalReflectionArea = 0.0;
};

BinauralEngine::BinauralEngine() {
    for (uint16_t i = 0; i < BinauralEngine::MAX_SPEAKERS; i++) {
        speakers[i] = nullptr;
        rndSpeakers[i] = nullptr;
    }
    for (uint16_t i = 0; i < BinauralEngine::MAX_MICROPHONES; i++) {
        mics[i] = nullptr;
    }
    for (uint16_t i = 0; i < BinauralEngine::MAX_WALLS; i++) {
        walls[i] = nullptr;
    }
}

BinauralEngine::~BinauralEngine() {
    reset();
}

/* calcMicLists finds out, which microphones can "hear" which speakers directly, calculates their distance, and,
how much the sound is reduced by the distance. This information will later be used by calcDirectStaticSpeakerSamples
to increase the calculation speed */

void BinauralEngine::calcMicLists() {
    int16_t i, x;
    float d;

    for (i = 0; i < MAX_SPEAKERS; i++) {
        if (speakers[i] != nullptr) {
            speakers[i]->micCnt = 0;
            for (x = 0; x < MAX_MICROPHONES; x++) {
                if (mics[x] != nullptr) {
                    d = Pos::calcDistance(mics[x]->pos, speakers[i]->pos);
                    //todo: it is just a random value
                    if (d < 1000000) {
                        speakers[i]->micDist[x] = d;
                        speakers[i]->micSampleDist[x] = Pos::distanceToSamples(d, settings.sampleRate);
                        speakers[i]->micPowDist[x] = pow(d, 2.0);
                        speakers[i]->micNr[speakers[i]->micCnt] = x;
                        speakers[i]->micCnt++;
                        speakers[i]->damper[x] = 0.0;
                        speakers[i]->damperStrength[x] = 1.0 + (d / settings.airDamping);
                    }
                }

            }
        }
    }
}


int8_t BinauralEngine::getStatus() {

    if (running == true) {
        return STATUS_PLAYING;
    } else {
        return STATUS_IDLE;
    }
}

/* Clears memory and some settings. This is important if the object is destroyed or if you want to empty
the memory after the job is done */
void BinauralEngine::reset() {
    for (uint16_t i = 0; i < BinauralEngine::MAX_SPEAKERS; i++) {
        if (speakers[i] != nullptr) {
            delete speakers[i];
            speakers[i] = nullptr;
        }

        if (rndSpeakers[i] != nullptr) {
            delete speakers[i];
            speakers[i] = nullptr;
        }
    }
    for (uint16_t i = 0; i < BinauralEngine::MAX_MICROPHONES; i++) {
        if (mics[i] != nullptr) {
            delete mics[i];
            mics[i] = nullptr;
        }
    }

    for (uint16_t i = 0; i < BinauralEngine::MAX_WALLS; i++) {
        if (walls[i] != nullptr) {
            delete walls[i];
            walls[i] = nullptr;
        }
    }
    running = false;
}

Microphone * BinauralEngine::addMicrophone(string outputFile, float x, float y, float z) {
    for (int16_t i = 0; i < MAX_MICROPHONES; i++) {
        if (mics[i] == nullptr) {
            mics[i] = new Microphone;
            mics[i]->setOutputFile(outputFile);
            mics[i]->setPos(x, y, z);
            return mics[i];
        }
    }
    return nullptr;
}

Microphone * BinauralEngine::addMicrophone(Microphone *secondChan, float x, float y, float z) {
    for (int16_t i = 0; i < MAX_MICROPHONES; i++) {
        if (mics[i] == nullptr) {
            mics[i] = new Microphone;
            mics[i]->setPos(x, y, z);
            secondChan->addSecondChanMic(mics[i]);
            return mics[i];
        }
    }
    return nullptr;
}


Wall * BinauralEngine::addWall(float x1, float y1, float z1, float x2, float y2, float z2, float amount) {
    for (int16_t i = 0; i < MAX_WALLS; i++) {
        if (walls[i] == nullptr) {
            walls[i] = new Wall;
            walls[i]->posStart.set(x1, y1, z1);
            walls[i]->posEnd.set(x2, y2, z2);
            walls[i]->reflectionAmount = amount;
            walls[i]->calcArea();

            return walls[i];
        }
    }
    return nullptr;
}

BinauralEngine::SpeakerBE * BinauralEngine::addSpeaker(string fileName, float x, float y, float z) {
    for (int16_t i = 0; i < MAX_SPEAKERS; i++) {
        if (speakers[i] == nullptr) {
            speakers[i] = new SpeakerBE;
            setSpeakerPos(i, x, y, z);
            speakers[i]->setFileName(fileName);
            return speakers[i];
        }
    }
    return nullptr;
}


BinauralEngine::SpeakerBE * BinauralEngine::addSpeaker(string fileName, float x, float y, float z, float volume) {
    SpeakerBE *speaker = addSpeaker(fileName, x, y, z);
    if (speaker != nullptr) {
        speaker->volume = volume;

        return speaker;
    }
    return nullptr;
}

/* A random spawn speaker is added. The final positions of each spawn point is generated
in the start() function */
RandomSpawnSpeaker * BinauralEngine::addRndSpeaker(string fileName, double x, double y, double z,
                                                    double x2, double y2, double z2, float volume,
                                                    double totalTime, double minPause, double maxPause,
                                                    double minSpeed, double maxSpeed, uint16_t flags) {
    for (int16_t i = 0; i < MAX_SPEAKERS; i++) {
        if (rndSpeakers[i] == nullptr) {
            rndSpeakers[i] = new RandomSpawnSpeaker;
            rndSpeakers[i]->setFileName(fileName);
            rndSpeakers[i]->pos.x = x;
            rndSpeakers[i]->pos.y = y;
            rndSpeakers[i]->pos.z = z;
            rndSpeakers[i]->pos2.x = x2;
            rndSpeakers[i]->pos2.y = y2;
            rndSpeakers[i]->pos2.z = z2;
            rndSpeakers[i]->volume = volume;
            rndSpeakers[i]->minPause = minPause;
            rndSpeakers[i]->maxPause = maxPause;
            rndSpeakers[i]->maxTime = totalTime;
            rndSpeakers[i]->flags = flags;
            if (minSpeed == 0.0) {
                rndSpeakers[i]->minSpeed = 1.0;
            } else {
                rndSpeakers[i]->minSpeed = minSpeed;
            }
            if (maxSpeed == 0.0) {
                rndSpeakers[i]->maxSpeed = 1.0;
            } else {
                rndSpeakers[i]->maxSpeed = maxSpeed;
            }

            return rndSpeakers[i];
        }
    }
    return nullptr;
}


/* This method adds keyframes to a speaker by parsing a string that contains all information that is read
out of a text file. '/' is the delimiter between key frames that contain a time stamp and absolute x, y,
z coordinates, separated by ',' */
void BinauralEngine::addSpeakerKeyFramesByString(SpeakerBE *speaker, string keyFrames) {
    int32_t oldPosV1 = 0, posV1 = 0, oldPosV2, posV2, nrOfKeyFrames = 0, currKeyFrameNr = 1;
    string currentKeyFrame, currValue;
    Pos startLocation;

    if (keyFrames == "") {
        return;
    }

    /* first, count all keyframes to allocate the right amount of memory */
    do {
        posV1 = keyFrames.find("/", oldPosV1);
        oldPosV1 = posV1 + 1;
        nrOfKeyFrames++;
    } while (posV1 != -1);

    /* Set up the keyframes and allocate memory */
    speaker->movement.useMovement = true;
    speaker->movement.nrKeyFrames = nrOfKeyFrames;
    speaker->movement.keyFrames = (SpeakerBE::Movement::KeyFrames *)malloc((nrOfKeyFrames + 1) * sizeof(SpeakerBE::Movement::KeyFrames));
    speaker->movement.keyFrames[0].pos = speaker->pos;
    startLocation = speaker->pos;

    /* Parse string and set keyframe values */
    oldPosV1 = 0;
    do {
        posV1 = keyFrames.find("/", oldPosV1);
        if (posV1 == -1) {
            currentKeyFrame = keyFrames.substr(oldPosV1, keyFrames.length() - oldPosV1);
        } else {
            currentKeyFrame = keyFrames.substr(oldPosV1, posV1 - oldPosV1);
        }

        oldPosV2 = 0;
        for (uint8_t valNr = 0; valNr < 4; valNr++) {
            posV2 = currentKeyFrame.find(",", oldPosV2);
            if (posV2 == -1) {
                currValue = currentKeyFrame.substr(oldPosV2, currentKeyFrame.length() - oldPosV2);
            } else {
                currValue = currentKeyFrame.substr(oldPosV2, posV2 - oldPosV2);
            }

            switch (valNr) {
            case 0:
                speaker->movement.keyFrames[currKeyFrameNr].time =
                        atof(currValue.c_str());
                break;
            case 1:
                speaker->movement.keyFrames[currKeyFrameNr].pos.x =
                        startLocation.x + atof(currValue.c_str());
                break;
            case 2:
                speaker->movement.keyFrames[currKeyFrameNr].pos.y =
                        startLocation.y + atof(currValue.c_str());
                break;
            case 3:
                speaker->movement.keyFrames[currKeyFrameNr].pos.z =
                        startLocation.z + atof(currValue.c_str());
                break;
            }
            oldPosV2 = posV2 + 1;
            if (posV2 == -1) {
                break;
            }
        }

        oldPosV1 = posV1 + 1;
        currKeyFrameNr++;
    } while (posV1 != -1);

}
void BinauralEngine::setSpeakerPos(int16_t speaker_nr, float x, float y, float z) {
    if (speaker_nr >= 0 && speaker_nr < MAX_SPEAKERS && speakers[speaker_nr] != nullptr) {
        speakers[speaker_nr]->pos.x = x;
        speakers[speaker_nr]->pos.y = y;
        speakers[speaker_nr]->pos.z = z;
    }
}

void BinauralEngine::setMicrophonePos(uint8_t micNr, float x, float y, float z) {
    if (micNr >= 0 && micNr < MAX_MICROPHONES && mics[micNr] != nullptr) {
        mics[micNr]->pos.x = x;
        mics[micNr]->pos.y = y;
        mics[micNr]->pos.z = z;
    }
}

/* Call this function if you want to start the process */
int8_t BinauralEngine::start() {
    cout << "Engine starts..." << endl;
    uint32_t i;
    totalSamples = 0;

    actualSpeakers = 0;
    actualMicrophones = 0;
    actualRndSpeakers = 0;
    actualWalls = 0;

    sampleOverflowWarning = false;

    //Prepare the speaker objects and load their files. Print result (1 = OK)
    for (i = 0; i < MAX_SPEAKERS; i++) {
        if(speakers[i] != nullptr) {
            cout << "Starting speaker #" << i << " returns " << (int16_t)speakers[i]->start() << endl;
            if (speakers[i]->sampleCount > totalSamples) {
                totalSamples = speakers[i]->sampleCount;
            }
            actualSpeakers = i;

            uint32_t sampleNrDiff;

            if (speakers[i]->movement.useMovement == true) {
                /* Prepare keyframes of speakers */

                for (uint16_t keyFrameNr = 1; keyFrameNr <= speakers[i]->movement.nrKeyFrames; keyFrameNr++) {
                    speakers[i]->movement.keyFrames[keyFrameNr].sampleNr =
                        speakers[i]->movement.keyFrames[keyFrameNr].time * settings.sampleRate;

                    sampleNrDiff = speakers[i]->movement.keyFrames[keyFrameNr].sampleNr -
                        speakers[i]->movement.keyFrames[keyFrameNr - 1].sampleNr;

                    if (sampleNrDiff > 0) {

                        speakers[i]->movement.keyFrames[keyFrameNr - 1].deltaPerSample.x =
                            (speakers[i]->movement.keyFrames[keyFrameNr].pos.x -
                            speakers[i]->movement.keyFrames[keyFrameNr - 1].pos.x) / sampleNrDiff;

                        speakers[i]->movement.keyFrames[keyFrameNr - 1].deltaPerSample.y =
                            (speakers[i]->movement.keyFrames[keyFrameNr].pos.y -
                            speakers[i]->movement.keyFrames[keyFrameNr - 1].pos.y) / sampleNrDiff;

                        speakers[i]->movement.keyFrames[keyFrameNr - 1].deltaPerSample.z =
                            (speakers[i]->movement.keyFrames[keyFrameNr].pos.z -
                            speakers[i]->movement.keyFrames[keyFrameNr - 1].pos.z) / sampleNrDiff;
                    }
                }
            }
        }

        if (rndSpeakers[i] != nullptr) {
            /* Random Spawn Speakers are initialized */

            uint32_t nextStart = 0;

            /* This should keep the spawn points consistent in all speakers, independent from the number of
            spawn points and the number of other rndspeakers */
            srand(settings.randomSeed + i * 1337);
            cout << "Starting rndspawnspeaker #" << i << " returns " << (int16_t)rndSpeakers[i]->start() << endl;

            actualRndSpeakers = i;
            rndSpeakers[i]->nrSpawns = 0;
            cout << rndSpeakers[i]->sampleCount << ", " << rndSpeakers[i]->minPause << ", " << rndSpeakers[i]->maxPause << endl;

            /* The spawn locations and times for the Random Spawn Speaker are generated here.*/
            for (uint16_t spawnNr = 0; spawnNr < RandomSpawnSpeaker::MAX_SPAWNS; spawnNr++) {

                rndSpeakers[i]->spawns[spawnNr].pos.x = (rndSpeakers[i]->pos2.x - rndSpeakers[i]->pos.x)
                                                        * floatRand() + rndSpeakers[i]->pos.x;
                rndSpeakers[i]->spawns[spawnNr].pos.y = (rndSpeakers[i]->pos2.y - rndSpeakers[i]->pos.y)
                                                        * floatRand() + rndSpeakers[i]->pos.y;
                rndSpeakers[i]->spawns[spawnNr].pos.z = (rndSpeakers[i]->pos2.z - rndSpeakers[i]->pos.z)
                                                        * floatRand() + rndSpeakers[i]->pos.z;
                rndSpeakers[i]->spawns[spawnNr].speed = (rndSpeakers[i]->maxSpeed - rndSpeakers[i]->minSpeed)
                                                        * floatRand() + rndSpeakers[i]->minSpeed;

                rndSpeakers[i]->spawns[spawnNr].startSample = nextStart + (rndSpeakers[i]->maxPause - rndSpeakers[i]->minPause) *
                                                              floatRand() * settings.sampleRate + rndSpeakers[i]->minPause *
                                                              settings.sampleRate;
                if (rndSpeakers[i]->spawns[spawnNr].startSample + rndSpeakers[i]->sampleCount >= rndSpeakers[i]->maxTime * settings.sampleRate) {
                    break;
                } else {
                    nextStart = rndSpeakers[i]->spawns[spawnNr].startSample + rndSpeakers[i]->sampleCount;
                    rndSpeakers[i]->nrSpawns = spawnNr + 1;
                    if (nextStart > totalSamples) {
                        totalSamples = nextStart;
                    }
                }
            }

        }

    }

    //Additional time is added to allow sound reverb to be processed.
    if (totalSamples > 0) {
        totalSamples += settings.additionalTime * settings.sampleRate;
    }

    /* Just print the area of the walls for information and calculate totalReflection area, which is used
    to calculate the volume of the reflection sound */
    actualWalls =  -1;
    totalReflectionArea = 0.0;
    for (i = 0; i < MAX_WALLS; i++) {
        if(walls[i] != nullptr) {
            actualWalls = i;
            cout << "wall #" << i << ", area: " << walls[i]->area << " m2" << endl;
            totalReflectionArea += walls[i]->area;
        }
    }
    cout << "Total reflection area: " << totalReflectionArea << " m2"<< endl;

    /* Now start the microphones and let them allocate memory */
    actualMicrophones = 0;
    for (i = 0; i < MAX_MICROPHONES; i++) {
        if(mics[i] != nullptr) {
            cout << "Starting microphone #" << i << " returns " << (int16_t)mics[i]->start(totalSamples, totalReflectionArea) << endl;
            actualMicrophones = i;
        }
    }

    /* The total amount of speakers, microphones and walls is calculated to increase computing speed
    in calcSamples */
    actualSpeakers += 1;
    actualMicrophones += 1;
    actualWalls += 1;
    actualRndSpeakers += 1;

    calcMicLists();

    srand(settings.randomSeed);
    running = true;

    return totalSamples;
}

void BinauralEngine::stop(bool writeMicFiles) {
    int32_t i;
    cout << "Stopping speakers and microphones..." << endl;

    for (i = 0; i < MAX_SPEAKERS; i++) {
        if(speakers[i] != nullptr) {
            if (speakers[i]->movement.useMovement == true) {
                speakers[i]->movement.useMovement = false;
                speakers[i]->movement.nrKeyFrames = 0;
                free(speakers[i]->movement.keyFrames);
            }
            speakers[i]->stop();
        }
    }
    for (i = 0; i < MAX_MICROPHONES; i++) {
        if(mics[i] != nullptr) {
            mics[i]->stop(writeMicFiles);
        }
    }
    running = false;
}

/* update() was supposed to be a function that manages threads, call the appopriate functions and return a status
value. Since Threads are not implemented yet, this function does the entinre calculation and just calls
calcSamples and temporarily displays the progress on the screen. Later, it should be called periodically, but
for now, just call it once and wait until it is done */
uint8_t BinauralEngine::update() {
    ThreadData td;

    cout << "direct pass..." << endl;
    td.task = TASK_DIRECT;
    td.threadNr = 0;
    calcSamples(&td);

    for (uint32_t i = 0; i < settings.maxReflectionPasses; i++) {
       if (settings.saveMicsEveryNrOfPasses != 0 && i % settings.saveMicsEveryNrOfPasses == 0) {
            cout << "\rSaving microphone files   " << endl;
            for (int x = 0; x < actualMicrophones; x++) {
                if (mics[x] != nullptr) {
                    mics[x]->writeFile();
                }
            }
        }
        td.task = TASK_REFLECTION;
        td.threadNr = 0;
        cout << "\rReflection pass #" << i + 1 << "...";
        cout.flush();
        calcSamples(&td);

    }
    cout << endl;
    return STATUS_PLAYING;
}

/* findRandomWall will select a pseudo random wall, while large walls are returned more often than small
walls to maintain a realistic relation. This function is called by calcSamples to select a target wall
on which a reflector is placed. Pass exceptWallNr to exclude a wall (usually the wall a reflection is
emitted from) or -1 to return any wall. Be sure not to call this function with only one wall available,
which is excluded, this would result in an infinite loop. Returns -1 if operation is not possible */
int16_t BinauralEngine::findRandomWallByArea(int16_t exceptWallNr) {
    int16_t targetWall = -1;
    double r, addedAreas;
    if (actualWalls > 0) {
        if (actualWalls == 1) {
            targetWall = 0;
        } else {
            do {
                r = floatRand() * totalReflectionArea;
                addedAreas = 0.0;
                for (uint16_t i = 0; i < actualWalls; i++) {
                    addedAreas += walls[i]->area;
                    if (addedAreas > r) {
                        targetWall = i;
                        break;
                    }
                }
            } while (exceptWallNr == targetWall);

        }
        return targetWall;
    }
    return -1;
}

/* This function prints a rough text version of the 3D scenery, printing '.' as wall, 'S' as speaker, 'M'
as microphone. Currently, only top view is available */
void BinauralEngine::printSetup(uint8_t width, uint8_t height, uint8_t view) {
    double maxW = -1E+99, minW = 1E+99, maxH = -1E+99, minH = 1E+99;
    switch (view) {
        case 0: // top

            //First, find all maximal and minimal x/y/z positions of all objects to measure scene dimensions
            for (uint16_t wallNr = 0; wallNr < MAX_WALLS; wallNr++) {
                if (walls[wallNr] != nullptr) {
                    if (walls[wallNr]->posEnd.x + walls[wallNr]->modifier.amplitude / 2 > maxW) {
                        maxW = walls[wallNr]->posEnd.x + walls[wallNr]->modifier.amplitude / 2 ;
                    }
                    if (walls[wallNr]->posStart.x + walls[wallNr]->modifier.amplitude / 2 > maxW) {
                        maxW = walls[wallNr]->posStart.x + walls[wallNr]->modifier.amplitude / 2 ;
                    }
                    if (walls[wallNr]->posEnd.y + walls[wallNr]->modifier.amplitude / 2 > maxH) {
                        maxH = walls[wallNr]->posEnd.y + walls[wallNr]->modifier.amplitude / 2 ;
                    }
                    if (walls[wallNr]->posStart.y + walls[wallNr]->modifier.amplitude / 2 > maxH) {
                        maxH = walls[wallNr]->posEnd.y + walls[wallNr]->modifier.amplitude / 2;
                    }

                    if (walls[wallNr]->posEnd.x - walls[wallNr]->modifier.amplitude / 2 < minW) {
                        minW = walls[wallNr]->posEnd.x - walls[wallNr]->modifier.amplitude / 2;
                    }
                    if (walls[wallNr]->posStart.x - walls[wallNr]->modifier.amplitude / 2 < minW) {
                        minW = walls[wallNr]->posStart.x - walls[wallNr]->modifier.amplitude / 2;
                    }
                    if (walls[wallNr]->posEnd.y - walls[wallNr]->modifier.amplitude / 2 < minH) {
                        minH = walls[wallNr]->posEnd.y - walls[wallNr]->modifier.amplitude / 2;
                    }
                    if (walls[wallNr]->posStart.y - walls[wallNr]->modifier.amplitude / 2 < minH) {
                        minH = walls[wallNr]->posEnd.y - walls[wallNr]->modifier.amplitude / 2;
                    }
                }
            }

            for (uint16_t speakerNr = 0; speakerNr < MAX_SPEAKERS; speakerNr++) {
                if (speakers[speakerNr] != nullptr) {
                    if (speakers[speakerNr]->pos.x > maxW) {
                        maxW = speakers[speakerNr]->pos.x;
                    }
                    if (speakers[speakerNr]->pos.y > maxH) {
                        maxH = speakers[speakerNr]->pos.y;
                    }
                    if (speakers[speakerNr]->pos.x < minW) {
                        minW = speakers[speakerNr]->pos.x;
                    }
                    if (speakers[speakerNr]->pos.y < minH) {
                        minH = speakers[speakerNr]->pos.y;
                    }

                    if (speakers[speakerNr]->movement.useMovement == true) {
                        for (uint16_t keyFrameNr = 0; keyFrameNr < speakers[speakerNr]->movement.nrKeyFrames + 1; keyFrameNr++) {
                            if (speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.x > maxW) {
                                maxW = speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.x ;
                            }
                            if (speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.y > maxH) {
                                maxH = speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.y;
                            }
                            if (speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.x < minW) {
                                minW = speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.x;
                            }
                            if (speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.y < minH) {
                                minH = speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.y;
                            }
                        }
                    }
                }
            }


            for (uint16_t micNr = 0; micNr < MAX_MICROPHONES; micNr++) {
                if (mics[micNr] != nullptr) {
                    if (mics[micNr]->pos.x > maxW) {
                        maxW = mics[micNr]->pos.x;
                    }
                    if (mics[micNr]->pos.y > maxH) {
                        maxH = mics[micNr]->pos.y;
                    }
                    if (mics[micNr]->pos.x < minW) {
                        minW = mics[micNr]->pos.x;
                    }
                    if (mics[micNr]->pos.y < minH) {
                        minH = mics[micNr]->pos.y;
                    }
                }
            }


            /* Now fit that into the desired console character width and height */

#ifdef _WIN32   // console character height:widht ratio, depending on OS
            const double heightratio = 1.5;
#else
            const double heightratio = 2.0;
#endif
            double scale;
            uint16_t realH, realW;

            if (((maxH - minH) / heightratio) / height > (maxW -  minW) / width) {
                realH = height;
                scale = height / ((maxH - minH) / heightratio);
                realW = (maxW - minW) * scale;
            } else {
                realW = width;
                scale = width / (maxW - minW);
                realH = (maxH - minH) * scale / heightratio;
            }

            /* Create a character array with the calculated dimensions and fill it with spaces */
            char c[realH + 1][realW + 2];
            for (uint8_t w = 0; w < realW + 1; w++) {
                for (uint8_t h = 0; h < realH + 1; h++) {
                    c[h][w] = ' ';
                }
            }

            for (uint8_t h = 0; h < realH + 1; h++) {
                c[h][realW + 1] = '\0';
            }

            /* Now fill characters at the calculated spots in the char array, '.' for a reflecting wall,
            'S' for speaker, 'M' for microphone */
            for (uint16_t wallNr = 0; wallNr < MAX_WALLS; wallNr++) {
                if (walls[wallNr] != nullptr && abs(walls[wallNr]->posStart.z - walls[wallNr]->posEnd.z) > .05) {

                    uint8_t x, startP, endP;
                    if (walls[wallNr]->posEnd.x > walls[wallNr]->posStart.x) {
                        startP = (walls[wallNr]->posStart.x - minW)  * scale;
                        endP = (walls[wallNr]->posEnd.x - minW)  * scale;
                    } else {
                        endP = (walls[wallNr]->posStart.x - minW)  * scale;
                        startP = (walls[wallNr]->posEnd.x - minW)  * scale;
                    }
                    for (x = startP; x < endP; x++) {
                        c[realH - (uint8_t)((walls[wallNr]->posStart.y + walls[wallNr]->modifier.getModifiedAmplitude((x + minW) / scale) - minH) * scale / heightratio)][x] = '.';
                        c[realH - (uint8_t)((walls[wallNr]->posEnd.y + walls[wallNr]->modifier.getModifiedAmplitude((x + minW) / scale) - minH) * scale / heightratio)][x] = '.';
                        //kkk
                    }

                    if (walls[wallNr]->posEnd.y < walls[wallNr]->posStart.y) {
                        startP = realH - (uint8_t)(walls[wallNr]->posStart.y - minH)  * scale / heightratio;
                        endP = realH - (uint8_t) (walls[wallNr]->posEnd.y - minH)  * scale / heightratio;
                    } else {
                        endP = realH - (uint8_t) (walls[wallNr]->posStart.y - minH)  * scale / heightratio;
                        startP = realH - (uint8_t) (walls[wallNr]->posEnd.y - minH)  * scale / heightratio;
                    }
                    for (x = startP; x < endP; x++) {
                        c[x][(uint8_t)((walls[wallNr]->posStart.x + walls[wallNr]->modifier.getModifiedAmplitude((x + minH) / scale) - minW) * scale)] = '.';
                        c[x][(uint8_t)((walls[wallNr]->posEnd.x + walls[wallNr]->modifier.getModifiedAmplitude((x + minH) / scale) - minW) * scale)] = '.';
                    }
                }
            }


            for (uint16_t micNr = 0; micNr < MAX_MICROPHONES; micNr++) {
                if (mics[micNr] != nullptr) {
                    c[realH - (uint8_t)((mics[micNr]->pos.y - minH) * scale / heightratio)]
                    [(uint8_t)((mics[micNr]->pos.x - minW) * scale)] = 'M';
                }
            }


            for (uint16_t speakerNr = 0; speakerNr < MAX_SPEAKERS; speakerNr++) {
                if (speakers[speakerNr] != nullptr) {

                    if (speakers[speakerNr]->movement.useMovement == true) {
                        for (uint16_t keyFrameNr = 0; keyFrameNr < speakers[speakerNr]->movement.nrKeyFrames + 1; keyFrameNr++) {
                            c[realH - (uint8_t)((speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.y - minH) * scale / heightratio)]
                            [(uint8_t)((speakers[speakerNr]->movement.keyFrames[keyFrameNr].pos.x - minW) * scale)] = '*';
                        }
                    }

                    c[realH - (uint8_t)((speakers[speakerNr]->pos.y - minH) * scale / heightratio)]
                    [(uint8_t)((speakers[speakerNr]->pos.x - minW) * scale)] = 'S';

                }
            }

            /* print the char array */
            for (uint8_t h = 0; h < realH + 1; h++) {
                cout << c[h] << endl;
            }
            break;

    }
}

/* The calcSamples function  calls further functions that read all samples from all speakers, calculate
the changing of the amplitudes and high frequencies by wall damping and distance and write the results into
the microphone buffers. It was designed to be executed in multiple threads, but this is not implemented yet.
It can perform two tasks: Calculate direct samples, from speakers to microphones, and reflection samples,
in which the sound samples come from speakers bounce off walls and then arrive microphones. The samples are
modified in three main ways: The first is the lowering of volume  and the second is the delay
that is caused by sound waves traveling through air, the third is the reduction of high frequencies when distance
and damping increase. All are very important for binaural hearing and allow to percieve the location of
the sound source directly and the dimensions of the room by locating
indirect sound waves from walls. */
void BinauralEngine::calcSamples(ThreadData *td) {
    int32_t speakerNr, micNr;
    uint32_t reflNr;

    switch (td->task) {

        /* Calculation of direct sound samples. Values are used that were previously calculated by
        calcMicLists*/
        case TASK_DIRECT:
            for (speakerNr = 0; speakerNr < actualSpeakers || speakerNr < actualRndSpeakers; speakerNr++) {

                if (speakers[speakerNr] != nullptr) {

                    if (speakers[speakerNr]->movement.useMovement == false) { // Static speaker

                        /* Only calculate if the speaker is not disabled for direct samples or in general
                        (set in by flags). This step repeats for each type of sample calculation with
                        slight variations, depending on the kind of speaker and calculation */
                        if (!(speakers[speakerNr]->flags & (SpeakerBE::SPKR_DISABLED | SpeakerBE::SPKR_NO_DIRECT))) {
                            calcDirectStaticSpeakerSamples(speakers[speakerNr]);
                        }

                    } else { // Moving speaker, keyframes are used.

                        if (!(speakers[speakerNr]->flags & (SpeakerBE::SPKR_DISABLED | SpeakerBE::SPKR_NO_DIRECT))) {
                            calcDirectMovingSpeakerSamples(speakers[speakerNr]);
                        }
                    }
                }

                if (rndSpeakers[speakerNr] != nullptr) { // Random spawn speaker

                    if (!(rndSpeakers[speakerNr]->flags & (RandomSpawnSpeaker::RNDSPKR_DISABLED | RandomSpawnSpeaker::RNDSPKR_NO_DIRECT))) {
                        calcDirectRndSpeakerSamples(rndSpeakers[speakerNr]);
                    }
                }
            }
            break;

        /* In this case, only reflection is calculated. The speakers bounce their samples to random
        points on the walls, which then bounce the sound to the microphone and to the next random
        reflector point */
        case TASK_REFLECTION:
            if (actualWalls > 0) {
                uint16_t nrReflections = settings.nrBounces;
                int16_t wallNr, lastRefl = -1, lastWall = -1;

                if (actualWalls == 1) {
                    nrReflections = 1;
                }

                Reflector refl[nrReflections];
                for (micNr = 0; micNr < actualMicrophones; micNr++) {
                    mics[micNr]->totalReflections++; //this is needed to adjust reflection volume later
                }

                /* First, all reflection points are selected. At the current state, the function
                findRandomWallByArea just returns another random wall, depending on the are of the wall,
                then, for each wall, a random spot for a reflector is chosen within that area
                (or volume, which should be the same, when wall objects have flat dimensions.
                This concept can be improved obviously) */
                for (reflNr = 0; reflNr < nrReflections; reflNr++) {
                    wallNr = findRandomWallByArea(lastWall);
                    refl[reflNr].pos.x = floatRand() * (walls[wallNr]->posEnd.x - walls[wallNr]->posStart.x) + walls[wallNr]->posStart.x;
                    refl[reflNr].pos.y = floatRand() * (walls[wallNr]->posEnd.y - walls[wallNr]->posStart.y) + walls[wallNr]->posStart.y;
                    refl[reflNr].pos.z = floatRand() * (walls[wallNr]->posEnd.z - walls[wallNr]->posStart.z) + walls[wallNr]->posStart.z;

                    if (walls[wallNr]->modifier.usedModifier != WallModifier::MOD_NO_MOD) {
                        switch(walls[wallNr]->getWallType()) {
                            case Wall::TYPE_FRONT:
                                refl[reflNr].pos.y += walls[wallNr]->modifier.getModifiedAmplitude(refl[reflNr].pos.x);
                                break;
                            case Wall::TYPE_SIDE:
                                refl[reflNr].pos.x += walls[wallNr]->modifier.getModifiedAmplitude(refl[reflNr].pos.y);
                                break;
                            case Wall::TYPE_TOP:
                                refl[reflNr].pos.z += walls[wallNr]->modifier.getModifiedAmplitude(refl[reflNr].pos.x);
                                break;
                        }
                    }

                    if (lastRefl == -1) { // The first reflector, directly hit by a speaker
                        refl[reflNr].totalDistance = 0.0;
                        refl[reflNr].totalSamples = 0;
                        refl[reflNr].totalReflAmount = walls[wallNr]->reflectionAmount;
                        refl[reflNr].reflAmount = walls[wallNr]->reflectionAmount;
                    } else { // Reflectors that are hit by a previous reflector
                        refl[reflNr].totalDistance = refl[lastRefl].totalDistance + Pos::calcDistance(refl[reflNr].pos, refl[lastRefl].pos);
                        refl[reflNr].totalSamples = Pos::distanceToSamples(refl[reflNr].totalDistance, settings.sampleRate);
                        refl[reflNr].reflAmount = walls[wallNr]->reflectionAmount;

                        /* Cumulate the total reflection amount that is left - not sure, if this is
                        calculated correctly */
                        refl[reflNr].totalReflAmount = refl[0].reflAmount;
                        for (uint16_t x = 1; x < reflNr - 1; x++) {
                            refl[reflNr].totalReflAmount *= refl[x].reflAmount;
                        }
                    }

                    /* Assign the results to target mics (tMics), which will be used later when the current
                    speaker and mic distance is known */
                    for (micNr = 0; micNr < actualMicrophones; micNr++) {
                        refl[reflNr].tMics[micNr].micDist = refl[reflNr].totalDistance + Pos::calcDistance(refl[reflNr].pos, mics[micNr]->pos);
                        refl[reflNr].tMics[micNr].micSampleDist = Pos::distanceToSamples(refl[reflNr].tMics[micNr].micDist, settings.sampleRate);
                    }
                    lastRefl = reflNr;
                    lastWall = wallNr;

                }

                /* With these prepared data, the target functions are called that will process the sound
                samples. */
                for (speakerNr = 0; speakerNr < actualSpeakers || speakerNr < actualRndSpeakers; speakerNr++) {

                    if (speakers[speakerNr] != nullptr) {

                        if (speakers[speakerNr]->movement.useMovement == false) { //static speaker

                            if (!(speakers[speakerNr]->flags & (SpeakerBE::SPKR_DISABLED | SpeakerBE::SPKR_NO_REFLECTION))) {
                                calcReflStaticSpeakerSamples(speakers[speakerNr], refl, nrReflections);
                            }

                        } else { // Moving Speaker, keyframes are used

                            if (!(speakers[speakerNr]->flags & (SpeakerBE::SPKR_DISABLED | SpeakerBE::SPKR_NO_REFLECTION))) {
                                calcReflMovingSpeakerSamples(speakers[speakerNr], refl, nrReflections);
                            }
                        }
                    }

                    if (rndSpeakers[speakerNr] != nullptr) { // Random spawn speakers

                        if (!(rndSpeakers[speakerNr]->flags & (RandomSpawnSpeaker::RNDSPKR_DISABLED | RandomSpawnSpeaker::RNDSPKR_NO_REFLECTION))) {
                            calcReflRndSpeakerSamples(rndSpeakers[speakerNr], refl, nrReflections);
                        }
                    }
                }
            }
            break;

    }
}

/* Sound samples of direct static speakers are processed here. */
void BinauralEngine::calcDirectStaticSpeakerSamples(SpeakerBE *speaker) {
    double amp;

    //todo: if speaker is too far away, it could exceed limits of the buffer
    for (uint32_t sampleNr = 0; sampleNr < speaker->sampleCount; sampleNr++) {
        amp = speaker->samples[sampleNr];
        for (int32_t micNr = 0; micNr < speaker->micCnt; micNr++) {

            /* Now each sound sample is written in the direct mic buffer, but
            'in the future', depending on the distance of the sound source to the
            microphone. additionally the volume is reduced*/
            speaker->damper[micNr] += (amp - speaker->damper[micNr]) /
                speaker->damperStrength[micNr];

            //mics[speakers[speakerNr]->micNr[micNr]]->
            //    drctBuffer[(sampleNr + speakers[speakerNr]->micSampleDist[micNr])]
            //    += amp / speakers[speakerNr]->micDist[micNr];
            mics[speaker->micNr[micNr]]->
                drctBuffer[(sampleNr + speaker->micSampleDist[micNr])]
                += speaker->damper[micNr] / speaker->micDist[micNr];
        }
    }
}

/* Sound samples of moving speakers with keyframes are processed here. */
void BinauralEngine::calcDirectMovingSpeakerSamples(SpeakerBE *speaker) {
    uint16_t currKeyFrame = 0;
    uint32_t iMicSampleDist;
    int32_t micNr;
    double dMicSampleDist, rest;
    double micDist, amp;
    Pos speakerPos = speaker->pos;

    /* The high frequency dampers act like frequency crossovers and reduce high frequencies depending on the
    damperStrength. This is not perfect but the best working solution i figured out yet. */
    double damper[speaker->micCnt], damperStrength[speaker->micCnt];


    /* Reset all high frequency dampers */
    for (micNr = 0; micNr < speaker->micCnt; micNr++) {
        damper[micNr] = 0.0;
    }

    for (uint32_t sampleNr = 0; sampleNr < speaker->sampleCount; sampleNr++) {
        amp = speaker->samples[sampleNr];
        if (sampleNr >= speaker->movement.keyFrames[currKeyFrame + 1].sampleNr) {

            /* Check if we have to enter a new keyframe */
            if (currKeyFrame + 1<= speaker->movement.nrKeyFrames) {
                currKeyFrame++;
                speakerPos = speaker->movement.keyFrames[currKeyFrame].pos;
            }
        }

        /* Move the speaker by the delta per keyframe */
        speakerPos.x += speaker->movement.keyFrames[currKeyFrame].deltaPerSample.x;
        speakerPos.y += speaker->movement.keyFrames[currKeyFrame].deltaPerSample.y;
        speakerPos.z += speaker->movement.keyFrames[currKeyFrame].deltaPerSample.z;

        for (micNr = 0; micNr < speaker->micCnt; micNr++) {
            /* This is the same as Pos::calcDistance, but with a minimum of function calls
            for significant higher performance */
            micDist = sqrt((speakerPos.x - mics[speaker->micNr[micNr]]->pos.x) *
                           (speakerPos.x - mics[speaker->micNr[micNr]]->pos.x) +
                           (speakerPos.y - mics[speaker->micNr[micNr]]->pos.y) *
                           (speakerPos.y - mics[speaker->micNr[micNr]]->pos.y) +
                           (speakerPos.z - mics[speaker->micNr[micNr]]->pos.z) *
                           (speakerPos.z - mics[speaker->micNr[micNr]]->pos.z)
                            );

            dMicSampleDist = micDist / MPS * settings.sampleRate;
            damperStrength[micNr] =  1.0 + micDist / settings.airDamping;

            /* When moving, two samples have to be written to maintain a smooth tranistion if
            the changing distance of the sound source causes a switch of the target sample.
            The difference between int and double is used to increase the impact on the second
            and decrease it on the first sample */
            iMicSampleDist = dMicSampleDist;
            rest = dMicSampleDist - iMicSampleDist;

            damper[micNr] += (amp - damper[micNr]) / damperStrength[micNr];

            mics[speaker->micNr[micNr]]->drctBuffer[(sampleNr + iMicSampleDist)] +=
                    (damper[micNr] / micDist) * (1.0 -rest);
            mics[speaker->micNr[micNr]]->drctBuffer[(sampleNr + iMicSampleDist + 1)] +=
                    (damper[micNr] / micDist) * (rest);
        }
    }
}

/* Sound samples of random spawn speakers are processed here. */
void BinauralEngine::calcDirectRndSpeakerSamples(RandomSpawnSpeaker *speaker) {
    int32_t micNr, spawnNr;
    uint32_t sampleNr;
    double amp;
    double micDist[actualMicrophones];
    uint32_t micSampleDist[actualMicrophones];
    uint32_t speedSampleCount;

    /* The high frequency dampers act like frequency crossovers and reduce high frequencies depending on the
    damperStrength. This is not perfect but the best working solution i figured out yet. */
    /* The high frequency dampers act like frequency crossovers and reduce high frequencies depending on the
    damperStrength. This is not perfect but the best working solution i figured out yet. */
    double damper[actualMicrophones];
    double damperStrength[actualMicrophones];

    for (spawnNr = 0; spawnNr < speaker->nrSpawns; spawnNr++) {
        for (micNr = 0; micNr < actualMicrophones; micNr++) {
            micDist[micNr] = Pos::calcDistance(speaker->spawns[spawnNr].pos, mics[micNr]->pos);
            micSampleDist[micNr] = Pos::distanceToSamples(micDist[micNr], settings.sampleRate);
            damper[micNr] = 0.0;
            damperStrength[micNr] = 1.0 + micDist[micNr] / settings.airDamping;
        }

        speedSampleCount = speaker->sampleCount / speaker->spawns[spawnNr].speed;
        for (sampleNr = 0; sampleNr < speedSampleCount; sampleNr++) {
            amp = speaker->samples[(uint32_t)(sampleNr * speaker->spawns[spawnNr].speed)];
            for (micNr = 0; micNr < actualMicrophones; micNr++) {

                damper[micNr] += (amp - damper[micNr]) / damperStrength[micNr];
                mics[micNr]->
                    drctBuffer[(sampleNr + speaker->spawns[spawnNr].startSample) + micSampleDist[micNr]]
                    += damper[micNr] / micDist[micNr];
            }
        }
    }
}

/* Reflecting sound samples of static speakers are processed here. */
void BinauralEngine::calcReflStaticSpeakerSamples(SpeakerBE *speaker, Reflector *refl, uint16_t &nrReflections) {
    uint16_t reflNr;
    int32_t micNr;
    uint32_t sampleNr;
    double speakerDist;

    /* Calculate the distance of the speaker to the first reflector */
    speakerDist = Pos::calcDistance(speaker->pos, refl[0].pos);

    /* And add them to the precalculated total distances in all reflector points */
    for (reflNr = 0; reflNr < nrReflections; reflNr++) {
        refl[reflNr].currTotalDistance = speakerDist + refl[reflNr].totalDistance;
        refl[reflNr].currTotalSamples = Pos::distanceToSamples(refl[reflNr].currTotalDistance, settings.sampleRate);

        /* Add those final values to all target microphones and build a final factor
        for fast calculations */
        for (micNr = 0; micNr < actualMicrophones; micNr++) {
            refl[reflNr].tMics[micNr].currMicDist = speakerDist + refl[reflNr].tMics[micNr].micDist;
            refl[reflNr].tMics[micNr].currMicSampleDist = Pos::distanceToSamples(refl[reflNr].tMics[micNr].currMicDist, settings.sampleRate);
            refl[reflNr].tMics[micNr].finalFactor = 1 / refl[reflNr].tMics[micNr].currMicDist;
            refl[reflNr].tMics[micNr].damper = 0.0;
            refl[reflNr].tMics[micNr].damperStrength = ((1.0 + refl[reflNr].tMics[micNr].currMicDist / settings.airDamping) /
                    refl[reflNr].totalReflAmount);
            /* check if the sample distance (number of the samples 'in the future' caused by
            the travel time of the sound waves exceeds the total buffer length - and reduce the
            number of bounces, if so. Display a warning once */
            if (refl[reflNr].tMics[micNr].currMicSampleDist >= settings.additionalTime * settings.sampleRate) {
                nrReflections = reflNr - 1;
                if (sampleOverflowWarning == false) {
                sampleOverflowWarning = true;
                    cout << "\rWARNING: Reflection time exceed sound length. Reflection bounces have been reduced. To avoid this, increase additional length to the audio buffer or reduce reflection bounces." << endl;
                }
            }
        }
    }


    /* Finally calculate the samples using the previously generated sample distances and
    amplitude reductions */
    for (sampleNr = 0; sampleNr < speaker->sampleCount; sampleNr++) {
        for (reflNr = 0; reflNr < nrReflections; reflNr++) {
            for (micNr = 0; micNr < actualMicrophones; micNr++) {

                refl[reflNr].tMics[micNr].damper += (speaker->samples[sampleNr]
                    - refl[reflNr].tMics[micNr].damper) / refl[reflNr].tMics[micNr].damperStrength;

                mics[micNr]->reflBuffer[sampleNr + refl[reflNr].tMics[micNr].currMicSampleDist]
                    += refl[reflNr].tMics[micNr].damper * refl[reflNr].tMics[micNr].finalFactor;
            }
        }
    }
}

/* Reflecting sound samples of moving speakers are processed here. */
void BinauralEngine::calcReflMovingSpeakerSamples(SpeakerBE *speaker, Reflector *refl, uint16_t &nrReflections) {
    int32_t micNr;
    uint32_t sampleNr;
    uint16_t reflNr;

    /* Reflector distances are calculated, but without the distance of the speaker
    to the first reflector since this will change during the samples */
    for (reflNr = 0; reflNr < nrReflections; reflNr++) {
        refl[reflNr].currTotalDistance = refl[reflNr].totalDistance;
        refl[reflNr].currTotalSamples = Pos::distanceToSamples(refl[reflNr].currTotalDistance, settings.sampleRate);

        for (micNr = 0; micNr < actualMicrophones; micNr++) {
            refl[reflNr].tMics[micNr].currMicDist = refl[reflNr].tMics[micNr].micDist;
            refl[reflNr].tMics[micNr].currMicSampleDist = Pos::distanceToSamples(refl[reflNr].tMics[micNr].currMicDist, settings.sampleRate);
            refl[reflNr].tMics[micNr].damper = 0.0;
        }
    }

    /* Todo: Overflow warning and reduction of bounces to prevent memory access problems when the
    additional time is too short */

    uint16_t currKeyFrame = 0;
    uint32_t iReflSampleDist;
    double dReflSampleDist, rest;
    double reflDist;
    Pos speakerPos = speaker->pos;

    for (sampleNr = 0; sampleNr < speaker->sampleCount; sampleNr++) {

        /* New keyframe? If yes, apply keyframe values */
        if (sampleNr >= speaker->movement.keyFrames[currKeyFrame + 1].sampleNr) {
            if (currKeyFrame + 1<= speaker->movement.nrKeyFrames) {
                currKeyFrame++;
                speakerPos = speaker->movement.keyFrames[currKeyFrame].pos;
            }
        }

        /* Move the speaker by the delta per keyframe */
        speakerPos.x += speaker->movement.keyFrames[currKeyFrame].deltaPerSample.x;
        speakerPos.y += speaker->movement.keyFrames[currKeyFrame].deltaPerSample.y;
        speakerPos.z += speaker->movement.keyFrames[currKeyFrame].deltaPerSample.z;

        /* Calculate all samples for each reflector */
        for (reflNr = 0; reflNr < nrReflections; reflNr++) {

            /* This is the same as Pos::calcDistance, but with a minimum of function calls
            for significant higher performance */
            reflDist = sqrt((speakerPos.x - refl[0].pos.x) *
                           (speakerPos.x - refl[0].pos.x) +
                           (speakerPos.y - refl[0].pos.y) *
                           (speakerPos.y - refl[0].pos.y) +
                           (speakerPos.z - refl[0].pos.z) *
                           (speakerPos.z - refl[0].pos.z)
                            );
            /* floating point value of time delay in samples, depending on the physical
            distance of the speaker to first reflector position. This is added to the
            total distance of the sound traveling from reflectors to microhpones. */
            dReflSampleDist = reflDist / MPS * settings.sampleRate;

            /* When moving, two samples have to be written to maintain a smooth tranistion if
            the changing distance of the sound source causes a switch of the target sample.
            The difference between int and double is used to increase the impact on the second
            and decrease it on the first sample */
            iReflSampleDist = dReflSampleDist;
            rest = dReflSampleDist - iReflSampleDist;

            for (micNr = 0; micNr < actualMicrophones; micNr++) {
                refl[reflNr].tMics[micNr].damperStrength = 1.0 + (refl[reflNr].tMics[micNr].currMicDist + reflDist) / settings.airDamping;
                refl[reflNr].tMics[micNr].damper +=  (speaker->samples[sampleNr] - refl[reflNr].tMics[micNr].damper) /
                        refl[reflNr].tMics[micNr].damperStrength;


                mics[micNr]->reflBuffer[sampleNr + refl[reflNr].tMics[micNr].currMicSampleDist + iReflSampleDist] +=
                       refl[reflNr].tMics[micNr].damper / (refl[reflNr].tMics[micNr].currMicDist + reflDist) * (1.0 - rest);
                mics[micNr]->reflBuffer[sampleNr + refl[reflNr].tMics[micNr].currMicSampleDist + iReflSampleDist + 1] +=
                       refl[reflNr].tMics[micNr].damper / (refl[reflNr].tMics[micNr].currMicDist + reflDist) * rest;

            }
        }
    }
}


void BinauralEngine::calcReflRndSpeakerSamples(RandomSpawnSpeaker *speaker, Reflector *refl, uint16_t &nrReflections) {
    int32_t reflNr, sampleNr, micNr, spawnNr, speedSampleCount;

    /* Reflector distances are calculated, but without the distance of the speaker
    to the first reflector since this will change during the samples */
    for (reflNr = 0; reflNr < nrReflections; reflNr++) {
        refl[reflNr].currTotalDistance = refl[reflNr].totalDistance;
        refl[reflNr].currTotalSamples = Pos::distanceToSamples(refl[reflNr].currTotalDistance, settings.sampleRate);

        for (micNr = 0; micNr < actualMicrophones; micNr++) {
            refl[reflNr].tMics[micNr].currMicDist = refl[reflNr].tMics[micNr].micDist;
            refl[reflNr].tMics[micNr].currMicSampleDist = Pos::distanceToSamples(refl[reflNr].tMics[micNr].currMicDist, settings.sampleRate);
            refl[reflNr].tMics[micNr].damper = 0.0;
        }
    }

    /* Todo: Overflow warning and reduction of bounces to prevent overflow when the
    additional time is too short */

    double reflDist;
    Pos speakerPos;

    for (spawnNr = 0; spawnNr < speaker->nrSpawns; spawnNr++) {

        /* Distance from speaker to the first reflector. This is needed later */
        speakerPos = speaker->spawns[spawnNr].pos;
        reflDist = Pos::calcDistance(speakerPos, refl[0].pos);

        /* Calculate all samples for each reflector */
        for (reflNr = 0; reflNr < nrReflections; reflNr++) {
            for (micNr = 0; micNr < actualMicrophones; micNr++) {
                refl[reflNr].tMics[micNr].currMicDist = refl[reflNr].tMics[micNr].micDist + reflDist;
                refl[reflNr].tMics[micNr].currMicSampleDist = Pos::distanceToSamples(refl[reflNr].tMics[micNr].currMicDist, settings.sampleRate);

                refl[reflNr].tMics[micNr].damperStrength = ((1.0 + refl[reflNr].tMics[micNr].currMicDist /
                                                           settings.airDamping) / refl[reflNr].totalReflAmount);
            }

        }
        speedSampleCount = speaker->sampleCount / speaker->spawns[spawnNr].speed;
        for (sampleNr = 0; sampleNr < speedSampleCount; sampleNr++) {

            for (reflNr = 0; reflNr < nrReflections; reflNr++) {


                for (micNr = 0; micNr < actualMicrophones; micNr++) {
                    refl[reflNr].tMics[micNr].damper += (speaker->samples[(uint32_t)(sampleNr * speaker->spawns[spawnNr].speed)] -
                                                         refl[reflNr].tMics[micNr].damper) / refl[reflNr].tMics[micNr].damperStrength;

                    mics[micNr]->reflBuffer[sampleNr + refl[reflNr].tMics[micNr].currMicSampleDist + speaker->spawns[spawnNr].startSample] +=
                    refl[reflNr].tMics[micNr].damper / refl[reflNr].tMics[micNr].currMicDist;
                }
            }
        }
    }
}


/* This function reads a text file which contains information about speakers, microphones, settings, walls
and adds some functionality like the head setup. It directly sets up the BinauralEngine object privided as
reference. This could be put into the BinauralEngineClass as method later */
void configEngineByFile(BinauralEngine &binaural, string fileName) {
    cout << "Loading file: " << fileName << endl;


    ifstream in(fileName.c_str());

    if(!in) {
        cout << "Cannot open input file '" << fileName << "'." << endl;
        return;
    }

    string str, arg, value;
    int32_t pos, oldPos, subValCnt;
    string subVals[16];
    float version = 0.0;
    int16_t versionSubValShift = 0;

    while (getline(in, str)) {
        pos = str.find("=");
        if (str.substr(0, 1) != "#") {
            for (uint8_t i = 0; i < 16; i++) {
                subVals[i] = "";
            }
            arg = str.substr(0, pos);

            if (pos > 0 && (int32_t)str.length() > pos) {
                value = str.substr(pos + 1);
                //cout << "ARG: " << arg << endl;
                for (uint16_t i = i; i < 16; i++) {
                    subVals[i] = "";
                }
                oldPos = 0;
                subValCnt = 0;
                do {
                    pos = value.find(";", oldPos);
                    if (pos == -1) {
                       subVals[subValCnt] = value.substr(oldPos, value.length() - oldPos);
                    } else {
                        subVals[subValCnt] = value.substr(oldPos, pos - oldPos);
                    }
                    oldPos = pos + 1;
                    if (subValCnt < 16) {
                        subValCnt++;
                    }
                } while(pos != -1);

                if (arg == "version") {
                    version = atof(subVals[0].c_str());
                    if (version >= 0.5) {
                        versionSubValShift = 1;
                    }
                }

                if (arg == "speaker") {
                    BinauralEngine::SpeakerBE *speaker = binaural.addSpeaker(subVals[0 + versionSubValShift],
                                            atof(subVals[1 + versionSubValShift].c_str()),
                                            atof(subVals[2 + versionSubValShift].c_str()),
                                            atof(subVals[3 + versionSubValShift].c_str()),
                                            atof(subVals[4 + versionSubValShift].c_str()));
                    if (subVals[5 + versionSubValShift] != ""); //todo: start time

                    if (subVals[6 + versionSubValShift] != "") {
                        binaural.addSpeakerKeyFramesByString(speaker, subVals[6 + versionSubValShift]);
                    }
                    if (subVals[7 + versionSubValShift] != "") {
                        speaker->flags = atoi(subVals[7 + versionSubValShift].c_str());
                    }
                }

                if (arg == "rndspawnspeaker") {
                   binaural.addRndSpeaker(subVals[0 + versionSubValShift],
                            atof(subVals[1 + versionSubValShift].c_str()), // x1
                            atof(subVals[2 + versionSubValShift].c_str()), // y1
                            atof(subVals[3 + versionSubValShift].c_str()), // z1
                            atof(subVals[4 + versionSubValShift].c_str()), // x2
                            atof(subVals[5 + versionSubValShift].c_str()), // y2
                            atof(subVals[6 + versionSubValShift].c_str()), // z2
                            atof(subVals[7 + versionSubValShift].c_str()), // volume
                            atof(subVals[8 + versionSubValShift].c_str()), // total time
                            atof(subVals[9 + versionSubValShift].c_str()), // min pause
                            atof(subVals[10 + versionSubValShift].c_str()),  // max pause
                            atof(subVals[11 + versionSubValShift].c_str()), // min speed
                            atof(subVals[12 + versionSubValShift].c_str()), // max speed
                            atoi(subVals[13 + versionSubValShift].c_str())

                    );
                }

                if (arg == "microphone") {
                    binaural.addMicrophone(subVals[0 + versionSubValShift],
                                        atof(subVals[1 + versionSubValShift].c_str()),
                                        atof(subVals[2 + versionSubValShift].c_str()),
                                        atof(subVals[3 + versionSubValShift].c_str()));
                }

                /* Link two microphone objects to generate a stereo file */
                if (arg == "2chanmic") {
                    Microphone *micCh1 = binaural.addMicrophone(subVals[0 + versionSubValShift],
                                        atof(subVals[1 + versionSubValShift].c_str()),
                                        atof(subVals[2 + versionSubValShift].c_str()),
                                        atof(subVals[3 + versionSubValShift].c_str()));
                    binaural.addMicrophone(micCh1,
                                        atof(subVals[4 + versionSubValShift].c_str()),
                                        atof(subVals[5 + versionSubValShift].c_str()),
                                        atof(subVals[6 + versionSubValShift].c_str()));

                }

                /* A head creates two microhpones with a given distance and angle around the desired
                3D location and links them to write a stereo file */
                if (arg == "head") {
                    Pos mp1, mp2, headpos;
                    headpos.x = atof(subVals[1 + versionSubValShift].c_str());
                    headpos.y = atof(subVals[2 + versionSubValShift].c_str());
                    headpos.z = atof(subVals[3 + versionSubValShift].c_str());
                    mp1 = headpos;
                    mp2 = headpos;
                    double angle = atof(subVals[4 + versionSubValShift].c_str());
                    double dist =  atof(subVals[5 + versionSubValShift].c_str());
                    mp1.x = headpos.x - cos(angle / 360.0 * 2.0 * M_PI) * dist / 2;
                    mp1.y = headpos.y + sin(angle / 360.0 * 2.0 * M_PI) * dist / 2;

                    mp2.x = headpos.x - cos((180 + angle) / 360.0 * 2.0 * M_PI) * dist / 2;
                    mp2.y = headpos.y + sin((180 + angle) / 360.0 * 2.0 * M_PI) * dist / 2;

                    Microphone *mic1 = binaural.addMicrophone(subVals[0 + versionSubValShift], mp1.x, mp1.y, mp1.z);
                    binaural.addMicrophone(mic1, mp2.x, mp2.y, mp2.z);
                    cout << "Created head microphones at: x=" << mp1.x << ", y=" << mp1.y << " and x="<<mp2.x << ", y=" << mp2.y << endl;

                }

                if (arg == "wall") {
                    float x1 = atof(subVals[0 + versionSubValShift].c_str()), y1 = atof(subVals[1 + versionSubValShift].c_str());
                    float z1 = atof(subVals[2 + versionSubValShift].c_str()), x2 = atof(subVals[3 + versionSubValShift].c_str());
                    float y2 = atof(subVals[4 + versionSubValShift].c_str()), z2 = atof(subVals[5 + versionSubValShift].c_str());
                    float amount = atof(subVals[6 + versionSubValShift].c_str());
                    Wall *wall = binaural.addWall(x1, y1, z1, x2, y2, z2, amount);
                    if (subVals[7] != "") {
                        wall->modifier.configByString(subVals[7 + versionSubValShift]);
                    }
                }

                if (arg == "room") {
                    float x1 = atof(subVals[0 + versionSubValShift].c_str()), y1 = atof(subVals[1 + versionSubValShift].c_str());
                    float z1 = atof(subVals[2 + versionSubValShift].c_str()), x2 = atof(subVals[3 + versionSubValShift].c_str());
                    float y2 = atof(subVals[4 + versionSubValShift].c_str()), z2 = atof(subVals[5 + versionSubValShift].c_str());
                    float amount = atof(subVals[6 + versionSubValShift].c_str());
                    binaural.addWall(x1, y1, z1, x1, y2, z2, amount); // left
                    binaural.addWall(x2, y1, z1, x2, y2, z2, amount); // right
                    binaural.addWall(x1, y1, z1, x2, y2, z1, amount); // floor
                    binaural.addWall(x1, y1, z2, x2, y2, z2, amount); // ceiling
                    binaural.addWall(x1, y1, z1, x2, y1, z2, amount); // front
                    binaural.addWall(x1, y2, z1, x2, y2, z2, amount); // back
                }

                if (arg == "nrbounces") {
                    binaural.settings.nrBounces = atoi(subVals[0].c_str());
                }

                if (arg == "nrreflections") {
                    binaural.settings.maxReflectionPasses = atoi(subVals[0].c_str());
                }

                if (arg == "saveeverynrofpasses") {
                    binaural.settings.saveMicsEveryNrOfPasses = atoi(subVals[0].c_str());
                }

                if (arg == "samplerate") {
                    binaural.settings.sampleRate = atoi(subVals[0].c_str());
                }

                if (arg == "additionaltime") {
                    binaural.settings.additionalTime = atof(subVals[0].c_str());
                }

                if (arg == "randomseed") {
                    binaural.settings.randomSeed = atoi(subVals[0].c_str());
                }

                if (arg == "airdamping") {
                    binaural.settings.airDamping = atof(subVals[0].c_str());
                }
            }
        }

    }

    in.close();
}

/* Main function, called first. It requires a file name as parameter that is provided in the command line */
int main(int argc, char** argv) {

    BinauralEngine binaural; // The main object


    if (argc >= 2) {

        /* Try to read the file and config the binaural object with the content */
        configEngineByFile(binaural, string(argv[1]));
    } else {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return -1;
    }


    /* Lets the engine prepare its objects and start the process */
    binaural.start();

    /* This prints the generated 3D scene roughly as console characters in top view */
    binaural.printSetup(79, 20, 0);

    /* This is just temporary. It lets the engine do the actual calculation of sound samples completely.
    Later, this function will manage threads and tasks. */
    binaural.update();

    /* Stop objects, write files, free memory */
    binaural.stop(true);

    return 0;
}
