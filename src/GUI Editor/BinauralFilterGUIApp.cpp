/***************************************************************
 * Name:      BinauralFilterGUIApp.cpp
 * Purpose:   Code for Application Class
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

#include "BinauralFilterGUIApp.h"
#include "BinauralFilterGUIMain.h"

IMPLEMENT_APP(BinauralFilterGUIApp);

bool BinauralFilterGUIApp::OnInit()
{
    BinauralFilterGUIFrame* frame = new BinauralFilterGUIFrame(0L, _("Binaural Filter GUI Editor 0.7"));
    frame->SetSize(wxSize(1024, 600));
    frame->Show();

    return true;
}
