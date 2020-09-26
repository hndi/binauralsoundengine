/***************************************************************
 * Name:      BinauralFilterGUIMain.h
 * Purpose:   Defines Application Frame
 * Author:    hndi ()
 * Created:   2020-08-04
 * Copyright: hndi ()
 * License:
 **************************************************************/

#ifndef BINAURALFILTERGUIMAIN_H
#define BINAURALFILTERGUIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "BinauralFilterGUIApp.h"
#include "ViewPort.h"
#include <wx/listctrl.h>
#include "BinauralProject.h"
#include "PropertiesEditor.h"
#include "RenderSettingsFrame.h"

class BinauralFilterGUIFrame: public wxFrame
{
    public:
        BinauralFilterGUIFrame(wxFrame *frame, const wxString& title);
        ~BinauralFilterGUIFrame();

        void updateObjList();
        void updateObjProperties(int objNr);

        wxToolBar *toolBar = nullptr;
        wxButton *toolBarButtonTop = nullptr;
        wxButton *toolBarButtonFront = nullptr;
        wxButton *toolBarButtonSide = nullptr;

        wxButton *toolBarRender = nullptr;

        wxButton *toolBarAddSpeaker = nullptr;
        wxButton *toolBarAddHead = nullptr;
        wxButton *toolBarAddMicrophone = nullptr;
        wxButton *toolBarAddRndSpeaker = nullptr;
        wxButton *toolBarAddWall = nullptr;
        wxButton *toolBarAddRoom = nullptr;

        ViewPort *viewPort = nullptr;
        wxStaticBox *boxObjects = nullptr;
        wxStaticBox *boxProp = nullptr;
        PropertiesEditor *propEdit = nullptr;

        wxListBox *listObj = nullptr;

        wxTimer *timer = nullptr;

        BinauralProject *project = nullptr;

    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuNew = 2000,
            idMenuAbout
        };

        static const int idMenuOpen = 6780;
        static const int idMenuSave = 6781;
        static const int idMenuSaveAs = 6782;
        static const int idMenuSceneSettings = 6783;

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnMenuNew(wxCommandEvent& event);
        void OnMenuOpen(wxCommandEvent& event);
        void OnMenuSave(wxCommandEvent& event);
        void OnMenuSaveAs(wxCommandEvent& event);
        void OnMenuSceneSettings(wxCommandEvent &event);
        void OnWindowResize(wxSizeEvent &event);

        void OnViewTopClick(wxCommandEvent &event);
        void OnViewFrontClick(wxCommandEvent &event);
        void OnViewSideClick(wxCommandEvent &event);

        void OnRenderClick(wxCommandEvent &event);
        void OnAddSpeakerClick(wxCommandEvent &event);
        void OnAddHeadClick(wxCommandEvent &event);
        void OnAddMicrophoneClick(wxCommandEvent &event);
        void OnAddRndSpeakerClick(wxCommandEvent &event);
        void OnAddRoomClick(wxCommandEvent &event);
        void OnAddWallClick(wxCommandEvent &event);

        void OnTimer(wxTimerEvent &evt);

        void OnViewPortMouseMove(int x, int y);
        void OnObjListClick(wxCommandEvent &evt);
        DECLARE_EVENT_TABLE()

        wxString currentFileName;
};


#endif // BINAURALFILTERGUIMAIN_H
