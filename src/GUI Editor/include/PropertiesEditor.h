#ifndef PROPERTIESEDITOR_H
#define PROPERTIESEDITOR_H

#include <wx/wx.h>
#include <wx/vscroll.h>
#include <BinauralProject.h>

class PropertiesEditor : public wxVScrolledWindow
{
    public:
        PropertiesEditor 	(wxWindow *parent,
                            wxWindowID id = wxID_ANY,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = 0,
                            const wxString & name = wxPanelNameStr);

        ~PropertiesEditor();

        static const int PROP_COUNT = 41;
        wxStaticText *txtObjName = nullptr, *txtObjType = nullptr, *txtNameTitle = nullptr;
        wxStaticText *txtProp[PROP_COUNT];
        wxTextCtrl *editProp[PROP_COUNT];
        wxButton *btnProp[PROP_COUNT];
        wxTextCtrl *editName = nullptr;

        void setProject(BinauralProject *project);
        void setObject(int objectNr);
        void updateObject();

    protected:

    private:
            static const int PROP_SPEAK_FILE = 1;
            static const int PROP_SPEAK_POS1X = 2;
            static const int PROP_SPEAK_POS1Y = 3;
            static const int PROP_SPEAK_POS1Z = 4;
            static const int PROP_SPEAK_VOLUME = 5;
            static const int PROP_SPEAK_KEYFRAMES = 6;
            static const int PROP_SPEAK_FLAGS = 7;

            static const int PROP_3DH_FILE = 8;
            static const int PROP_3DH_POS1X = 9;
            static const int PROP_3DH_POS1Y = 10;
            static const int PROP_3DH_POS1Z = 11;
            static const int PROP_3DH_ANGLE = 12;
            static const int PROP_3DH_DIST = 13;

            static const int PROP_MIC_FILE = 14;
            static const int PROP_MIC_POS1X = 15;
            static const int PROP_MIC_POS1Y = 16;
            static const int PROP_MIC_POS1Z = 17;

            static const int PROP_RNDSPKR_FILE = 18;
            static const int PROP_RNDSPKR_POS1X = 19;
            static const int PROP_RNDSPKR_POS1Y = 20;
            static const int PROP_RNDSPKR_POS1Z = 21;
            static const int PROP_RNDSPKR_POS2X = 22;
            static const int PROP_RNDSPKR_POS2Y = 23;
            static const int PROP_RNDSPKR_POS2Z = 24;
            static const int PROP_RNDSPKR_VOLUME = 25;
            static const int PROP_RNDSPKR_TTIME = 26;
            static const int PROP_RNDSPKR_MINP = 27;
            static const int PROP_RNDSPKR_MAXP = 28;
            static const int PROP_RNDSPKR_MINS = 29;
            static const int PROP_RNDSPKR_MAXS = 30;
            static const int PROP_RNDSPKR_FLAGS = 31;

            static const int PROP_WALL_POS1X = 32;
            static const int PROP_WALL_POS1Y = 33;
            static const int PROP_WALL_POS1Z = 34;
            static const int PROP_WALL_POS2X = 35;
            static const int PROP_WALL_POS2Y = 36;
            static const int PROP_WALL_POS2Z = 37;
            static const int PROP_WALL_AMOUNT = 38;
            static const int PROP_WALL_GROUP = 39;
            static const int PROP_WALL_MOD = 40;




            virtual wxCoord OnGetRowHeight(size_t row) const;
            void createObjects();
            void hideAllObjects();
            void showProperties();
            void fillProperties();
            void OnEditText(wxCommandEvent &evt);
            void OnNameEdit(wxCommandEvent &evt);
            void OnPropButton(wxCommandEvent &evt);

            static const int startId = 12000;

            BinauralProject *myProject = nullptr;
            int currentObject = -1;
            bool blockEvent = false;

            wxDECLARE_EVENT_TABLE();
};

#endif // PROPERTIESEDITOR_H
