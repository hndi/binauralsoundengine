#ifndef BINAURALPROJECT_H
#define BINAURALPROJECT_H

#include <wx/wx.h>
#include <3DPoint.h>
#include <wx/textfile.h>
#include "guihelpers.h"

class TempEvents {
    public:
        static const int EVT_NO_EVENT = 0;

        static const int EVT_VP_SELECTION = 1;
        static const int EVT_VP_MOVED = 2;

        static const int EVT_PE_RENAME = 10;
        static const int EVT_PE_PROP_CHANGE = 11;

        static const int EVT_WM_CHANGED = 20;


        int lastEvent = EVT_NO_EVENT;
        int objNr = -1;
        wxString lastName = "";
};

class BinauralProject
{
    public:
        static const int MAX_SPEAKERS = 400;
        static const int MAX_RND_SPEAKERS = 400;
        static const int MAX_MICROPHONES = 16;
        static const int MAX_HEADS = 8;
        static const int MAX_WALLS = 100;
        static const int MAX_OBJECTS = 1000;

        static const int OBJ_NONE = -1;
        static const int OBJ_SPEAKER = 0;
        static const int OBJ_RND_SPEAKER = 1;
        static const int OBJ_MICROPHONE = 2;
        static const int OBJ_HEAD = 3;
        static const int OBJ_WALL = 4;

        static const int WT_UNKNOWN = 0;
        static const int WT_TOP = 1;
        static const int WT_FRONT = 2;
        static const int WT_SIDE = 3;

        struct GUIObject {
            wxString name = "";
            wxString file = "";
            int objType = OBJ_NONE;
            c3DPoint pos, pos2;
            short flags = 0;
            bool selected = false;
            double volume = 1.0;
            double totalTime = 60.0;
            double startDelay = 0.0;
            double minPause = 0.0;
            double maxPause = 0.0;
            double minSpeed = 1.0;
            double maxSpeed = 1.0;
            wxString keyFrames = "";
            wxString modifier = "";
            wxString wallGroup = "";
            double angle = 0.0;
            double earDistance = 0.18;
            double amount = 0.9;
        } guiObjects[MAX_OBJECTS];

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
        BinauralProject();
        virtual ~BinauralProject();

        bool saveToFile(wxString fileName);
        bool loadFromFile(wxString fileName);

        bool isWallConnectedToGroup(int objNr);
        bool areWallsConnected(int obj1, int obj2);
        void movePointsInWallGroup(int originObj, c3DPoint oldPos1, c3DPoint oldPos2);
        bool arePointsClose(c3DPoint pt1, c3DPoint pt2, double maxDist);

        int addSpeaker();
        int addRndSpeaker();
        int addMicrophone();
        int addHead();
        int addWall();

        void reset();

        bool setName(int objNr, wxString name);

        int findObject(wxString name);

        int getWallType(int obj);

        void deleteSpeaker(int nr);
        void deleteRndSpeaker(int nr);
        void deleteMicrophone(int nr);
        void deleteHead(int nr);
        void deleteWall(int nr);

        double getModifiedWallAmplitude(int usedModifier, double position, double amplitude, double shift, double waveLength);

        const float version = 0.5;
        TempEvents tempEvent;
    protected:

    private:
};



#endif // BINAURALPROJECT_H
