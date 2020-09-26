#include "RenderSettingsFrame.h"

BEGIN_EVENT_TABLE(RenderSettingsFrame, wxFrame)
    EVT_BUTTON(RenderSettingsFrame::startID + 666, RenderSettingsFrame::OnButtonOKClick)
END_EVENT_TABLE()

RenderSettingsFrame::RenderSettingsFrame(wxFrame *frame, const wxString& title, BinauralProject *project)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(300, 575), 0x800 | 0x400 | 0x200 | 0x1000 | wxCAPTION | wxCLIP_CHILDREN) {



    myProject = project;


    for (int i = 0; i < NR_FIELDS; i++) {
        txtDescr[i] = new wxStaticText(this, wxID_ANY, "", wxPoint(5, i * 70), wxSize(this->GetSize().GetWidth() - 10, 20));
        txtHelp[i] = new wxStaticText(this, wxID_ANY, "", wxPoint(5, i * 70 + 20), wxSize(this->GetSize().GetWidth() - 10, 30));
        txtHelp[i]->SetFont(wxFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        editValue[i] = new wxTextCtrl(this, startID + i, "", wxPoint(5, i * 70 + 45), wxSize(this->GetSize().GetWidth() - 10, 20));
    }
    buttonOk = new wxButton(this, startID + 666, "OK", wxPoint(120, 510), wxSize(60, 20));

    txtDescr[0]->SetLabel("Project Samplerate");
    txtHelp[0]->SetLabel("Outputs sound files at this sample rate, all input files should have this\nrate. Default=44100");
    editValue[0]->SetValue(wxString::Format(wxT("%i"), myProject->settings.sampleRate));

    txtDescr[1]->SetLabel("Additional time (seconds):");
    txtHelp[1]->SetLabel("Adds additional time to the output files for reverb and sound delay.\nLarge rooms/distances need higher times. Default: 30s.");
    editValue[1]->SetValue(fToStr(myProject->settings.additionalTime));

    txtDescr[2]->SetLabel("Number of reflection bounces:");
    txtHelp[2]->SetLabel("The number times a reflection bounces off walls. Small rooms need\na higher number. Default: 50");
    editValue[2]->SetValue(wxString::Format(wxT("%i"), myProject->settings.nrBounces));

    txtDescr[3]->SetLabel("Maximum nr of reflection passes:");
    txtHelp[3]->SetLabel("Rendering quits after this number of reflection calculations\nis completed. Large rooms need a higher number. Default: 100");
    editValue[3]->SetValue(wxString::Format(wxT("%i"), myProject->settings.maxReflectionPasses));

    txtDescr[4]->SetLabel("Save every nr of reflection passes:");
    txtHelp[4]->SetLabel("When not 0, the output is being saved every number of reflection\npasses to preview the result. Default is 0.");
    editValue[4]->SetValue(wxString::Format(wxT("%i"), myProject->settings.saveMicsEveryNrOfPasses));

    txtDescr[5]->SetLabel("Random seed:");
    txtHelp[5]->SetLabel("Initiates random number generator. Same numbers will deliver\nthe same result. Change for different results.");
    editValue[5]->SetValue(wxString::Format(wxT("%i"), myProject->settings.randomSeed));

    txtDescr[6]->SetLabel("Air damping:");
    txtHelp[6]->SetLabel("Higher numbers will reduce high frequencies stronger over\nlonger distances. Default is 100.");
    editValue[6]->SetValue(fToStr(myProject->settings.airDamping));

}

RenderSettingsFrame::~RenderSettingsFrame()
{

}

void RenderSettingsFrame::OnButtonOKClick(wxCommandEvent &evt) {
    myProject->settings.sampleRate = atoi(editValue[0]->GetValue());
    myProject->settings.additionalTime = atof(editValue[1]->GetValue());
    myProject->settings.nrBounces = atoi(editValue[2]->GetValue());
    myProject->settings.maxReflectionPasses = atoi(editValue[3]->GetValue());
    myProject->settings.saveMicsEveryNrOfPasses = atoi(editValue[4]->GetValue());
    myProject->settings.randomSeed = atoi(editValue[5]->GetValue());
    myProject->settings.airDamping = atof(editValue[6]->GetValue());

    Destroy();
}
