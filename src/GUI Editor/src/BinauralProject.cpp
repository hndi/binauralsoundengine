#include "BinauralProject.h"

BinauralProject::BinauralProject() {

}

BinauralProject::~BinauralProject() {
}

void BinauralProject::reset() {
    GUIObject tmp;

    for (int i = 0; i < MAX_OBJECTS; i++) {
            guiObjects[i] = tmp;
    }
}

bool BinauralProject::loadFromFile(wxString fileName) {
    wxTextFile  file;
    file.Open(fileName);
    std::string str, arg, value;
    int32_t pos, oldPos, subValCnt;
    std::string subVals[16];
    float version = 0.0;
    int16_t versionSubValShift = 0;
    int objNr;
    bool firstLine = true;

    if (file.IsOpened()) {
        reset();

        while(!file.Eof()) {
            if (firstLine == true) {
                str = file.GetFirstLine();
                firstLine = false;
            } else {
                str = file.GetNextLine();
            }
            pos = str.find("=");
            if (str.substr(0, 1) != "#") {
                for (uint8_t i = 0; i < 16; i++) {
                    subVals[i] = "";
                }
                arg = str.substr(0, pos);

                if (pos > 0 && (int32_t)str.length() > pos) {
                    value = str.substr(pos + 1);
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
                        objNr = addSpeaker();
                        if (version >= 0.5) {
                            guiObjects[objNr].name = subVals[0];
                        }
                        guiObjects[objNr].file = subVals[0 + versionSubValShift];
                        guiObjects[objNr].pos.x = atof(subVals[1 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.y = atof(subVals[2 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.z = atof(subVals[3 + versionSubValShift].c_str());
                        guiObjects[objNr].volume = atof(subVals[4 + versionSubValShift].c_str());
                        guiObjects[objNr].keyFrames = subVals[6 + versionSubValShift];
                        guiObjects[objNr].flags = atoi(subVals[7 + versionSubValShift].c_str());
                    }

                    if (arg == "rndspawnspeaker") {
                        objNr = addRndSpeaker();
                        if (version >= 0.5) {
                            guiObjects[objNr].name = subVals[0];
                        }
                        guiObjects[objNr].file = subVals[0 + versionSubValShift];
                        guiObjects[objNr].pos.x = atof(subVals[1 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.y = atof(subVals[2 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.z = atof(subVals[3 + versionSubValShift].c_str());
                        guiObjects[objNr].pos2.x = atof(subVals[4 + versionSubValShift].c_str());
                        guiObjects[objNr].pos2.y = atof(subVals[5 + versionSubValShift].c_str());
                        guiObjects[objNr].pos2.z = atof(subVals[6 + versionSubValShift].c_str());
                        guiObjects[objNr].volume = atof(subVals[7 + versionSubValShift].c_str());
                        guiObjects[objNr].totalTime = atof(subVals[8 + versionSubValShift].c_str());
                        guiObjects[objNr].minPause = atof(subVals[9 + versionSubValShift].c_str());
                        guiObjects[objNr].maxPause = atof(subVals[10 + versionSubValShift].c_str());
                        guiObjects[objNr].minSpeed = atof(subVals[11 + versionSubValShift].c_str());
                        guiObjects[objNr].maxSpeed = atof(subVals[12 + versionSubValShift].c_str());
                        guiObjects[objNr].flags = atoi(subVals[13 + versionSubValShift].c_str());


                    }

                    if (arg == "microphone") {
                        objNr = addMicrophone();
                        if (version >= 0.5) {
                            guiObjects[objNr].name = subVals[0];
                        }
                        guiObjects[objNr].file = subVals[0 + versionSubValShift];
                        guiObjects[objNr].pos.x = atof(subVals[1 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.y = atof(subVals[2 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.z = atof(subVals[3 + versionSubValShift].c_str());
                    }

                    /* Link two microphone objects to generate a stereo file */
                    if (arg == "2chanmic") {
                        //todo
                        /*
                        Microphone *micCh1 = binaural.addMicrophone(subVals[0 + versionSubValShift],
                                            atof(subVals[1 + versionSubValShift].c_str()),
                                            atof(subVals[2 + versionSubValShift].c_str()),
                                            atof(subVals[3 + versionSubValShift].c_str()));
                        binaural.addMicrophone(micCh1,
                                            atof(subVals[4 + versionSubValShift].c_str()),
                                            atof(subVals[5 + versionSubValShift].c_str()),
                                            atof(subVals[6 + versionSubValShift].c_str()));

                        */
                    }

                    if (arg == "head") {
                        objNr = addHead();
                        if (version >= 0.5) {
                            guiObjects[objNr].name = subVals[0];
                        }
                        guiObjects[objNr].file = subVals[0 + versionSubValShift];
                        guiObjects[objNr].pos.x = atof(subVals[1 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.y = atof(subVals[2 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.z = atof(subVals[3 + versionSubValShift].c_str());
                        guiObjects[objNr].angle = atof(subVals[4 + versionSubValShift].c_str());
                        guiObjects[objNr].earDistance = atof(subVals[5 + versionSubValShift].c_str());
                    }

                    if (arg == "wall") {
                        objNr = addWall();
                        if (version >= 0.5) {
                            guiObjects[objNr].name = subVals[0];
                        }
                        guiObjects[objNr].pos.x = atof(subVals[0 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.y = atof(subVals[1 + versionSubValShift].c_str());
                        guiObjects[objNr].pos.z = atof(subVals[2 + versionSubValShift].c_str());
                        guiObjects[objNr].pos2.x = atof(subVals[3 + versionSubValShift].c_str());
                        guiObjects[objNr].pos2.y = atof(subVals[4 + versionSubValShift].c_str());
                        guiObjects[objNr].pos2.z = atof(subVals[5 + versionSubValShift].c_str());
                        guiObjects[objNr].amount = atof(subVals[6 + versionSubValShift].c_str());
                        guiObjects[objNr].modifier = subVals[7 + versionSubValShift];
                        guiObjects[objNr].wallGroup = subVals[8 + versionSubValShift];

                    }

                    if (arg == "room") {

                        //todo
                        /*
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
                        */
                    }

                    if (arg == "nrbounces") {
                        settings.nrBounces = atoi(subVals[0].c_str());
                    }

                    if (arg == "nrreflections") {
                        settings.maxReflectionPasses = atoi(subVals[0].c_str());
                    }

                    if (arg == "saveeverynrofpasses") {
                        settings.saveMicsEveryNrOfPasses = atoi(subVals[0].c_str());
                    }

                    if (arg == "samplerate") {
                        settings.sampleRate = atoi(subVals[0].c_str());
                    }

                    if (arg == "additionaltime") {
                        settings.additionalTime = atof(subVals[0].c_str());
                    }

                    if (arg == "randomseed") {
                        settings.randomSeed = atoi(subVals[0].c_str());
                    }

                    if (arg == "airdamping") {
                        settings.airDamping = atof(subVals[0].c_str());
                    }
                }
            }

        }
    } else {
        return false;
    }

    return true;
}


bool BinauralProject::saveToFile(wxString fileName) {
    wxString str;
    wxFile file(fileName, wxFile::write);

    if (file.IsOpened()) {
        file.Write("version=" + wxString::Format(wxT("%f"), version) + '\n');
        for (int i = 0; i < MAX_OBJECTS; i++) {
            switch(guiObjects[i].objType) {
            case OBJ_SPEAKER:
                str = "speaker=" + guiObjects[i].name + ";" +
                      guiObjects[i].file + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.z) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].volume) + ";;" +
                      guiObjects[i].keyFrames + ";" +
                      wxString::Format(wxT("%i"), guiObjects[i].flags);
                      file.Write(str + '\n');
                break;
            case OBJ_HEAD:
                str = "head=" + guiObjects[i].name + ";" +
                      guiObjects[i].file + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.z) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].angle) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].earDistance);
                file.Write(str + '\n');
                break;
            case OBJ_MICROPHONE:
                str = "microphone=" + guiObjects[i].name + ";" +
                      guiObjects[i].file + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.z);
                file.Write(str + '\n');
                break;
            case OBJ_RND_SPEAKER:
                 str = "rndspawnspeaker=" + guiObjects[i].name + ";" +
                      guiObjects[i].file + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.z) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos2.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos2.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos2.z) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].volume) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].totalTime) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].minPause) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].maxPause) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].minSpeed) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].maxSpeed) + ";" +
                      wxString::Format(wxT("%i"), guiObjects[i].flags);;
                file.Write(str + '\n');
                break;
            case OBJ_WALL:
                 str = "wall=" + guiObjects[i].name + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos.z) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos2.x) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos2.y) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].pos2.z) + ";" +
                      wxString::Format(wxT("%f"), guiObjects[i].amount) + ";" +
                      guiObjects[i].modifier + ";" +
                      guiObjects[i].wallGroup;
                file.Write(str + '\n');
                break;
            }
        }
        file.Write("samplerate=" + wxString::Format(wxT("%i"), settings.sampleRate) + '\n');
        file.Write("additionaltime=" + wxString::Format(wxT("%f"), settings.additionalTime) + '\n');
        file.Write("nrbounces=" + wxString::Format(wxT("%i"), settings.nrBounces) + '\n');
        file.Write("nrreflections=" + wxString::Format(wxT("%i"), settings.maxReflectionPasses) + '\n');
        file.Write("randomseed=" + wxString::Format(wxT("%i"), settings.randomSeed) + '\n');
        file.Write("airdamping=" + wxString::Format(wxT("%f"), settings.airDamping) + '\n');
        file.Write("saveeverynrofpasses=" + wxString::Format(wxT("%i"), settings.saveMicsEveryNrOfPasses) + '\n');
        file.Close();
        return true;
    }
    return false;
}



int BinauralProject::addSpeaker() {
    wxString n;
    int cnt = 1;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (guiObjects[i].name == "") {
            do {
                n = "Speaker" + wxString::Format(wxT("%i"), cnt);
                cnt++;
            } while (findObject(n) != -1);

            GUIObject t;
            guiObjects[i] = t;
            guiObjects[i].name = n;
            guiObjects[i].objType = OBJ_SPEAKER;
            return i;
        }
    }
    return -1;
}

int BinauralProject::addRndSpeaker() {
    wxString n;
    int cnt = 1;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (guiObjects[i].name == "") {
            do {
                n = "RndSpeaker" + wxString::Format(wxT("%i"), cnt);
                cnt++;
            } while (findObject(n) != -1);

            GUIObject t;
            guiObjects[i] = t;
            guiObjects[i].name = n;
            guiObjects[i].objType = OBJ_RND_SPEAKER;
            return i;
        }
    }
    return -1;
}

int BinauralProject::addMicrophone() {
    wxString n;
    int cnt = 1;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (guiObjects[i].name == "") {
            do {
                n = "Mic" + wxString::Format(wxT("%i"), cnt);
                cnt++;
            } while (findObject(n) != -1);

            GUIObject t;
            guiObjects[i] = t;
            guiObjects[i].name = n;
            guiObjects[i].objType = OBJ_MICROPHONE;
            return i;
        }
    }
    return -1;
}

int BinauralProject::addHead() {
    wxString n;
    int cnt = 1;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (guiObjects[i].name == "") {
            do {
                n = "3DHead" + wxString::Format(wxT("%i"), cnt);
                cnt++;
            } while (findObject(n) != -1);

            GUIObject t;
            guiObjects[i] = t;
            guiObjects[i].name = n;
            guiObjects[i].objType = OBJ_HEAD;
            return i;
        }
    }
    return -1;
}

int BinauralProject::addWall() {
    wxString n;
    int cnt = 1;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (guiObjects[i].name == "") {
            do {
                n = "Wall" + wxString::Format(wxT("%i"), cnt);
                cnt++;
            } while (findObject(n) != -1);

            GUIObject t;
            guiObjects[i] = t;
            guiObjects[i].name = n;
            guiObjects[i].objType = OBJ_WALL;
            return i;
        }
    }
    return -1;
}

int BinauralProject::findObject(wxString name) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (guiObjects[i].name == name) {
            return i;
        }
    }
    return -1;
}

bool BinauralProject::setName(int objNr, wxString name) {
    if (objNr >= 0 && objNr < MAX_OBJECTS) {
        int x;
        if ((x = findObject(name)) == -1) {
            guiObjects[objNr].name = name;
            return true;
        }

        int cnt = 2;
        wxString n;
        do {
            n = name + wxString::Format(wxT("%i"), cnt);
            cnt++;
        } while (findObject(n) != -1);

        guiObjects[objNr].name = n;
    }
    return false;
}

void BinauralProject::deleteSpeaker(int nr) {
}

void BinauralProject::deleteRndSpeaker(int nr) {
}

void BinauralProject::deleteMicrophone(int nr) {
}

void BinauralProject::deleteHead(int nr) {
}

void BinauralProject::deleteWall(int nr) {
}

int BinauralProject::getWallType(int obj) {
    if (obj >= 0 && obj < MAX_OBJECTS && guiObjects[obj].objType == OBJ_WALL) {
        if (abs(guiObjects[obj].pos2.z - guiObjects[obj].pos.z) < .05) {
            return WT_TOP;
        }
        if (abs(guiObjects[obj].pos2.y - guiObjects[obj].pos.y) < .05) {
            return WT_FRONT;
        }
        if (abs(guiObjects[obj].pos2.x - guiObjects[obj].pos.x) < .05) {
            return WT_SIDE;
        }
    }

    return WT_UNKNOWN;
}

bool BinauralProject::isWallConnectedToGroup(int objNr) {
    if (objNr < 0 || objNr >= MAX_OBJECTS || guiObjects[objNr].objType != OBJ_WALL || guiObjects[objNr].wallGroup == "") {
        return false;
    }

    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (i != objNr && guiObjects[i].objType == OBJ_WALL && guiObjects[i].wallGroup == guiObjects[objNr].wallGroup) {
            if (areWallsConnected(objNr, i) == true) {
                return true;
            }
        }
    }

    return false;
}

bool BinauralProject::areWallsConnected(int obj1, int obj2) {
    c3DPoint pt[2][4];
    int objNr[2] = {obj1, obj2};
    int objType;

    pt[0][0] = guiObjects[obj1].pos;
    pt[0][1] = guiObjects[obj1].pos2;
    pt[1][0] = guiObjects[obj2].pos;
    pt[1][1] = guiObjects[obj2].pos2;
    for (int ptNr = 0; ptNr < 2; ptNr++) {
        objType = getWallType(objNr[ptNr]);
        switch (objType) {
            case WT_TOP:
                pt[ptNr][2].z = pt[ptNr][0].z;
                pt[ptNr][3].z = pt[ptNr][1].z;

                pt[ptNr][2].y = pt[ptNr][0].y;
                pt[ptNr][3].y = pt[ptNr][1].y;

                pt[ptNr][2].x = pt[ptNr][1].x;
                pt[ptNr][3].x = pt[ptNr][0].x;
                break;

            case WT_FRONT:
                pt[ptNr][2].y = pt[ptNr][0].y;
                pt[ptNr][3].y = pt[ptNr][1].y;

                pt[ptNr][2].z = pt[ptNr][1].z;
                pt[ptNr][3].z = pt[ptNr][0].z;

                pt[ptNr][2].x = pt[ptNr][0].x;
                pt[ptNr][3].x = pt[ptNr][1].x;
                break;

            case WT_SIDE:
                pt[ptNr][2].x = pt[ptNr][0].x;
                pt[ptNr][3].x = pt[ptNr][1].x;

                pt[ptNr][2].y = pt[ptNr][0].y;
                pt[ptNr][3].y = pt[ptNr][1].y;

                pt[ptNr][2].z = pt[ptNr][1].z;
                pt[ptNr][3].z = pt[ptNr][0].z;
                break;
        }
    }

    wxString tmp = "";
    for (int ptNr1 = 0; ptNr1 < 4; ptNr1++) {
        for (int ptNr2 = 0; ptNr2 < 4; ptNr2++) {
            if (arePointsClose(pt[0][ptNr1], pt[1][ptNr2], 0.05) == true) {
                return true;
            }
        }
    }
    return false;
}

bool BinauralProject::arePointsClose(c3DPoint pt1, c3DPoint pt2, double maxDist) {
    if (abs(pt1.x - pt2.x) <= maxDist &&
        abs(pt1.y - pt2.y) <= maxDist &&
        abs(pt1.z - pt2.z) <= maxDist) {


        return true;
    }

    return false;
}

void BinauralProject::movePointsInWallGroup(int originObj, c3DPoint oldPos1, c3DPoint oldPos2) {
    c3DPoint pt[2][4];
    c3DPoint oldPosOfMovedWall1, oldPosOfMovedWall2;
    bool moved;
    double totalMovement;

    pt[0][0] = oldPos1;
    pt[0][1] = oldPos2;
    pt[1][0] = guiObjects[originObj].pos;
    pt[1][1] = guiObjects[originObj].pos2;

    int objType = getWallType(originObj);
    for (int ptNr = 0; ptNr < 2; ptNr++) {

        switch (objType) {
            case WT_TOP:
                pt[ptNr][2].z = pt[ptNr][0].z;
                pt[ptNr][3].z = pt[ptNr][1].z;

                pt[ptNr][2].y = pt[ptNr][0].y;
                pt[ptNr][3].y = pt[ptNr][1].y;

                pt[ptNr][2].x = pt[ptNr][1].x;
                pt[ptNr][3].x = pt[ptNr][0].x;
                break;

            case WT_FRONT:
                pt[ptNr][2].y = pt[ptNr][0].y;
                pt[ptNr][3].y = pt[ptNr][1].y;

                pt[ptNr][2].z = pt[ptNr][1].z;
                pt[ptNr][3].z = pt[ptNr][0].z;

                pt[ptNr][2].x = pt[ptNr][0].x;
                pt[ptNr][3].x = pt[ptNr][1].x;
                break;

            case WT_SIDE:
                pt[ptNr][2].x = pt[ptNr][0].x;
                pt[ptNr][3].x = pt[ptNr][1].x;

                pt[ptNr][2].y = pt[ptNr][0].y;
                pt[ptNr][3].y = pt[ptNr][1].y;

                pt[ptNr][2].z = pt[ptNr][1].z;
                pt[ptNr][3].z = pt[ptNr][0].z;
                break;
        }
    }
    for (int objNr = 0; objNr < MAX_OBJECTS; objNr++) {
        if (objNr != originObj && guiObjects[objNr].objType == OBJ_WALL && guiObjects[objNr].wallGroup == guiObjects[originObj].wallGroup) {
            oldPosOfMovedWall1 = guiObjects[objNr].pos;
            oldPosOfMovedWall2 = guiObjects[objNr].pos2;
            moved = false;
            totalMovement = 0.0;

            for (int x = 0; x < 4; x++) {
                if (arePointsClose(pt[0][x], guiObjects[objNr].pos, 0.05) == true) {
                    guiObjects[objNr].pos = pt[1][x];
                    if (abs(oldPosOfMovedWall1.x - oldPosOfMovedWall2.x) < 0.05) {
                        totalMovement += abs(guiObjects[objNr].pos2.x - guiObjects[objNr].pos.x);
                        guiObjects[objNr].pos2.x = guiObjects[objNr].pos.x;
                    }
                    if (abs(oldPosOfMovedWall1.y - oldPosOfMovedWall2.y) < 0.05) {
                        totalMovement += abs(guiObjects[objNr].pos2.y - guiObjects[objNr].pos.y);
                        guiObjects[objNr].pos2.y = guiObjects[objNr].pos.y;
                    }
                    if (abs(oldPosOfMovedWall1.z - oldPosOfMovedWall2.z) < 0.05) {
                        totalMovement += abs(guiObjects[objNr].pos2.z - guiObjects[objNr].pos.z);
                        guiObjects[objNr].pos2.z = guiObjects[objNr].pos.z;
                    }
                    moved = true;
                }

                if (arePointsClose(pt[0][x], guiObjects[objNr].pos2, 0.05) == true) {
                    guiObjects[objNr].pos2 = pt[1][x];

                    if (abs(oldPosOfMovedWall1.x - oldPosOfMovedWall2.x) < 0.05) {
                        totalMovement += abs(guiObjects[objNr].pos.x - guiObjects[objNr].pos2.x);
                        guiObjects[objNr].pos.x = guiObjects[objNr].pos2.x;
                    }
                    if (abs(oldPosOfMovedWall1.y - oldPosOfMovedWall2.y) < 0.05) {
                        totalMovement += abs(guiObjects[objNr].pos.y - guiObjects[objNr].pos2.y);
                        guiObjects[objNr].pos.y = guiObjects[objNr].pos2.y;
                    }
                    if (abs(oldPosOfMovedWall1.z - oldPosOfMovedWall2.z) < 0.05) {
                        totalMovement += abs(guiObjects[objNr].pos.z - guiObjects[objNr].pos2.z);
                        guiObjects[objNr].pos.z = guiObjects[objNr].pos2.z;
                    }
                    moved = true;
                }

                if (moved == true && totalMovement > 0.00001) {

                    movePointsInWallGroup(objNr, oldPosOfMovedWall1, oldPosOfMovedWall2);
                }
            }
        }
    }
}

double BinauralProject::getModifiedWallAmplitude(int usedModifier, double position, double amplitude, double shift, double waveLength) {
    double tmp;
    const int MOD_NO_MOD = 0;
    const int MOD_SINE = 1;
    const int MOD_SQUARE = 2;
    const int MOD_TRIANGLE = 3;

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
