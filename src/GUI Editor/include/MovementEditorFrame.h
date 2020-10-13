#ifndef MOVEMENTEDITORFRAME_H
#define MOVEMENTEDITORFRAME_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BinauralProject.h"
#include "ViewPort.h"


class MovementEditorFrame : public wxFrame
{
    public:
        static const int idSliderTime = 15000;
        static const int idBtnPlay = 15001;
        static const int idBtnTop = 15002;
        static const int idBtnFront = 15003;
        static const int idBtnSide = 15004;
        static const int idTxtSpeedfactor = 15005;
        static const int idTxtStartDelay = 15006;
        static const int idBtnRecAbs = 15007;
        static const int idBtnRecRel = 15008;
        static const int idTxtRecInterval = 15009;
        static const int idBtnSmooth = 15010;
        static const int idSliderSmooth = 15011;
        static const int idTxtTimeCurr = 15012;
        static const int idTxtTimeEnd = 15013;


        MovementEditorFrame(wxFrame *frame, const wxString& title, BinauralProject *project, int objectNr);
        ~MovementEditorFrame();

        void OnTimer(wxTimerEvent &evt);

        ViewPort *viewPort = nullptr;
        wxButton *btnTopView = nullptr;
        wxButton *btnFrontView = nullptr;
        wxButton *btnSideView = nullptr;

        wxButton *btnPlay = nullptr;

        wxButton *btnRecAbs = nullptr;
        wxButton *btnRecRel = nullptr;
        wxButton *btnSmooth = nullptr;


        //wxStaticText *txtTime = nullptr;
        wxTextCtrl *editTimeCurr = nullptr;
        wxTextCtrl *editTimeEnd = nullptr;
        wxSlider *sliderTime = nullptr;
        wxSlider *sliderSmooth = nullptr;
        wxTimer *timer = nullptr;
        wxTextCtrl *txtStartDelay = nullptr;
        wxTextCtrl *txtSpeedFactor = nullptr;
        wxTextCtrl *txtRecordInterval = nullptr;

        void OnButtonTopClick(wxCommandEvent &event);
        void OnButtonFrontClick(wxCommandEvent &event);
        void OnButtonSideClick(wxCommandEvent &event);
        void OnButtonPlayClick(wxCommandEvent &event);
        void OnSlideTime(wxCommandEvent &event);
        void OnTextChange(wxCommandEvent &event);
        void OnRecAbsClick(wxCommandEvent &event);
        void OnRecRelClick(wxCommandEvent &event);
        void OnKeyPress(wxKeyEvent &event);
        void OnButtonSmoothClick(wxCommandEvent &event);
        void OnSmoothSlide(wxCommandEvent &event);
        void stopRecording();
    protected:

    private:
        struct KeyFrameData {
            c3DPoint pos;
            double time;
        } nextKeyFrameData;

        void updateEnabledDisabled();
        void findNextKeyFramePosition();
        void getNextKeyFrameData();
        void setNextKeyFrameData(KeyFrameData data);
        void applyMouseChangesToCurrentKeyFrame();

        BinauralProject *myProject;
        double timeStamp = 0.0;
        double maxTime = 60.0;
        double speedFactor = 1.0;
        double recordInterval = 0.5;
        double lastKeyframeRecorded = 0.0;
        double smoothing = 1.0;
        c3DPoint recRelSmoother;
        int nextKeyFrameStart = 0;
        int nextKeyFrameEnd = 0;
        c3DPoint startMousePoint;
        bool lastKeyFrameReached = false;
        int recordCountDown = 0;
        wxString origKeyFrames;
        wxString recordedKeyFrames;


        uint64_t startTimeMS;
        int currentObject;
        bool playing = false;
        bool recording = false;
        bool noEvents = false;
        bool recordingAbsolute = true;

       DECLARE_EVENT_TABLE()
};

#endif // MOVEMENTEDITORFRAME_H
