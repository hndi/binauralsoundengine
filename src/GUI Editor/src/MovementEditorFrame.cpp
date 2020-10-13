#include "MovementEditorFrame.h"

BEGIN_EVENT_TABLE(MovementEditorFrame, wxFrame)

    EVT_TIMER(1235, MovementEditorFrame::OnTimer)
    EVT_SLIDER(MovementEditorFrame::idSliderTime, MovementEditorFrame::OnSlideTime)
    EVT_BUTTON(MovementEditorFrame::idBtnPlay, MovementEditorFrame::OnButtonPlayClick)
    EVT_BUTTON(MovementEditorFrame::idBtnTop, MovementEditorFrame::OnButtonTopClick)
    EVT_BUTTON(MovementEditorFrame::idBtnFront, MovementEditorFrame::OnButtonFrontClick)
    EVT_BUTTON(MovementEditorFrame::idBtnSide, MovementEditorFrame::OnButtonSideClick)
    EVT_TEXT(MovementEditorFrame::idTxtSpeedfactor, MovementEditorFrame::OnTextChange)
    EVT_TEXT(MovementEditorFrame::idTxtStartDelay, MovementEditorFrame::OnTextChange)
    EVT_TEXT(MovementEditorFrame::idTxtTimeCurr, MovementEditorFrame::OnTextChange)
    EVT_TEXT(MovementEditorFrame::idTxtTimeEnd, MovementEditorFrame::OnTextChange)
    EVT_BUTTON(MovementEditorFrame::idBtnRecAbs, MovementEditorFrame::OnRecAbsClick)
    EVT_BUTTON(MovementEditorFrame::idBtnRecRel, MovementEditorFrame::OnRecRelClick)
    EVT_BUTTON(MovementEditorFrame::idBtnSmooth, MovementEditorFrame::OnButtonSmoothClick)
    EVT_SLIDER(MovementEditorFrame::idSliderSmooth, MovementEditorFrame::OnSmoothSlide)

END_EVENT_TABLE()

MovementEditorFrame::MovementEditorFrame(wxFrame *frame, const wxString& title, BinauralProject *project, int objectNr)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(800, 600), 0x40 | 0x800 | 0x400 | 0x200 | 0x1000 | wxCAPTION | wxCLIP_CHILDREN) {

    CreateStatusBar(2);
    SetStatusText(_(""), 0);

    myProject = project;
    currentObject = objectNr;

    wxBoxSizer *v0Sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *h1PlaybackSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *h1Sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *v2Sizer = new wxBoxSizer(wxVERTICAL);


    wxPanel *panel = new wxPanel(this, wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTAB_TRAVERSAL,
                                     _("mypanel"));
    panel->SetSizer(v0Sizer);

    noEvents = true;
    wxStaticBox *boxTest = new wxStaticBox(panel, wxID_ANY, "Record Keyframes", wxPoint(0, 0), wxSize(180, 170));
    btnRecAbs = new wxButton(boxTest, idBtnRecAbs, "&Record", wxPoint(5, 0 * 30), wxSize(170,25));
    btnRecRel = new wxButton(boxTest, idBtnRecRel, "Record (&add)", wxPoint(5, 1 * 30), wxSize(170,25));
    new wxStaticText(boxTest, wxID_ANY, "Keyframe interval (s):", wxPoint(5, 3 * 20), wxSize(170, 20));
    txtRecordInterval = new wxTextCtrl(boxTest, idTxtRecInterval, fToStr(recordInterval), wxPoint(5, 4 * 20), wxSize(170, 20));
    new wxStaticText(boxTest, wxID_ANY, "Smoothed recording:", wxPoint(5, 5 * 20), wxSize(170,20));
    sliderSmooth = new wxSlider(boxTest, idSliderSmooth, 0, 0, 1000, wxPoint(5, 6 * 20), wxSize(170,20));

    wxStaticBox *boxTest2 = new wxStaticBox(panel, wxID_ANY, "Options", wxPoint(0, 0), wxSize(180, 200));

    new wxStaticText(boxTest2, wxID_ANY, "Speaker start delay:", wxPoint(5, 0 * 20), wxSize(170, 20));
    txtStartDelay = new wxTextCtrl(boxTest2, idTxtStartDelay, fToStr(myProject->guiObjects[currentObject].startDelay), wxPoint(5, 1 * 20), wxSize(170, 20));
    new wxStaticText(boxTest2, wxID_ANY, "Speed factor (editor):", wxPoint(5, 2 * 20), wxSize(170, 20));
    txtSpeedFactor = new wxTextCtrl(boxTest2, idTxtSpeedfactor, fToStr(speedFactor), wxPoint(5, 3 * 20), wxSize(170, 20));
    btnSmooth = new wxButton(boxTest2, idBtnSmooth, "Smooth", wxPoint(5, 6 * 20), wxSize(170,20));


    btnPlay = new wxButton(panel, idBtnPlay, ">", wxPoint(0, 0), wxSize(32, 32));

    wxBoxSizer *sizerTimeCurrV = new wxBoxSizer(wxVERTICAL);
    wxStaticText *txtTimeCurr = new wxStaticText(panel, wxID_ANY, "Time (s):");
    editTimeCurr = new wxTextCtrl(panel, idTxtTimeCurr, "0.0", wxDefaultPosition, wxSize(64, 20));
    sizerTimeCurrV->Add(txtTimeCurr, 0, wxALL, 0);
    sizerTimeCurrV->Add(editTimeCurr, 0, wxALL, 0);
    sliderTime = new wxSlider(panel, idSliderTime, 0, 0, 1000, wxPoint(96, 0), wxSize(4000, 32));
    wxBoxSizer *sizerTimeEndV = new wxBoxSizer(wxVERTICAL);
    wxStaticText *txtTimeEnd = new wxStaticText(panel, wxID_ANY, "End (s):");
    editTimeEnd = new wxTextCtrl(panel, idTxtTimeEnd, fToStr(maxTime), wxDefaultPosition, wxSize(64, 20));
    sizerTimeEndV->Add(txtTimeEnd, 0, wxALL, 0);
    sizerTimeEndV->Add(editTimeEnd, 0, wxALL, 0);
    h1PlaybackSizer->Add(btnPlay, 0, wxALL, 5);
    h1PlaybackSizer->Add(sizerTimeCurrV, 0, wxALL, 5);
    h1PlaybackSizer->Add(sliderTime, 1, wxALL | wxEXPAND, 5);
    h1PlaybackSizer->Add(sizerTimeEndV, 0, wxALL, 5);


    viewPort = new ViewPort(panel, wxDefaultPosition, wxSize(4000, 4000));
    viewPort->setProject(myProject);
    viewPort->time = myProject->guiObjects[currentObject].startDelay;
    viewPort->flags = ViewPort::VP_NO_SELECTION | ViewPort::VP_SHOW_MOV_POS_ALL;
    viewPort->setCenterOfView(myProject->guiObjects[currentObject].pos);

    btnTopView = new wxButton(viewPort, idBtnTop, "T", wxPoint(0, 0), wxSize(32, 32));
    btnFrontView = new wxButton(viewPort, idBtnFront, "F", wxPoint(32, 0), wxSize(32, 32));
    btnSideView = new wxButton(viewPort, idBtnSide, "S", wxPoint(64, 0), wxSize(32, 32));

    v2Sizer->Add(boxTest, 0, wxALL, 0);
    v2Sizer->Add(boxTest2, 0, wxALL, 0);

    h1Sizer->Add(v2Sizer, 0, wxALL, 5);
    h1Sizer->Add(viewPort, 0, wxALL, 5);

    v0Sizer->Add(h1PlaybackSizer,0 , wxALL, 0);
    v0Sizer->Add(h1Sizer, 1, wxALL, 0);

    timer = new wxTimer(this, 1235);

    recordedKeyFrames = myProject->guiObjects[currentObject].keyFrames;
    origKeyFrames = myProject->guiObjects[currentObject].keyFrames;

    noEvents = false;

    updateEnabledDisabled();

}

MovementEditorFrame::~MovementEditorFrame()
{
    if (playing == true || recording == true) {
        timer->Stop();
    }
}

void MovementEditorFrame::OnTimer(wxTimerEvent &evt) {
    if (playing == true) {
        uint64_t currTime = timeSinceEpochMillisec();
        timeStamp = (currTime - startTimeMS) * speedFactor / 1000;
        if (timeStamp >= maxTime) {
            timeStamp = maxTime;
            wxCommandEvent tmp;
            OnButtonPlayClick(tmp);
        }
        editTimeCurr->SetValue(fToStr(round(timeStamp * 100) / 100));
        viewPort->time = timeStamp + myProject->guiObjects[currentObject].startDelay;
        viewPort->Refresh();
        sliderTime->SetValue(timeStamp / maxTime * sliderTime->GetMax());
    }


    if (recording == true) {
        uint64_t currTime = timeSinceEpochMillisec();
        timeStamp = (currTime - startTimeMS) * speedFactor / 1000;

        if (recordCountDown == 0) {

            if (timeStamp >= maxTime) {
                timeStamp = 0.0;
                stopRecording();
            }

            if (recordingAbsolute == true) {

                if (timeStamp - lastKeyframeRecorded >= recordInterval / speedFactor) {

                    wxString tmp;

                    if (myProject->guiObjects[currentObject].keyFrames != "") {
                        tmp = "/";
                    }

                    c3DPoint tmp3Dpt = viewPort->lastMousePosition;
                    tmp3Dpt.x -= myProject->guiObjects[currentObject].pos.x;
                    tmp3Dpt.y -= myProject->guiObjects[currentObject].pos.y;
                    tmp3Dpt.z -= myProject->guiObjects[currentObject].pos.z;
                    tmp += fToStr(timeStamp) + ","
                        + fToStr(tmp3Dpt.x) + ","
                        + fToStr(tmp3Dpt.y) + ","
                        + fToStr(tmp3Dpt.z);

                    lastKeyframeRecorded = timeStamp;

                     //test:
                     myProject->guiObjects[currentObject].keyFrames = recordedKeyFrames;

                    myProject->guiObjects[currentObject].keyFrames += tmp;


                    //test:
                    recordedKeyFrames = myProject->guiObjects[currentObject].keyFrames;
                    myProject->smoothKeyFrames(currentObject, smoothing);
                    viewPort->time = timeStamp + myProject->guiObjects[currentObject].startDelay;
                    viewPort->Refresh();
                }

            } else { //recording relative
                static c3DPoint lastMousePosCheck;

                if (lastMousePosCheck != viewPort->lastMousePosition || timeStamp >= nextKeyFrameData.time) {
                    lastMousePosCheck = viewPort->lastMousePosition;
                    applyMouseChangesToCurrentKeyFrame();
                }

                if (timeStamp >= nextKeyFrameData.time) {

                    findNextKeyFramePosition();
                    if (lastKeyFrameReached == false) {
                        getNextKeyFrameData();
                        applyMouseChangesToCurrentKeyFrame();
                    } else {
                        stopRecording();
                    }
                }
                viewPort->time = timeStamp + myProject->guiObjects[currentObject].startDelay;
                viewPort->Refresh();
            }

            editTimeCurr->SetValue(fToStr(round(timeStamp * 100) / 100));
            sliderTime->SetValue(timeStamp / maxTime * sliderTime->GetMax());



        } else { //Record countdown running

            if (timeStamp / speedFactor <= recordCountDown) {
                if (recordingAbsolute == true) {
                    btnRecAbs->SetLabel(fToStr((int)(1 + recordCountDown - timeStamp / speedFactor)));
                } else {
                    btnRecRel->SetLabel(fToStr((int)(1 + recordCountDown - timeStamp / speedFactor)));
                    startMousePoint = viewPort->lastMousePosition;
                }
            } else {
                recordCountDown = 0;
                startTimeMS = timeSinceEpochMillisec();
                if (recordingAbsolute == true) {
                    btnRecAbs->SetLabel("&Stop (ALT+S)");
                } else {
                    btnRecRel->SetLabel("&Stop (ALT+S)");
                }
            }
        }
    }
    evt.Skip();
}

void MovementEditorFrame::OnButtonTopClick(wxCommandEvent &event) {
    viewPort->setView(ViewPort::VIEW_TOP);
    viewPort->Refresh();

    event.Skip();
}

void MovementEditorFrame::OnButtonFrontClick(wxCommandEvent &event) {
    viewPort->setView(ViewPort::VIEW_FRONT);
    viewPort->Refresh();

    event.Skip();
}

void MovementEditorFrame::OnButtonSideClick(wxCommandEvent &event) {
    viewPort->setView(ViewPort::VIEW_SIDE);
    viewPort->Refresh();

    event.Skip();
}

void MovementEditorFrame::OnButtonPlayClick(wxCommandEvent &event) {
    playing = !playing;

    if (playing == true) {
        btnPlay->SetLabel("||");
        timer->Start(25);
        startTimeMS = timeSinceEpochMillisec();

    } else {
        btnPlay->SetLabel(">");
        timer->Stop();
    }
    event.Skip();
}

void MovementEditorFrame::OnSlideTime(wxCommandEvent &event) {
    timeStamp = maxTime / 1000.0 * sliderTime->GetValue();
    editTimeCurr->SetValue(fToStr(round(timeStamp * 100) / 100.0));
    viewPort->time = timeStamp + myProject->guiObjects[currentObject].startDelay;
    viewPort->Refresh();

    event.Skip();
}

void MovementEditorFrame::OnTextChange(wxCommandEvent &event) {
    if (noEvents == true) {
        event.Skip();
        return;
    }

    switch (event.GetId()) {
        case idTxtSpeedfactor:
            speedFactor = atof(txtSpeedFactor->GetValue());
            break;

        case idTxtStartDelay:
            myProject->guiObjects[currentObject].startDelay = atof(txtStartDelay->GetValue());
            viewPort->time = timeStamp + myProject->guiObjects[currentObject].startDelay;
            viewPort->Refresh();
            break;

        case idTxtTimeCurr:
            if (recording == false && playing == false) {
                timeStamp = atof(editTimeCurr->GetValue());
                sliderTime->SetValue(timeStamp / maxTime * sliderTime->GetMax());
                viewPort->time = timeStamp + myProject->guiObjects[currentObject].startDelay;
                viewPort->Refresh();
            }
            break;

        case idTxtTimeEnd:
            maxTime = atof(editTimeEnd->GetValue());
            if (maxTime <= 0) {
                maxTime = 0.1;
            }
            sliderTime->SetValue(timeStamp / maxTime * sliderTime->GetMax());
            break;
    }

    event.Skip();
}

void MovementEditorFrame::OnRecAbsClick(wxCommandEvent &event) {
    if (recording == false && recordCountDown == 0) {
        recordInterval = atof(txtRecordInterval->GetValue()) * speedFactor;
        if (speedFactor <= 0) {
            wxMessageBox("Speed factor has to bee larger than 0.");
            event.Skip();
            return;
        }
        if (playing == true) {
            playing = false;
            timer->Stop();
        }
        recording = true;
        recordingAbsolute = true;
        myProject->guiObjects[currentObject].keyFrames = "";
        recordedKeyFrames = "";
        timeStamp = 0;
        lastKeyframeRecorded = 0;
        viewPort->Refresh();

        startTimeMS = timeSinceEpochMillisec();
        recordCountDown = 3;
        btnRecAbs->SetLabel(fToStr(recordCountDown));
        timer->Start(10);
        updateEnabledDisabled();
    } else {
        stopRecording();
    }

    event.Skip();
}

void MovementEditorFrame::OnRecRelClick(wxCommandEvent &event) {
    if (recording == false && recordCountDown == 0) {
        recordInterval = atof(txtRecordInterval->GetValue()) * speedFactor;
        if (speedFactor <= 0) {
            wxMessageBox("Speed factor has to bee larger than 0.");
            event.Skip();
            return;
        }
        if (playing == true) {
            playing = false;
            timer->Stop();
        }
        recording = true;
        recordingAbsolute = false;
        recRelSmoother = c3DPoint();
        nextKeyFrameEnd = 0;
        nextKeyFrameStart = 0;
        findNextKeyFramePosition();
        getNextKeyFrameData();
        updateEnabledDisabled();

        startTimeMS = timeSinceEpochMillisec();
        recordCountDown = 3;
        btnRecRel->SetLabel(fToStr(recordCountDown));
        viewPort->flags |= ViewPort::VP_HIGHLIGHT_NEXT_KF;
        timer->Start(10);
    } else {
        stopRecording();
    }
}

void MovementEditorFrame::OnKeyPress(wxKeyEvent &event) {

}

void MovementEditorFrame::stopRecording() {
    if (recording == true || recordCountDown != 0) {
        recordedKeyFrames = myProject->guiObjects[currentObject].keyFrames;
        recording = false;
        lastKeyFrameReached = false;
        timer->Stop();
        recordCountDown = 0;
        viewPort->flags &= ~ViewPort::VP_HIGHLIGHT_NEXT_KF;
    }

    if (recordingAbsolute == true) {
        btnRecAbs->SetLabel("&Record");
    } else {
        btnRecRel->SetLabel("Record (&add)");
    }
    updateEnabledDisabled();
}

void MovementEditorFrame::OnButtonSmoothClick(wxCommandEvent &event) {
    myProject->smoothKeyFrames(currentObject, 1.1);
    viewPort->Refresh();

}

void MovementEditorFrame::OnSmoothSlide(wxCommandEvent &event) {
    smoothing = sqrt(sliderSmooth->GetValue() / 200.0) + 1;
}

void MovementEditorFrame::updateEnabledDisabled() {
    if (myProject->guiObjects[currentObject].keyFrames == "") {
        btnRecRel->Enable(false);
    }

    if (recording == true) {
        btnPlay->Enable(false);

        if (recordingAbsolute == true) {
            btnRecRel->Enable(false);
        } else {
            btnRecAbs->Enable(false);
        }

    } else {
        btnPlay->Enable(true);

        btnRecAbs->Enable(true);
        if (myProject->guiObjects[currentObject].keyFrames != "") {
            btnRecRel->Enable(true);
        }
    }
}

void MovementEditorFrame::findNextKeyFramePosition() {

   if (nextKeyFrameEnd == 0 && lastKeyFrameReached == false) {
        nextKeyFrameStart = 0;
        nextKeyFrameEnd = myProject->guiObjects[currentObject].keyFrames.find("/");
        if (nextKeyFrameEnd == 0) {
            nextKeyFrameEnd = myProject->guiObjects[currentObject].keyFrames.length();
        }
        return;
    }


    nextKeyFrameStart = nextKeyFrameEnd + 1;

    if (nextKeyFrameStart >= myProject->guiObjects[currentObject].keyFrames.length()) {
        lastKeyFrameReached = true;
        return;
    }
    nextKeyFrameEnd = myProject->guiObjects[currentObject].keyFrames.find("/", nextKeyFrameStart);
    if (nextKeyFrameEnd == -1) {
        nextKeyFrameEnd = myProject->guiObjects[currentObject].keyFrames.length();
    }


}

void MovementEditorFrame::getNextKeyFrameData() {
    wxString data = myProject->guiObjects[currentObject].keyFrames.substr(nextKeyFrameStart, (nextKeyFrameEnd - nextKeyFrameStart));

    wxStringTokenizer keyToken(data, ",");

    nextKeyFrameData.time = atof(keyToken.GetNextToken());
    nextKeyFrameData.pos.x = atof(keyToken.GetNextToken());
    nextKeyFrameData.pos.y = atof(keyToken.GetNextToken());
    nextKeyFrameData.pos.z = atof(keyToken.GetNextToken());


}

void MovementEditorFrame::setNextKeyFrameData(KeyFrameData data) {
    wxString oldStr = myProject->guiObjects[currentObject].keyFrames;
    wxString oldLastPart;
    if (nextKeyFrameEnd != -1) {
        oldLastPart = oldStr.substr(nextKeyFrameEnd);
    } else {
        oldLastPart = "";
    }

    wxString newPart = fToStr(data.time) + "," + fToStr(data.pos.x) + "," + fToStr(data.pos.y) + "," + fToStr(data.pos.z);

    myProject->guiObjects[currentObject].keyFrames =
        oldStr.substr(0, nextKeyFrameStart)
        + newPart
        + oldLastPart;

    nextKeyFrameEnd += -(nextKeyFrameEnd - nextKeyFrameStart) + newPart.length();

}


void MovementEditorFrame::applyMouseChangesToCurrentKeyFrame() {
    KeyFrameData kfd = nextKeyFrameData;
    c3DPoint rrs = recRelSmoother;

    rrs.x = (rrs.x - (startMousePoint.x - viewPort->lastMousePosition.x)) * smoothing;
    rrs.y = (rrs.y - (startMousePoint.y - viewPort->lastMousePosition.x)) * smoothing;
    rrs.z = (rrs.z - (startMousePoint.z - viewPort->lastMousePosition.x)) * smoothing;
    //todo: irgendeine magie!

    switch (viewPort->view) {
        case ViewPort::VIEW_TOP:
            kfd.pos.x -= startMousePoint.x - viewPort->lastMousePosition.x;
            kfd.pos.y -= startMousePoint.y - viewPort->lastMousePosition.y;
            break;
        case ViewPort::VIEW_FRONT:
            kfd.pos.x -= startMousePoint.x - viewPort->lastMousePosition.x;
            kfd.pos.z -= startMousePoint.z - viewPort->lastMousePosition.z;
            break;
        case ViewPort::VIEW_SIDE:
            kfd.pos.y -= startMousePoint.y - viewPort->lastMousePosition.y;
            kfd.pos.z -= startMousePoint.z - viewPort->lastMousePosition.z;
            break;
    }
    setNextKeyFrameData(kfd);
}
