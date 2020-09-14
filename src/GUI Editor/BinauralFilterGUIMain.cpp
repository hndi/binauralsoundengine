/***************************************************************
 * Name:      BinauralFilterGUIMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    hndi ()
 * Created:   2020-08-04
 * Copyright: hndi ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "BinauralFilterGUIMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(BinauralFilterGUIFrame, wxFrame)

    EVT_CLOSE(BinauralFilterGUIFrame::OnClose)
    EVT_MENU(idMenuQuit, BinauralFilterGUIFrame::OnQuit)
    EVT_MENU(idMenuAbout, BinauralFilterGUIFrame::OnAbout)
    EVT_MENU(idMenuSave, BinauralFilterGUIFrame::OnMenuSave)
    EVT_MENU(idMenuSaveAs, BinauralFilterGUIFrame::OnMenuSaveAs)
    EVT_MENU(idMenuSceneSettings, BinauralFilterGUIFrame::OnMenuSceneSettings)
    EVT_MENU(idMenuOpen, BinauralFilterGUIFrame::OnMenuOpen)

    EVT_SIZE(BinauralFilterGUIFrame::OnWindowResize)
    EVT_LISTBOX(11000, BinauralFilterGUIFrame::OnObjListClick)

    EVT_BUTTON(10001, BinauralFilterGUIFrame::OnAddSpeakerClick)
    EVT_BUTTON(10002, BinauralFilterGUIFrame::OnAddHeadClick)
    EVT_BUTTON(10003, BinauralFilterGUIFrame::OnAddMicrophoneClick)
    EVT_BUTTON(10004, BinauralFilterGUIFrame::OnAddRndSpeakerClick)
    EVT_BUTTON(10005, BinauralFilterGUIFrame::OnAddRoomClick)
    EVT_BUTTON(10006, BinauralFilterGUIFrame::OnAddWallClick)
    EVT_BUTTON(10007, BinauralFilterGUIFrame::OnRenderClick)

    EVT_TIMER(1234, BinauralFilterGUIFrame::OnTimer)

END_EVENT_TABLE()

BinauralFilterGUIFrame::BinauralFilterGUIFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuNew, _("&New\tCtrl-N"), _("New blank scene"));
    fileMenu->Append(idMenuOpen, _("&Open...\tCtrl-O"), _("Open a binaural scene file"));
    fileMenu->Append(idMenuSave, _("&Save\tCtrl-S"), _("Save scene to file"));
    fileMenu->Append(idMenuSaveAs, _("&Save as...\tCtrl-Shift-S"), _("Save scene to file"));
    fileMenu->Append(idMenuSceneSettings, _("Scene &Render Settings...\tCtrl-P"), _("Edit rendering properties for your 3D scene"));

    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);

#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Position: x: 0, y: 0, z: 0"),0);
    //SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR
    project = new BinauralProject;

    /* Default scene */
    int x= project->addSpeaker();
    project->guiObjects[x].pos.setPoint(-1, -1, 1.6);
    x = project->addHead();
    project->guiObjects[x].pos.setPoint(0, 0, 1.6);

    toolBar = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
    toolBarButtonTop = new wxButton(toolBar, wxID_ANY, "Top", wxDefaultPosition , wxSize(64, 32));
    toolBarButtonFront = new wxButton(toolBar, wxID_ANY, "Front", wxDefaultPosition , wxSize(64, 32));
    toolBarButtonSide = new wxButton(toolBar, wxID_ANY, "Side", wxDefaultPosition , wxSize(64, 32));
    toolBar->AddControl(new wxStaticText(toolBar, wxID_ANY, "View: "));
    toolBar->AddControl(toolBarButtonTop);
    toolBar->AddControl(toolBarButtonFront);
    toolBar->AddControl(toolBarButtonSide);
    toolBar->AddSeparator();
    toolBarRender = new wxButton(toolBar, 10007, "Render!", wxDefaultPosition);
    toolBar->AddControl(toolBarRender);
    toolBar->AddSeparator();
    toolBar->AddControl(new wxStaticText(toolBar, wxID_ANY, "Add: "));

    toolBarAddSpeaker = new wxButton(toolBar, 10001, "Speaker", wxDefaultPosition);
    toolBarAddHead = new wxButton(toolBar, 10002, "3D Head", wxDefaultPosition);
    toolBarAddMicrophone = new wxButton(toolBar, 10003, "Mic", wxDefaultPosition);
    toolBarAddRndSpeaker = new wxButton(toolBar, 10004, "RndSpeaker", wxDefaultPosition);
    toolBarAddRoom = new wxButton(toolBar, 10005, "Room", wxDefaultPosition);
    toolBarAddWall = new wxButton(toolBar, 10006, "Wall", wxDefaultPosition);
    toolBar->AddControl(toolBarAddSpeaker);
    toolBar->AddControl(toolBarAddHead);
    toolBar->AddControl(toolBarAddMicrophone);
    toolBar->AddControl(toolBarAddRndSpeaker);
    toolBar->AddControl(toolBarAddRoom);
    toolBar->AddControl(toolBarAddWall);
    toolBar->Realize();
    toolBar->Show();

    viewPort = new ViewPort(this, wxPoint(5, 0), wxSize(825, 505));
    viewPort->setProject(project);
    viewPort->setZoom(3);
    viewPort->setCenterOfView(c3DPoint(0, -1, 1.6));
    boxObjects = new wxStaticBox(this, wxID_ANY, "Objects", wxPoint(839, 0), wxSize(180, 200));
    boxProp = new wxStaticBox(this, wxID_ANY, "Properties", wxPoint(839, 220), wxSize(180, 285));

#ifdef __WXMSW__
    listObj = new wxListBox(boxObjects, 11000, wxPoint(5, 20), wxSize(160, 160));
    propEdit = new PropertiesEditor(boxProp, wxID_ANY, wxPoint(5, 20), wxSize(160, 160));
#else
    listObj = new wxListBox(boxObjects, 11000, wxPoint(10, 0), wxSize(160, 160));
    propEdit = new PropertiesEditor(boxProp, wxID_ANY, wxPoint(10, 0), wxSize(160, 260));
#endif //__WXMSW__

    propEdit->setProject(project);

    timer = new wxTimer(this, 1234);
    timer->Start(100);
    updateObjList();
}


BinauralFilterGUIFrame::~BinauralFilterGUIFrame()
{
}

void BinauralFilterGUIFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void BinauralFilterGUIFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void BinauralFilterGUIFrame::OnAbout(wxCommandEvent &event)
{
    //wxString msg = wxbuildinfo(long_f);
    //wxMessageBox(msg, _("Welcome to..."));
}

void BinauralFilterGUIFrame::OnMenuNew(wxCommandEvent& event) {
}

void BinauralFilterGUIFrame::OnMenuOpen(wxCommandEvent& event) {
    wxFileDialog dlg(this, "Open binaural audio scene", "", "", "Binaural 3d Scene (*.b3s)|*.b3s|Text file(*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK) {
        currentFileName = dlg.GetPath();
        project->loadFromFile(currentFileName);
        viewPort->Refresh();
        updateObjList();
    }

    event.Skip();

}

void BinauralFilterGUIFrame::OnMenuSave(wxCommandEvent& event) {
    if (currentFileName == "") {
        wxFileDialog dlg(this, "Save binaural audio scene", "", "Unnamed.b3s", "Binaural 3D Scene|*.b3s", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (dlg.ShowModal() == wxID_OK) {
            currentFileName = dlg.GetPath();
        }
    }
    if (currentFileName != "") {
        project->saveToFile(currentFileName);
    }
    event.Skip();
}

void BinauralFilterGUIFrame::OnMenuSaveAs(wxCommandEvent& event) {
    wxString tmp;
    if (currentFileName == "") {
        tmp = "Unnamed.b3s";
    }
    wxFileDialog dlg(this, "Save binaural audio scene", "", tmp, "Binaural 3D Scene|*.b3s", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dlg.ShowModal() == wxID_OK) {
        currentFileName = dlg.GetPath();
    }

    if (currentFileName != "") {
        project->saveToFile(currentFileName);
    }
    event.Skip();
}


void BinauralFilterGUIFrame::OnWindowResize(wxSizeEvent &event) {

    //viewPort->SetSize(viewPort->GetPosition().x, viewPort->GetPosition().y,
    //                   this->GetSize().GetX() - 100, this->GetSize().GetY() - 100);

}

void BinauralFilterGUIFrame::OnAddSpeakerClick(wxCommandEvent &event) {
    int x = project->addSpeaker();
    if (x >= 0) {
        project->guiObjects[x].pos = viewPort->getCenterOfView();
        viewPort->Refresh();
    }
    updateObjList();
    event.Skip();
}

void BinauralFilterGUIFrame::OnAddHeadClick(wxCommandEvent &event) {
    static double angle = 0;
    int x = project->addHead();
    if (x >= 0) {
        project->guiObjects[x].pos = viewPort->getCenterOfView();
        project->guiObjects[x].angle = angle += 15;
        viewPort->Refresh();
    }
    updateObjList();
    event.Skip();
}

void BinauralFilterGUIFrame::OnAddMicrophoneClick(wxCommandEvent &event){
    int x = project->addMicrophone();
    if (x >= 0) {
        project->guiObjects[x].pos = viewPort->getCenterOfView();
        viewPort->Refresh();
    }
    updateObjList();
    event.Skip();
}


void BinauralFilterGUIFrame::OnAddRndSpeakerClick(wxCommandEvent &event){
    int x = project->addRndSpeaker();
    c3DPoint pos2;
    if (x >= 0) {
        project->guiObjects[x].pos = viewPort->getCenterOfView();
        pos2 = viewPort->getCenterOfView();
        pos2.x += 1;
        pos2.y += -1;
        pos2.z += 1;
        project->guiObjects[x].pos2 = pos2;
        viewPort->Refresh();
    }
    updateObjList();
    event.Skip();
}

void BinauralFilterGUIFrame::OnAddRoomClick(wxCommandEvent &event) {
    int x;
    c3DPoint pos1, pos2;
    pos1.x = viewPort->getCenterOfView().x - 1.5;
    pos1.y = viewPort->getCenterOfView().y - 2.0;
    pos1.z = 0;

    pos2.x = viewPort->getCenterOfView().x + 1.5;
    pos2.y = viewPort->getCenterOfView().y + 1.5;
    pos2.z = 2.5;

    x = project->addWall();
    project->guiObjects[x].pos = pos1;
    project->guiObjects[x].pos2 = pos2;
    project->guiObjects[x].pos2.z = pos1.z;
    project->setName(x, "Floor");
    project->guiObjects[x].wallGroup = "Room";

    x = project->addWall();
    project->guiObjects[x].pos = pos1;
    project->guiObjects[x].pos2 = pos2;
    project->guiObjects[x].pos.z = pos2.z;
    project->setName(x, "Ceiling");
    project->guiObjects[x].wallGroup = "Room";

     x = project->addWall();
    project->guiObjects[x].pos = pos1;
    project->guiObjects[x].pos2 = pos2;
    project->guiObjects[x].pos2.y = pos1.y;
    project->setName(x, "Front Wall");
    project->guiObjects[x].wallGroup = "Room";

    x = project->addWall();
    project->guiObjects[x].pos = pos1;
    project->guiObjects[x].pos2 = pos2;
    project->guiObjects[x].pos.y = pos2.y;
    project->setName(x, "Back Wall");
    project->guiObjects[x].wallGroup = "Room";

     x = project->addWall();
    project->guiObjects[x].pos = pos1;
    project->guiObjects[x].pos2 = pos2;
    project->guiObjects[x].pos2.x = pos1.x;
    project->setName(x, "Left Wall");
    project->guiObjects[x].wallGroup = "Room";

    x = project->addWall();
    project->guiObjects[x].pos = pos1;
    project->guiObjects[x].pos2 = pos2;
    project->guiObjects[x].pos.x = pos2.x;
    project->setName(x, "Right Wall");
    project->guiObjects[x].wallGroup = "Room";


    updateObjList();
    viewPort->Refresh();
    event.Skip();
}

void BinauralFilterGUIFrame::OnAddWallClick(wxCommandEvent &event) {
    c3DPoint pos2;
    int x = project->addWall();

    if (x >= 0) {
        project->guiObjects[x].pos = viewPort->getCenterOfView();
        pos2 = viewPort->getCenterOfView();
        pos2.y -= 1.0;
        pos2.z -= 1.0;
        project->guiObjects[x].pos2 = pos2;
        updateObjList();
        viewPort->Refresh();
    }
    event.Skip();
}

void BinauralFilterGUIFrame::OnViewPortMouseMove(int x, int y) {
    SetStatusText(_("Position: x: 1, y: 1, z: 1"), 0);
}

void BinauralFilterGUIFrame::updateObjList() {
    listObj->Clear();
    for (int i = 0; i < BinauralProject::MAX_OBJECTS; i++) {
        if (project->guiObjects[i].name != "") {
            listObj->AppendAndEnsureVisible(project->guiObjects[i].name);
        }
    }
}

void BinauralFilterGUIFrame::OnObjListClick(wxCommandEvent &evt) {
    int obj;
    viewPort->unselectAll();
    for (unsigned int i = 0; i < listObj->GetCount(); i++) {
        if (listObj->IsSelected(i) == true) {
            obj = project->findObject(listObj->GetString(i));
            if (obj >= 0) {
                project->guiObjects[obj].selected = true;
                propEdit->setObject(obj);
            }
        }
    }
    viewPort->Refresh();
    evt.Skip();
}

void BinauralFilterGUIFrame::updateObjProperties(int objNr) {
}

void BinauralFilterGUIFrame::OnRenderClick(wxCommandEvent &event) {
    project->saveToFile("rendertemp.b3s");
#ifdef __WXGTK__
    long result = wxExecute("./BinauralFilterLinux \"rendertemp.b3s\"", wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE);
#elif __WXMSW__
    long result = wxExecute("BinauralFilterWin.exe \"rendertemp.b3s\"", wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE);
#endif

    if (result <= 0) {
        wxMessageBox("Failed to launch renderer.");
    } else {
        wxMessageBox("Scene is rendering in background...");
    }
}

void BinauralFilterGUIFrame::OnTimer(wxTimerEvent &evt) {

    //Sorry, this is ugly, but i didn't figure out how to emit events in wxwidgets yet.
    if (project != nullptr && project->tempEvent.lastEvent != TempEvents::EVT_NO_EVENT) {

        switch (project->tempEvent.lastEvent) {
            case TempEvents::EVT_VP_SELECTION:
                listObj->DeselectAll();
                for (unsigned int i = 0; i < listObj->GetCount(); i++) {
                    int nr = project->findObject(listObj->GetString(i));
                    if (nr != -1 && project->guiObjects[nr].selected == true) {
                        listObj->SetSelection(i, true);
                    }
                    propEdit->setObject(project->tempEvent.objNr);
                }
                break;

            case TempEvents::EVT_VP_MOVED:
                propEdit->updateObject();
                break;

            case TempEvents::EVT_PE_PROP_CHANGE:
                viewPort->Refresh();
                break;

            case TempEvents::EVT_PE_RENAME:
                for (unsigned int i = 0; i < listObj->GetCount(); i++) {
                    if (listObj->GetString(i) == project->tempEvent.lastName) {
                        listObj->SetString(i, project->guiObjects[project->tempEvent.objNr].name);
                    }
                }
                project->tempEvent.lastName = "";
                viewPort->Refresh();
                break;
        }

        project->tempEvent.lastEvent = TempEvents::EVT_NO_EVENT;
    }
    evt.Skip();
}

void BinauralFilterGUIFrame::OnMenuSceneSettings(wxCommandEvent &event) {
    RenderSettingsFrame *window = new RenderSettingsFrame(0L, _("Scene Settings"), project);

    window->Show();
}
