#include "PropertiesEditor.h"

wxBEGIN_EVENT_TABLE(PropertiesEditor, wxVScrolledWindow)
    /* name field */
    EVT_TEXT(PropertiesEditor::startId, PropertiesEditor::OnNameEdit)

    /* text fields for object propertes */
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_FILE, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_POS1X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_POS1Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_POS1Z, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_VOLUME, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_FLAGS, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_KEYFRAMES, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_DELAY, PropertiesEditor::OnEditText)


    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_FILE, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_POS1X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_POS1Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_POS1Z, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_POS2X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_POS2Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_POS2Z, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_VOLUME, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_MINP, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_MAXP, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_MINS, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_MAXS, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_FLAGS, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_TTIME, PropertiesEditor::OnEditText)

    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_MIC_FILE, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_MIC_POS1X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_MIC_POS1Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_MIC_POS1Z, PropertiesEditor::OnEditText)

    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_FILE, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_ANGLE, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_DIST, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_POS1X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_POS1Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_POS1Z, PropertiesEditor::OnEditText)

    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_AMOUNT, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_POS1X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_POS1Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_POS1Z, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_POS2X, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_POS2Y, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_POS2Z, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_GROUP, PropertiesEditor::OnEditText)
    EVT_TEXT(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_MOD, PropertiesEditor::OnEditText)



    /* Buttons */
    EVT_BUTTON(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_FILE, PropertiesEditor::OnPropButton)
    EVT_BUTTON(PropertiesEditor::startId + PropertiesEditor::PROP_SPEAK_KEYFRAMES, PropertiesEditor::OnPropButton)
    EVT_BUTTON(PropertiesEditor::startId + PropertiesEditor::PROP_RNDSPKR_FILE, PropertiesEditor::OnPropButton)
    EVT_BUTTON(PropertiesEditor::startId + PropertiesEditor::PROP_MIC_FILE, PropertiesEditor::OnPropButton)
    EVT_BUTTON(PropertiesEditor::startId + PropertiesEditor::PROP_3DH_FILE, PropertiesEditor::OnPropButton)
    EVT_BUTTON(PropertiesEditor::startId + PropertiesEditor::PROP_WALL_MOD, PropertiesEditor::OnPropButton)

wxEND_EVENT_TABLE()

PropertiesEditor::PropertiesEditor(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                                   const wxSize &size, long style,
                                   const wxString &name)
                                   : wxVScrolledWindow(parent, id, pos, size, style, name)
{
    this->SetRowCount(1);
    createObjects();
    hideAllObjects();

}


PropertiesEditor::~PropertiesEditor()
{
}

wxCoord PropertiesEditor::OnGetRowHeight(size_t row) const {
    return wxCoord(4);
}

void PropertiesEditor::createObjects() {
    static bool ran = false;
    const int width = 150, height = 20;

    if (ran != false) {
        return;
    }

    for (int i = 0; i < PROP_COUNT; i++) {
            txtProp[i] = nullptr;
            editProp[i] = nullptr;
            btnProp[i] = nullptr;
    }

    txtObjName = new wxStaticText(this, wxID_ANY, "(ObjName)", wxPoint(0,0), wxSize(width, height));
    txtObjType = new wxStaticText(this, wxID_ANY, "(ObjType)", wxPoint(0, height), wxSize(width, height));
    txtNameTitle = new wxStaticText(this, wxID_ANY, "Name:", wxPoint(0, height * 2), wxSize(width, height));
    editName = new wxTextCtrl(this, startId, "", wxPoint(0, height * 3), wxSize(width, height));

    /* Speaker */
    txtProp[PROP_SPEAK_FILE] = new wxStaticText(this, wxID_ANY, "Input File:", wxPoint(0, height * 4), wxSize(width, height));
    editProp[PROP_SPEAK_FILE] = new wxTextCtrl(this, startId + PROP_SPEAK_FILE, wxEmptyString, wxPoint(0, height * 5), wxSize(width - height, height));
    btnProp[PROP_SPEAK_FILE] = new wxButton(this, startId + PROP_SPEAK_FILE, "...", wxPoint(width - height, height * 5), wxSize(height, height));
    txtProp[PROP_SPEAK_POS1X] = new wxStaticText(this, wxID_ANY, "X, Y, Z position:", wxPoint(0, height * 6), wxSize(width, height));
    editProp[PROP_SPEAK_POS1X] = new wxTextCtrl(this, startId + PROP_SPEAK_POS1X, wxEmptyString, wxPoint(0 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_SPEAK_POS1Y] = new wxTextCtrl(this, startId + PROP_SPEAK_POS1Y, wxEmptyString, wxPoint(1 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_SPEAK_POS1Z] = new wxTextCtrl(this, startId + PROP_SPEAK_POS1Z, wxEmptyString, wxPoint(2 * width / 3, height * 7), wxSize(width / 3, height));
    txtProp[PROP_SPEAK_VOLUME] = new wxStaticText(this, wxID_ANY, "Volume:", wxPoint(0, height * 8), wxSize(width, height));
    editProp[PROP_SPEAK_VOLUME] = new wxTextCtrl(this, startId + PROP_SPEAK_VOLUME, wxEmptyString, wxPoint(0, height * 9), wxSize(width, height));
    txtProp[PROP_SPEAK_KEYFRAMES] = new wxStaticText(this, wxID_ANY, "Keyframes:", wxPoint(0, height * 10), wxSize(width, height));
    editProp[PROP_SPEAK_KEYFRAMES] = new wxTextCtrl(this, startId + PROP_SPEAK_KEYFRAMES, wxEmptyString, wxPoint(0, height * 11), wxSize(width - height, height));
    btnProp[PROP_SPEAK_KEYFRAMES] = new wxButton(this, startId + PROP_SPEAK_KEYFRAMES, "...", wxPoint(width - height, height * 11), wxSize(height, height));
    txtProp[PROP_SPEAK_FLAGS] = new wxStaticText(this, wxID_ANY, "Flags:", wxPoint(0, height * 12), wxSize(width, height));
    editProp[PROP_SPEAK_FLAGS] = new wxTextCtrl(this, startId + PROP_SPEAK_FLAGS, wxEmptyString, wxPoint(0, height * 13), wxSize(width, height));
    txtProp[PROP_SPEAK_DELAY] = new wxStaticText(this, wxID_ANY, "Start Delay (s):", wxPoint(0, height * 14), wxSize(width, height));
    editProp[PROP_SPEAK_DELAY] = new wxTextCtrl(this, startId + PROP_SPEAK_DELAY, wxEmptyString, wxPoint(0, height * 15), wxSize(width, height));


    /* 3D Head */
    txtProp[PROP_3DH_FILE] = new wxStaticText(this, wxID_ANY, "Output File:", wxPoint(0, height * 4), wxSize(width, height));
    editProp[PROP_3DH_FILE] = new wxTextCtrl(this, startId + PROP_3DH_FILE, wxEmptyString, wxPoint(0, height * 5), wxSize(width - height, height));
    btnProp[PROP_3DH_FILE] = new wxButton(this, startId + PROP_3DH_FILE, "...", wxPoint(width - height, height * 5), wxSize(height, height));
    txtProp[PROP_3DH_POS1X] = new wxStaticText(this, wxID_ANY, "X, Y, Z position:", wxPoint(0, height * 6), wxSize(width, height));
    editProp[PROP_3DH_POS1X] = new wxTextCtrl(this, startId + PROP_3DH_POS1X, wxEmptyString, wxPoint(0 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_3DH_POS1Y] = new wxTextCtrl(this, startId + PROP_3DH_POS1Y, wxEmptyString, wxPoint(1 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_3DH_POS1Z] = new wxTextCtrl(this, startId + PROP_3DH_POS1Z, wxEmptyString, wxPoint(2 * width / 3, height * 7), wxSize(width / 3, height));
    txtProp[PROP_3DH_ANGLE] = new wxStaticText(this, wxID_ANY, "Head angle:", wxPoint(0, height * 8), wxSize(width, height));
    editProp[PROP_3DH_ANGLE] = new wxTextCtrl(this, startId + PROP_3DH_ANGLE, wxEmptyString, wxPoint(0, height * 9), wxSize(width, height));
    txtProp[PROP_3DH_DIST] = new wxStaticText(this, wxID_ANY, "Ear Distance:", wxPoint(0, height * 10), wxSize(width, height));
    editProp[PROP_3DH_DIST] = new wxTextCtrl(this, startId + PROP_3DH_DIST, wxEmptyString, wxPoint(0, height * 11), wxSize(width, height));

    /* Microphone */
    txtProp[PROP_MIC_FILE] = new wxStaticText(this, wxID_ANY, "Output File:", wxPoint(0, height * 4), wxSize(width, height));
    editProp[PROP_MIC_FILE] = new wxTextCtrl(this, startId + PROP_MIC_FILE, wxEmptyString, wxPoint(0, height * 5), wxSize(width - height, height));
    btnProp[PROP_MIC_FILE] = new wxButton(this, startId + PROP_MIC_FILE, "...", wxPoint(width - height, height * 5), wxSize(height, height));
    txtProp[PROP_MIC_POS1X] = new wxStaticText(this, wxID_ANY, "X, Y, Z position:", wxPoint(0, height * 6), wxSize(width, height));
    editProp[PROP_MIC_POS1X] = new wxTextCtrl(this, startId + PROP_MIC_POS1X, wxEmptyString, wxPoint(0 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_MIC_POS1Y] = new wxTextCtrl(this, startId + PROP_MIC_POS1Y, wxEmptyString, wxPoint(1 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_MIC_POS1Z] = new wxTextCtrl(this, startId + PROP_MIC_POS1Z, wxEmptyString, wxPoint(2 * width / 3, height * 7), wxSize(width / 3, height));

    /* Random Spawn Speaker */
    txtProp[PROP_RNDSPKR_FILE] = new wxStaticText(this, wxID_ANY, "Input file:", wxPoint(0, height * 4), wxSize(width, height));
    editProp[PROP_RNDSPKR_FILE] = new wxTextCtrl(this, startId + PROP_RNDSPKR_FILE, wxEmptyString, wxPoint(0, height * 5), wxSize(width - height, height));
    btnProp[PROP_RNDSPKR_FILE] = new wxButton(this, startId + PROP_RNDSPKR_FILE, "...", wxPoint(width - height, height * 5), wxSize(height, height));
    txtProp[PROP_RNDSPKR_POS1X] = new wxStaticText(this, wxID_ANY, "X, Y, Z start position:", wxPoint(0, height * 6), wxSize(width, height));
    editProp[PROP_RNDSPKR_POS1X] = new wxTextCtrl(this, startId + PROP_RNDSPKR_POS1X, wxEmptyString, wxPoint(0 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_RNDSPKR_POS1Y] = new wxTextCtrl(this, startId + PROP_RNDSPKR_POS1Y, wxEmptyString, wxPoint(1 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_RNDSPKR_POS1Z] = new wxTextCtrl(this, startId + PROP_RNDSPKR_POS1Z, wxEmptyString, wxPoint(2 * width / 3, height * 7), wxSize(width / 3, height));
    txtProp[PROP_RNDSPKR_POS2X] = new wxStaticText(this, wxID_ANY, "X, Y, Z end position:", wxPoint(0, height * 8), wxSize(width, height));
    editProp[PROP_RNDSPKR_POS2X] = new wxTextCtrl(this, startId + PROP_RNDSPKR_POS2X, wxEmptyString, wxPoint(0 * width / 3, height * 9), wxSize(width / 3, height));
    editProp[PROP_RNDSPKR_POS2Y] = new wxTextCtrl(this, startId + PROP_RNDSPKR_POS2Y, wxEmptyString, wxPoint(1 * width / 3, height * 9), wxSize(width / 3, height));
    editProp[PROP_RNDSPKR_POS2Z] = new wxTextCtrl(this, startId + PROP_RNDSPKR_POS2Z, wxEmptyString, wxPoint(2 * width / 3, height * 9), wxSize(width / 3, height));
    txtProp[PROP_RNDSPKR_VOLUME] = new wxStaticText(this, wxID_ANY, "Volume:", wxPoint(0, height * 10), wxSize(width, height));
    editProp[PROP_RNDSPKR_VOLUME] = new wxTextCtrl(this, startId + PROP_RNDSPKR_VOLUME, wxEmptyString, wxPoint(0, height * 11), wxSize(width, height));
    txtProp[PROP_RNDSPKR_TTIME] = new wxStaticText(this, wxID_ANY, "Total play time:", wxPoint(0, height * 12), wxSize(width, height));
    editProp[PROP_RNDSPKR_TTIME] = new wxTextCtrl(this, startId + PROP_RNDSPKR_TTIME, wxEmptyString, wxPoint(0, height * 13), wxSize(width, height));
    txtProp[PROP_RNDSPKR_MINP] = new wxStaticText(this, wxID_ANY, "Min pause, max pause:", wxPoint(0, height * 14), wxSize(width, height));
    editProp[PROP_RNDSPKR_MINP] = new wxTextCtrl(this, startId + PROP_RNDSPKR_MINP, wxEmptyString, wxPoint(0, height * 15), wxSize(width / 2, height));
    editProp[PROP_RNDSPKR_MAXP] = new wxTextCtrl(this, startId + PROP_RNDSPKR_MAXP, wxEmptyString, wxPoint(width / 2, height * 15), wxSize(width / 2, height));
    txtProp[PROP_RNDSPKR_MINS] = new wxStaticText(this, wxID_ANY, "Min speed, max speed:", wxPoint(0, height * 16), wxSize(width, height));
    editProp[PROP_RNDSPKR_MINS] = new wxTextCtrl(this, startId + PROP_RNDSPKR_MINS, wxEmptyString, wxPoint(0, height * 17), wxSize(width / 2, height));
    editProp[PROP_RNDSPKR_MAXS] = new wxTextCtrl(this, startId + PROP_RNDSPKR_MAXS, wxEmptyString, wxPoint(width / 2, height * 17), wxSize(width / 2, height));
    txtProp[PROP_RNDSPKR_FLAGS] = new wxStaticText(this, wxID_ANY, "Flags:", wxPoint(0, height * 18), wxSize(width, height));
    editProp[PROP_RNDSPKR_FLAGS] = new wxTextCtrl(this, startId + PROP_RNDSPKR_FLAGS, wxEmptyString, wxPoint(0, height * 19), wxSize(width, height));

    /*wall */
    txtProp[PROP_WALL_AMOUNT] = new wxStaticText(this, wxID_ANY, "Reflection amount:", wxPoint(0, height * 4), wxSize(width, height));
    editProp[PROP_WALL_AMOUNT] = new wxTextCtrl(this, startId + PROP_WALL_AMOUNT, wxEmptyString, wxPoint(0, height * 5), wxSize(width, height));
    txtProp[PROP_WALL_POS1X] = new wxStaticText(this, wxID_ANY, "X, Y, Z start position:", wxPoint(0, height * 6), wxSize(width, height));
    editProp[PROP_WALL_POS1X] = new wxTextCtrl(this, startId + PROP_WALL_POS1X, wxEmptyString, wxPoint(0 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_WALL_POS1Y] = new wxTextCtrl(this, startId + PROP_WALL_POS1Y, wxEmptyString, wxPoint(1 * width / 3, height * 7), wxSize(width / 3, height));
    editProp[PROP_WALL_POS1Z] = new wxTextCtrl(this, startId + PROP_WALL_POS1Z, wxEmptyString, wxPoint(2 * width / 3, height * 7), wxSize(width / 3, height));
    txtProp[PROP_WALL_POS2X] = new wxStaticText(this, wxID_ANY, "X, Y, Z end position:", wxPoint(0, height * 8), wxSize(width, height));
    editProp[PROP_WALL_POS2X] = new wxTextCtrl(this, startId + PROP_WALL_POS2X, wxEmptyString, wxPoint(0 * width / 3, height * 9), wxSize(width / 3, height));
    editProp[PROP_WALL_POS2Y] = new wxTextCtrl(this, startId + PROP_WALL_POS2Y, wxEmptyString, wxPoint(1 * width / 3, height * 9), wxSize(width / 3, height));
    editProp[PROP_WALL_POS2Z] = new wxTextCtrl(this, startId + PROP_WALL_POS2Z, wxEmptyString, wxPoint(2 * width / 3, height * 9), wxSize(width / 3, height));
    txtProp[PROP_WALL_GROUP] = new wxStaticText(this, wxID_ANY, "Wall Group:", wxPoint(0, height * 10), wxSize(width, height));
    editProp[PROP_WALL_GROUP] = new wxTextCtrl(this, startId + PROP_WALL_GROUP, wxEmptyString, wxPoint(0, height * 11), wxSize(width, height));
    txtProp[PROP_WALL_MOD] = new wxStaticText(this, wxID_ANY, "Wall Modifier:", wxPoint(0, height * 12), wxSize(width, height));
    editProp[PROP_WALL_MOD] = new wxTextCtrl(this, startId + PROP_WALL_MOD, wxEmptyString, wxPoint(0, height * 13), wxSize(width - height, height));
    btnProp[PROP_WALL_MOD] = new wxButton(this, startId + PROP_WALL_MOD, "...", wxPoint(width - height, height * 13), wxSize(height, height));

    hideAllObjects();
    ran = true;
}

void PropertiesEditor::hideAllObjects() {
    txtObjType->Hide();
    txtObjName->Hide();
    txtNameTitle->Hide();
    editName->Hide();
    for (int i = 0; i < PROP_COUNT; i++) {
        if (txtProp[i] != nullptr) {
            txtProp[i]->Hide();
        }
        if (editProp[i] != nullptr) {
            editProp[i]->Hide();
        }
        if (btnProp[i] != nullptr) {
            btnProp[i]->Hide();
        }
    }
}

void PropertiesEditor::showProperties() {
    const int height = 20;
    hideAllObjects();

    if (myProject != nullptr && currentObject != -1 && myProject->guiObjects[currentObject].name != "") {
        txtNameTitle->Show();
        editName->Show();
        txtObjType->Show();
        txtObjName->Show();
        editName->SetValue(myProject->guiObjects[currentObject].name);
        switch (myProject->guiObjects[currentObject].objType) {

            case BinauralProject::OBJ_SPEAKER:
                txtObjName->SetLabel(myProject->guiObjects[currentObject].name);
                txtObjType->SetLabel("(Speaker)");
                txtProp[PROP_SPEAK_FILE]->Show();
                editProp[PROP_SPEAK_FILE]->Show();
                btnProp[PROP_SPEAK_FILE]->Show();
                txtProp[PROP_SPEAK_POS1X]->Show();
                editProp[PROP_SPEAK_POS1X]->Show();
                editProp[PROP_SPEAK_POS1Y]->Show();
                editProp[PROP_SPEAK_POS1Z]->Show();
                txtProp[PROP_SPEAK_VOLUME]->Show();
                editProp[PROP_SPEAK_VOLUME]->Show();
                txtProp[PROP_SPEAK_KEYFRAMES]->Show();
                editProp[PROP_SPEAK_KEYFRAMES]->Show();
                btnProp[PROP_SPEAK_KEYFRAMES]->Show();
                txtProp[PROP_SPEAK_FLAGS]->Show();
                editProp[PROP_SPEAK_FLAGS]->Show();
                txtProp[PROP_SPEAK_DELAY]->Show();
                editProp[PROP_SPEAK_DELAY]->Show();

                this->SetRowCount(height * 16 / 4);
                break;

            case BinauralProject::OBJ_HEAD:
                txtObjName->SetLabel(myProject->guiObjects[currentObject].name);
                txtObjType->SetLabel("(Binaural Head)");
                txtProp[PROP_3DH_FILE]->Show();
                editProp[PROP_3DH_FILE]->Show();
                btnProp[PROP_3DH_FILE]->Show();
                txtProp[PROP_3DH_POS1X]->Show();
                editProp[PROP_3DH_POS1X]->Show();
                editProp[PROP_3DH_POS1Y]->Show();
                editProp[PROP_3DH_POS1Z]->Show();
                txtProp[PROP_3DH_ANGLE]->Show();
                editProp[PROP_3DH_ANGLE]->Show();
                txtProp[PROP_3DH_DIST]->Show();
                editProp[PROP_3DH_DIST]->Show();
                this->SetRowCount(height * 12 / 4);
                break;

            case BinauralProject::OBJ_MICROPHONE:
                txtObjName->SetLabel(myProject->guiObjects[currentObject].name);
                txtObjType->SetLabel("(Microphone)");
                txtProp[PROP_MIC_FILE]->Show();
                editProp[PROP_MIC_FILE]->Show();
                btnProp[PROP_MIC_FILE]->Show();
                txtProp[PROP_MIC_POS1X]->Show();
                editProp[PROP_MIC_POS1X]->Show();
                editProp[PROP_MIC_POS1Y]->Show();
                editProp[PROP_MIC_POS1Z]->Show();
                this->SetRowCount(height * 8 / 4);
                break;

            case BinauralProject::OBJ_RND_SPEAKER:
                txtObjName->SetLabel(myProject->guiObjects[currentObject].name);
                txtObjType->SetLabel("(Random Spawn Speaker)");
                txtProp[PROP_RNDSPKR_FILE]->Show();
                editProp[PROP_RNDSPKR_FILE]->Show();
                btnProp[PROP_RNDSPKR_FILE]->Show();
                txtProp[PROP_RNDSPKR_POS1X]->Show();
                editProp[PROP_RNDSPKR_POS1X]->Show();
                editProp[PROP_RNDSPKR_POS1Y]->Show();
                editProp[PROP_RNDSPKR_POS1Z]->Show();
                txtProp[PROP_RNDSPKR_POS2X]->Show();
                editProp[PROP_RNDSPKR_POS2X]->Show();
                editProp[PROP_RNDSPKR_POS2Y]->Show();
                editProp[PROP_RNDSPKR_POS2Z]->Show();
                txtProp[PROP_RNDSPKR_VOLUME]->Show();
                editProp[PROP_RNDSPKR_VOLUME]->Show();
                txtProp[PROP_RNDSPKR_MINP]->Show();
                editProp[PROP_RNDSPKR_MINP]->Show();
                editProp[PROP_RNDSPKR_MAXP]->Show();
                txtProp[PROP_RNDSPKR_MINS]->Show();
                editProp[PROP_RNDSPKR_MINS]->Show();
                editProp[PROP_RNDSPKR_MAXS]->Show();
                txtProp[PROP_RNDSPKR_FLAGS]->Show();
                editProp[PROP_RNDSPKR_FLAGS]->Show();
                txtProp[PROP_RNDSPKR_TTIME]->Show();
                editProp[PROP_RNDSPKR_TTIME]->Show();
                this->SetRowCount(height * 20 / 4);
                break;

            case BinauralProject::OBJ_WALL:
                txtObjName->SetLabel(myProject->guiObjects[currentObject].name);
                txtObjType->SetLabel("(Reflecting wall)");
                txtProp[PROP_WALL_AMOUNT]->Show();
                editProp[PROP_WALL_AMOUNT]->Show();
                txtProp[PROP_WALL_POS1X]->Show();
                editProp[PROP_WALL_POS1X]->Show();
                editProp[PROP_WALL_POS1Y]->Show();
                editProp[PROP_WALL_POS1Z]->Show();
                txtProp[PROP_WALL_POS2X]->Show();
                editProp[PROP_WALL_POS2X]->Show();
                editProp[PROP_WALL_POS2Y]->Show();
                editProp[PROP_WALL_POS2Z]->Show();
                txtProp[PROP_WALL_GROUP]->Show();
                editProp[PROP_WALL_GROUP]->Show();
                txtProp[PROP_WALL_MOD]->Show();
                editProp[PROP_WALL_MOD]->Show();
                btnProp[PROP_WALL_MOD]->Show();
                this->SetRowCount(height * 14 / 4);
                break;
        }
    }
}

void PropertiesEditor::fillProperties() {
    if (myProject != nullptr && currentObject != -1 && myProject->guiObjects[currentObject].name != "") {

        blockEvent = true;

        switch (myProject->guiObjects[currentObject].objType) {
            case BinauralProject::OBJ_SPEAKER:
                editProp[PROP_SPEAK_FILE]->SetValue(myProject->guiObjects[currentObject].file);
                editProp[PROP_SPEAK_POS1X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.x));
                editProp[PROP_SPEAK_POS1Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.y));
                editProp[PROP_SPEAK_POS1Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.z));
                editProp[PROP_SPEAK_VOLUME]->SetValue(fToStr( myProject->guiObjects[currentObject].volume));
                editProp[PROP_SPEAK_KEYFRAMES]->SetValue(myProject->guiObjects[currentObject].keyFrames);
                editProp[PROP_SPEAK_FLAGS]->SetValue(wxString::Format(wxT("%i"), myProject->guiObjects[currentObject].flags));
                editProp[PROP_SPEAK_DELAY]->SetValue(fToStr( myProject->guiObjects[currentObject].startDelay));
                break;

            case BinauralProject::OBJ_HEAD:
                editProp[PROP_3DH_FILE]->SetValue(myProject->guiObjects[currentObject].file);
                editProp[PROP_3DH_POS1X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.x));
                editProp[PROP_3DH_POS1Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.y));
                editProp[PROP_3DH_POS1Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.z));
                editProp[PROP_3DH_ANGLE]->SetValue(fToStr( myProject->guiObjects[currentObject].angle));
                editProp[PROP_3DH_DIST]->SetValue(fToStr( myProject->guiObjects[currentObject].earDistance));
                break;

            case BinauralProject::OBJ_MICROPHONE:
                editProp[PROP_MIC_FILE]->SetValue(myProject->guiObjects[currentObject].file);
                editProp[PROP_MIC_POS1X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.x));
                editProp[PROP_MIC_POS1Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.y));
                editProp[PROP_MIC_POS1Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.z));
                break;

            case BinauralProject::OBJ_RND_SPEAKER:
                editProp[PROP_RNDSPKR_FILE]->SetValue(myProject->guiObjects[currentObject].file);
                editProp[PROP_RNDSPKR_POS1X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.x));
                editProp[PROP_RNDSPKR_POS1Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.y));
                editProp[PROP_RNDSPKR_POS1Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.z));
                editProp[PROP_RNDSPKR_POS2X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos2.x));
                editProp[PROP_RNDSPKR_POS2Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos2.y));
                editProp[PROP_RNDSPKR_POS2Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos2.z));
                editProp[PROP_RNDSPKR_MINP]->SetValue(fToStr( myProject->guiObjects[currentObject].minPause));
                editProp[PROP_RNDSPKR_MAXP]->SetValue(fToStr( myProject->guiObjects[currentObject].maxPause));
                editProp[PROP_RNDSPKR_MINS]->SetValue(fToStr( myProject->guiObjects[currentObject].minSpeed));
                editProp[PROP_RNDSPKR_MAXS]->SetValue(fToStr( myProject->guiObjects[currentObject].maxSpeed));
                editProp[PROP_RNDSPKR_VOLUME]->SetValue(fToStr( myProject->guiObjects[currentObject].volume));
                editProp[PROP_RNDSPKR_FLAGS]->SetValue(wxString::Format(wxT("%i"), myProject->guiObjects[currentObject].flags));
                editProp[PROP_RNDSPKR_TTIME]->SetValue(fToStr( myProject->guiObjects[currentObject].totalTime));

                break;

            case BinauralProject::OBJ_WALL:
                editProp[PROP_WALL_AMOUNT]->SetValue(fToStr( myProject->guiObjects[currentObject].amount));
                editProp[PROP_WALL_POS1X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.x));
                editProp[PROP_WALL_POS1Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.y));
                editProp[PROP_WALL_POS1Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos.z));
                editProp[PROP_WALL_POS2X]->SetValue(fToStr( myProject->guiObjects[currentObject].pos2.x));
                editProp[PROP_WALL_POS2Y]->SetValue(fToStr( myProject->guiObjects[currentObject].pos2.y));
                editProp[PROP_WALL_POS2Z]->SetValue(fToStr( myProject->guiObjects[currentObject].pos2.z));
                editProp[PROP_WALL_GROUP]->SetValue(myProject->guiObjects[currentObject].wallGroup);
                editProp[PROP_WALL_MOD]->SetValue(myProject->guiObjects[currentObject].modifier);
                break;
        }

        blockEvent = false;
    }
}

void PropertiesEditor::setProject(BinauralProject *project)  {
    myProject = project;
    currentObject = -1;
}

void PropertiesEditor::setObject(int objectNr) {
    currentObject = objectNr;
    showProperties();
    fillProperties();
}

void PropertiesEditor::OnEditText(wxCommandEvent &evt) {
    c3DPoint oldPos1, oldPos2;
    bool connected;
    int wallType;

    if (blockEvent == false && myProject != nullptr && currentObject >= 0) {
        myProject->tempEvent.lastEvent = TempEvents::EVT_PE_PROP_CHANGE;
        myProject->tempEvent.objNr = currentObject;

        switch (evt.GetId() - startId) {
            case PROP_SPEAK_FILE:
                myProject->guiObjects[currentObject].file = editProp[PROP_SPEAK_FILE]->GetValue();
                break;
            case PROP_SPEAK_POS1X:
                myProject->guiObjects[currentObject].pos.x = atof(editProp[PROP_SPEAK_POS1X]->GetValue());
                break;
            case PROP_SPEAK_POS1Y:
                myProject->guiObjects[currentObject].pos.y = atof(editProp[PROP_SPEAK_POS1Y]->GetValue());
                break;
            case PROP_SPEAK_POS1Z:
                myProject->guiObjects[currentObject].pos.z = atof(editProp[PROP_SPEAK_POS1Z]->GetValue());
                break;
            case PROP_SPEAK_VOLUME:
                myProject->guiObjects[currentObject].volume = atof(editProp[PROP_SPEAK_VOLUME]->GetValue());
                break;
            case PROP_SPEAK_FLAGS:
                myProject->guiObjects[currentObject].flags = atoi(editProp[PROP_SPEAK_FLAGS]->GetValue());
                break;
            case PROP_SPEAK_KEYFRAMES:
                myProject->guiObjects[currentObject].keyFrames= editProp[PROP_SPEAK_KEYFRAMES]->GetValue();
                break;
            case PROP_SPEAK_DELAY:
                myProject->guiObjects[currentObject].startDelay= atof(editProp[PROP_SPEAK_DELAY]->GetValue());
                break;

            case PROP_3DH_FILE:
                myProject->guiObjects[currentObject].file = editProp[PROP_3DH_FILE]->GetValue();
                break;
            case PROP_3DH_POS1X:
                myProject->guiObjects[currentObject].pos.x = atof(editProp[PROP_3DH_POS1X]->GetValue());
                break;
            case PROP_3DH_POS1Y:
                myProject->guiObjects[currentObject].pos.y = atof(editProp[PROP_3DH_POS1Y]->GetValue());
                break;
            case PROP_3DH_POS1Z:
                myProject->guiObjects[currentObject].pos.z = atof(editProp[PROP_3DH_POS1Z]->GetValue());
                break;
            case PROP_3DH_ANGLE:
                myProject->guiObjects[currentObject].angle = atof(editProp[PROP_3DH_ANGLE]->GetValue());
                break;
            case PROP_3DH_DIST:
                myProject->guiObjects[currentObject].earDistance = atof(editProp[PROP_3DH_DIST]->GetValue());
                break;

            case PROP_MIC_FILE:
                myProject->guiObjects[currentObject].file = editProp[PROP_MIC_FILE]->GetValue();
                break;
            case PROP_MIC_POS1X:
                myProject->guiObjects[currentObject].pos.x = atof(editProp[PROP_MIC_POS1X]->GetValue());
                break;
            case PROP_MIC_POS1Y:
                myProject->guiObjects[currentObject].pos.y = atof(editProp[PROP_MIC_POS1Y]->GetValue());
                break;
            case PROP_MIC_POS1Z:
                myProject->guiObjects[currentObject].pos.z = atof(editProp[PROP_MIC_POS1Z]->GetValue());
                break;

            case PROP_RNDSPKR_FILE:
                myProject->guiObjects[currentObject].file = editProp[PROP_RNDSPKR_FILE]->GetValue();
                break;
            case PROP_RNDSPKR_POS1X:
                myProject->guiObjects[currentObject].pos.x = atof(editProp[PROP_RNDSPKR_POS1X]->GetValue());
                break;
            case PROP_RNDSPKR_POS1Y:
                myProject->guiObjects[currentObject].pos.y = atof(editProp[PROP_RNDSPKR_POS1Y]->GetValue());
                break;
            case PROP_RNDSPKR_POS1Z:
                myProject->guiObjects[currentObject].pos.z = atof(editProp[PROP_RNDSPKR_POS1Z]->GetValue());
                break;
            case PROP_RNDSPKR_POS2X:
                myProject->guiObjects[currentObject].pos2.x = atof(editProp[PROP_RNDSPKR_POS2X]->GetValue());
                break;
            case PROP_RNDSPKR_POS2Y:
                myProject->guiObjects[currentObject].pos2.y = atof(editProp[PROP_RNDSPKR_POS2Y]->GetValue());
                break;
            case PROP_RNDSPKR_POS2Z:
                myProject->guiObjects[currentObject].pos2.z = atof(editProp[PROP_RNDSPKR_POS2Z]->GetValue());
                break;
            case PROP_RNDSPKR_TTIME:
                myProject->guiObjects[currentObject].totalTime = atof(editProp[PROP_RNDSPKR_TTIME]->GetValue());
                break;
            case PROP_RNDSPKR_MINP:
                myProject->guiObjects[currentObject].minPause = atof(editProp[PROP_RNDSPKR_MINP]->GetValue());
                break;
            case PROP_RNDSPKR_MAXP:
                myProject->guiObjects[currentObject].maxPause = atof(editProp[PROP_RNDSPKR_MAXP]->GetValue());
                break;
            case PROP_RNDSPKR_MINS:
                myProject->guiObjects[currentObject].minSpeed = atof(editProp[PROP_RNDSPKR_MINS]->GetValue());
                break;
            case PROP_RNDSPKR_MAXS:
                myProject->guiObjects[currentObject].maxSpeed = atof(editProp[PROP_RNDSPKR_MAXS]->GetValue());
                break;
            case PROP_RNDSPKR_VOLUME:
                myProject->guiObjects[currentObject].volume = atof(editProp[PROP_RNDSPKR_VOLUME]->GetValue());
                break;
            case PROP_RNDSPKR_FLAGS:
                myProject->guiObjects[currentObject].flags = atoi(editProp[PROP_RNDSPKR_FLAGS]->GetValue());
                break;

            case PROP_WALL_AMOUNT:
                myProject->guiObjects[currentObject].amount = atof(editProp[PROP_WALL_AMOUNT]->GetValue());
                break;

            case PROP_WALL_POS1X:
                connected = myProject->isWallConnectedToGroup(currentObject);
                wallType = myProject->getWallType(currentObject);
                oldPos1 = myProject->guiObjects[currentObject].pos;
                oldPos2 = myProject->guiObjects[currentObject].pos2;
                myProject->guiObjects[currentObject].pos.x = atof(editProp[PROP_WALL_POS1X]->GetValue());
                if (connected == true) {
                    blockEvent = true;
                    if (wallType == BinauralProject::WT_SIDE) {
                        editProp[PROP_WALL_POS2X]->SetValue(wxString::Format(wxT("%f"), myProject->guiObjects[currentObject].pos.x));
                        myProject->guiObjects[currentObject].pos2.x = myProject->guiObjects[currentObject].pos.x;
                    }
                    myProject->movePointsInWallGroup(currentObject, oldPos1, oldPos2);
                    blockEvent = false;
                }
                break;

            case PROP_WALL_POS1Y:
                connected = myProject->isWallConnectedToGroup(currentObject);
                wallType = myProject->getWallType(currentObject);
                oldPos1 = myProject->guiObjects[currentObject].pos;
                oldPos2 = myProject->guiObjects[currentObject].pos2;
                myProject->guiObjects[currentObject].pos.y = atof(editProp[PROP_WALL_POS1Y]->GetValue());
                if (connected == true) {
                    blockEvent = true;
                    if (wallType == BinauralProject::WT_FRONT) {
                        editProp[PROP_WALL_POS2Y]->SetValue(wxString::Format(wxT("%f"), myProject->guiObjects[currentObject].pos.y));
                        myProject->guiObjects[currentObject].pos2.y = myProject->guiObjects[currentObject].pos.y;
                    }
                    myProject->movePointsInWallGroup(currentObject, oldPos1, oldPos2);
                    blockEvent = false;
                }
                break;

            case PROP_WALL_POS1Z:
                connected = myProject->isWallConnectedToGroup(currentObject);
                wallType = myProject->getWallType(currentObject);
                oldPos1 = myProject->guiObjects[currentObject].pos;
                oldPos2 = myProject->guiObjects[currentObject].pos2;
                myProject->guiObjects[currentObject].pos.z = atof(editProp[PROP_WALL_POS1Z]->GetValue());
                if (connected == true) {
                    blockEvent = true;
                    if (wallType == BinauralProject::WT_TOP) {
                        editProp[PROP_WALL_POS2Z]->SetValue(wxString::Format(wxT("%f"), myProject->guiObjects[currentObject].pos.z));
                        myProject->guiObjects[currentObject].pos2.z = myProject->guiObjects[currentObject].pos.z;
                    }
                    myProject->movePointsInWallGroup(currentObject, oldPos1, oldPos2);
                    blockEvent = false;
                }
                break;

            case PROP_WALL_POS2X:
                connected = myProject->isWallConnectedToGroup(currentObject);
                wallType = myProject->getWallType(currentObject);
                oldPos1 = myProject->guiObjects[currentObject].pos;
                oldPos2 = myProject->guiObjects[currentObject].pos2;
                myProject->guiObjects[currentObject].pos2.x = atof(editProp[PROP_WALL_POS2X]->GetValue());
                if (connected == true) {
                    blockEvent = true;
                    if (wallType == BinauralProject::WT_SIDE) {
                        editProp[PROP_WALL_POS1X]->SetValue(wxString::Format(wxT("%f"), myProject->guiObjects[currentObject].pos2.x));
                        myProject->guiObjects[currentObject].pos.x = myProject->guiObjects[currentObject].pos2.x;
                    }
                    myProject->movePointsInWallGroup(currentObject, oldPos1, oldPos2);
                    blockEvent = false;
                }
                break;

            case PROP_WALL_POS2Y:
                connected = myProject->isWallConnectedToGroup(currentObject);
                wallType = myProject->getWallType(currentObject);
                oldPos1 = myProject->guiObjects[currentObject].pos;
                oldPos2 = myProject->guiObjects[currentObject].pos2;
                myProject->guiObjects[currentObject].pos2.y = atof(editProp[PROP_WALL_POS2Y]->GetValue());
                if (connected == true) {
                    blockEvent = true;
                    if (wallType == BinauralProject::WT_FRONT) {
                        editProp[PROP_WALL_POS1Y]->SetValue(wxString::Format(wxT("%f"), myProject->guiObjects[currentObject].pos2.y));
                        myProject->guiObjects[currentObject].pos.y = myProject->guiObjects[currentObject].pos2.y;
                    }
                    myProject->movePointsInWallGroup(currentObject, oldPos1, oldPos2);
                    blockEvent = false;
                }
                break;

            case PROP_WALL_POS2Z:
                connected = myProject->isWallConnectedToGroup(currentObject);
                wallType = myProject->getWallType(currentObject);
                oldPos1 = myProject->guiObjects[currentObject].pos;
                oldPos2 = myProject->guiObjects[currentObject].pos2;
                myProject->guiObjects[currentObject].pos2.z = atof(editProp[PROP_WALL_POS2Z]->GetValue());
                if (connected == true) {
                    blockEvent = true;
                    if (wallType == BinauralProject::WT_TOP) {
                        editProp[PROP_WALL_POS1Z]->SetValue(wxString::Format(wxT("%f"), myProject->guiObjects[currentObject].pos2.z));
                        myProject->guiObjects[currentObject].pos.z = myProject->guiObjects[currentObject].pos2.z;
                    }
                    myProject->movePointsInWallGroup(currentObject, oldPos1, oldPos2);
                    blockEvent = false;
                }
                break;

            case PROP_WALL_GROUP:
                myProject->guiObjects[currentObject].wallGroup = editProp[PROP_WALL_GROUP]->GetValue();
                break;

            case PROP_WALL_MOD:
                myProject->guiObjects[currentObject].modifier = editProp[PROP_WALL_MOD]->GetValue();
                break;
        }
    }
    evt.Skip();
}

void PropertiesEditor::OnNameEdit(wxCommandEvent &evt) {
    if (blockEvent == false && myProject != nullptr && currentObject >= 0) {
        if (editName->GetValue() != "" && myProject->findObject(editName->GetValue()) == -1) {
            myProject->tempEvent.lastEvent = TempEvents::EVT_PE_RENAME;
            myProject->tempEvent.objNr = currentObject;
            if (myProject->tempEvent.lastName == "") {
                myProject->tempEvent.lastName = myProject->guiObjects[currentObject].name;
            }

            myProject->guiObjects[currentObject].name = editName->GetValue();
        }
    }
}

void PropertiesEditor::OnPropButton(wxCommandEvent &evt) {

    if (myProject != nullptr && currentObject >= 0) {
        switch (evt.GetId() - startId) {
            case PROP_SPEAK_FILE: {
                    wxFileDialog dlg(this, "Input Audio File", "", "", "32-bit RAW floating point|*.raw", wxFD_OPEN);
                    if (dlg.ShowModal() == wxID_OK) {
                        editProp[PROP_SPEAK_FILE]->SetValue(dlg.GetPath());
                    }
                    break;
                }

            case PROP_RNDSPKR_FILE: {
                    wxFileDialog dlg(this, "Input Audio File", "", "", "32-bit RAW floating point|*.raw", wxFD_OPEN);
                    if (dlg.ShowModal() == wxID_OK) {
                        editProp[PROP_RNDSPKR_FILE]->SetValue(dlg.GetPath());
                    }
                    break;
                }

            case PROP_MIC_FILE: {
                    wxFileDialog dlg(this, "Output Audio File", "", "", "32-bit RAW floating point|*.raw", wxFD_SAVE);
                    if (dlg.ShowModal() == wxID_OK) {
                        editProp[PROP_MIC_FILE]->SetValue(dlg.GetPath());
                    }
                    break;
                }

            case PROP_3DH_FILE: {
                    wxFileDialog dlg(this, "Output Audio File", "", "", "32-bit RAW floating point|*.raw", wxFD_SAVE);
                    if (dlg.ShowModal() == wxID_OK) {
                        editProp[PROP_MIC_FILE]->SetValue(dlg.GetPath());
                    }
                    break;
                }

            case PROP_WALL_MOD: {
                WallModEditorFrame *window = new WallModEditorFrame(0L, _("Wall Modifier"), myProject, currentObject);
                window->Show();
                break;
            }

            case PROP_SPEAK_KEYFRAMES: {
                MovementEditorFrame *window = new MovementEditorFrame(0L, _("Movement Editor"), myProject, currentObject);
                window->Show();
                break;
            }

        }
    }
}

void PropertiesEditor::updateObject() {
    fillProperties();
}
