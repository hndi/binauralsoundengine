#include "WallModEditor.h"

BEGIN_EVENT_TABLE(WallModEditorFrame, wxFrame)
    EVT_BUTTON(WallModEditorFrame::btnOkId, WallModEditorFrame::OnOkClick)
    EVT_TEXT(WallModEditorFrame::txtAmpId, WallModEditorFrame::OnTextChange)
    EVT_TEXT(WallModEditorFrame::txtWaveId, WallModEditorFrame::OnTextChange)
    EVT_TEXT(WallModEditorFrame::txtShiftId, WallModEditorFrame::OnTextChange)
    EVT_COMBOBOX(WallModEditorFrame::cmbTypeId, WallModEditorFrame::OnTextChange)

END_EVENT_TABLE()

WallModEditorFrame::WallModEditorFrame(wxFrame *frame, const wxString& title, BinauralProject *project, int objNr)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(250, 285), 0x800 | 0x400 | 0x200 | 0x1000 | wxCAPTION | wxCLIP_CHILDREN) {

    wxArrayString arrChoices;
    arrChoices.Add("None");
    arrChoices.Add("Sine");
    arrChoices.Add("Square");
    arrChoices.Add("Triangle");
    wxSize size = wxSize(this->GetSize().GetWidth() - 10, 20);
    new wxStaticText(this, wxID_ANY, "Modfier Type:", wxPoint(5, 0 * 50), size);
    comboType = new wxComboBox(this, cmbTypeId, "None", wxPoint(5, 0 * 50 + 20), size, arrChoices);
    new wxStaticText(this, wxID_ANY, "Wave length (m):", wxPoint(5, 1 * 50), size);
    editLength = new wxTextCtrl(this, txtWaveId, "", wxPoint(5, 1 * 50 + 20), size);
    new wxStaticText(this, wxID_ANY, "Amplitude (m):", wxPoint(5, 2 * 50), size);
    editAmp = new wxTextCtrl(this, txtAmpId, "", wxPoint(5, 2 * 50 + 20), size);
    new wxStaticText(this, wxID_ANY, "Shift (m):", wxPoint(5, 3 * 50), size);
    editShift = new wxTextCtrl(this, txtShiftId, "", wxPoint(5, 3 * 50 + 20), size);

    buttonOk = new wxButton(this, btnOkId, "OK", wxPoint(90, 4 * 50 + 20), wxSize(60, 20));

    myProject = project;
    currentObj = objNr;

    if (isObjectValid() == true) {
        origString = myProject->guiObjects[currentObj].modifier;
    }

    FillValues();
}

void WallModEditorFrame::FillValues() {
    if (isObjectValid() == true) {
        int usedMod = 0;
        double waveLength = 5.0, amplitude = 1.5, shift = 0.0;
        wxStringTokenizer token(myProject->guiObjects[currentObj].modifier, ",");

        if (token.HasMoreTokens()) {
            wxString nt = token.GetNextToken();
            if (nt == "sine") usedMod = 1;
            if (nt == "square") usedMod = 2;
            if (nt == "triangle") usedMod = 3;
        }
        if (token.HasMoreTokens())
            waveLength = atof(token.GetNextToken());
        if (token.HasMoreTokens())
            amplitude = atof(token.GetNextToken());
        if (token.HasMoreTokens())
            shift = atof(token.GetNextToken());

      blockEvents = true;
      editAmp->SetValue(fToStr(amplitude));
      editLength->SetValue(fToStr(waveLength));
      editShift->SetValue(fToStr(shift));

      comboType->SetSelection(usedMod);
      blockEvents = false;

    }
}

bool WallModEditorFrame::isObjectValid() {
   if (myProject != nullptr && currentObj >= 0 && currentObj < BinauralProject::MAX_OBJECTS
        && myProject->guiObjects[currentObj].objType == BinauralProject::OBJ_WALL) {

        return true;
    }

    return false;
}

WallModEditorFrame::~WallModEditorFrame() {
    if (clickedOk == false) {
        myProject->guiObjects[currentObj].modifier = origString;
        myProject->tempEvent.lastEvent = TempEvents::EVT_WM_CHANGED;
        myProject->tempEvent.objNr = currentObj;
    }
}

wxString WallModEditorFrame::generateString() {
    wxString ret;
    switch (comboType->GetSelection()) {
        case 0:
            return "";
        case 1:
            ret = "sine,";
            break;
        case 2:
            ret = "square,";
            break;
        case 3:
            ret = "triangle,";
            break;
    }
    ret += fToStr(atof(editLength->GetValue())) + ",";
    ret += fToStr(atof(editAmp->GetValue())) + ",";
    ret += fToStr(atof(editShift->GetValue()));

    return ret;
}

void WallModEditorFrame::OnOkClick(wxCommandEvent &evt) {
    clickedOk = true;
    Destroy();
    evt.Skip();
}

void WallModEditorFrame::OnTextChange(wxCommandEvent &evt) {
    if (isObjectValid() == true && blockEvents == false) {
        myProject->tempEvent.lastEvent = TempEvents::EVT_WM_CHANGED;
        myProject->tempEvent.objNr = currentObj;
        myProject->guiObjects[currentObj].modifier = generateString();
    }

    evt.Skip();
}
