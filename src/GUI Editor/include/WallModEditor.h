#ifndef WALLMODEDITOR_H
#define WALLMODEDITOR_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <BinauralProject.h>
#include <guihelpers.h>
#include <wx/tokenzr.h>


class WallModEditorFrame : public wxFrame
{
    public:
        static const int btnOkId = 456;
        static const int txtWaveId = 457;
        static const int txtAmpId = 458;
        static const int txtShiftId = 459;
        static const int cmbTypeId = 460;

        WallModEditorFrame(wxFrame *frame, const wxString& title, BinauralProject *myProject, int objNr);
        virtual ~WallModEditorFrame();

        void OnOkClick(wxCommandEvent &evt);
        void OnTextChange(wxCommandEvent &evt);

        wxComboBox *comboType = nullptr;
        wxTextCtrl *editLength = nullptr;
        wxTextCtrl *editAmp = nullptr;
        wxTextCtrl *editShift = nullptr;
        wxButton *buttonOk = nullptr;

        BinauralProject *myProject = nullptr;
        int currentObj = -1;

    protected:

    private:


        void FillValues();
        bool isObjectValid();
        wxString generateString();

        bool clickedOk = false;
        bool blockEvents = false;
        wxString origString;

        DECLARE_EVENT_TABLE()

};

#endif // WALLMODEDITOR_H
