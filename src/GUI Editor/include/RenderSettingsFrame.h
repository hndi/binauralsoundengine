#ifndef RENDERSETTINGSFRAME_H
#define RENDERSETTINGSFRAME_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BinauralProject.h"
#include "guihelpers.h"


class RenderSettingsFrame : public wxFrame
{
    public:
        RenderSettingsFrame(wxFrame *frame, const wxString& title, BinauralProject *myProject);
        virtual ~RenderSettingsFrame();

        static const int NR_FIELDS = 7;
        wxStaticText *txtDescr[NR_FIELDS];
        wxStaticText *txtHelp[NR_FIELDS];
        wxTextCtrl *editValue[NR_FIELDS];
        wxButton *buttonOk = nullptr;
    protected:

    private:
        BinauralProject *myProject = nullptr;
        static const int startID = 5100;

        void OnButtonOKClick(wxCommandEvent &evt);
        DECLARE_EVENT_TABLE()

};

#endif // RENDERSETTINGSFRAME_H
