#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <wx/wx.h>
#include "3DPoint.h"
#include "BinauralProject.h"
#include "guihelpers.h"
#include <wx/tokenzr.h>

class ViewPort : public wxWindow
{
    public:
        static const int VIEW_TOP = 0;
        static const int VIEW_FRONT = 1;
        static const int VIEW_SIDE = 2;

        ViewPort(wxWindow *parent, const wxPoint &pos, const wxSize &size);
        ~ViewPort();


        void setCenterOfView(c3DPoint pos);
        c3DPoint getCenterOfView();
        void setZoom(double distance);
        void setProject(BinauralProject *project);
        void renderObjects(wxDC &dc, wxPen &pen, wxBrush &brush);
        void RenderWallModifier(wxDC &dc, wxPen &pen, wxBrush &wxBrush, int objNr);
        void RenderMovementPath(wxDC &dc, wxPen &pen, wxBrush &wxBrush, int objNr);
        void BindOnClick(void (*)(int x, int y));
        void BindOnMouseMove(void (*)(int x, int y));
        wxPoint threeDToScreenPoint(c3DPoint pos);
        c3DPoint screenPointTo3D(wxPoint pos);
        int findNextObjOnPos(wxPoint pos);
        void setObjectSelection(bool value, int objNr, bool unselectPrevious = true);
        void unselectAll();
        bool isObjectUnderMouse(int objNr, wxPoint mousePos);
        bool isObjectSizerUnderMouse(int objNr, wxPoint mousePos);
        bool isObjectCenterUnderMouse(int objNr, wxPoint mousePos);
        bool isSelectedObjectUnderMouse(wxPoint mousePos);
        wxString debugString = "";
    private:
        static const int DRAG_POS1 = 0;
        static const int DRAG_POS2 = 1;
        static const int DRAG_POS1_AND_2 = 2;

        static const int DRAG_AXIS_BOTH = 0;
        static const int DRAG_AXIS_ONLY_X = 1;
        static const int DRAG_AXIS_ONLY_Y = 2;

        wxColour vpClrBackground    = wxColour(64, 64, 64);
        wxColour vpClrGrid10        = wxColour(75, 75, 75);
        wxColour vpClrGrid1         = wxColour(68, 68, 68);
        wxColour vpClrXAxis         = wxColour(160, 48, 48);
        wxColour vpClrYAxis         = wxColour(48, 160, 48);
        wxColour vpClrZAxis         = wxColour(48, 48, 160);
        wxColour vpClrSelObject     = wxColour(255, 230, 60);
        wxColour vpClrSpeaker       = wxColour(0, 0, 64);
        wxColour vpClrRndSpeaker    = wxColour(64, 0, 64);
        wxColour vpClrHead          = wxColour(64, 0, 0);
        wxColour vpClrMic           = wxColour(0, 32, 64);
        wxColour vpClrWall          = wxColour(0, 48, 0);
        wxColour vpClrPath          = wxColour(48, 24, 0);



        void OnDraw(wxDC& dc);
        void OnPaint(wxPaintEvent &evt);
        void OnMouseMove(wxMouseEvent &evt);
        void OnMouseDown(wxMouseEvent &evt);
        void OnMouseUp(wxMouseEvent &evt);
        void OnMouseWheel(wxMouseEvent &evt);

        int getDragAxis(c3DPoint pos1, c3DPoint pos2);
        void moveSelectedObjects(wxMouseEvent &evt);
        int getDragAxisForWallGroups(int objNr, int dragType);
        int getCoAxisForWallGroups(int objNr, int dragType);
        double getTolerance(double def = 0.3);
        c3DPoint centerOfView;
        double zoom = 5.0;
        int view = VIEW_TOP;
        int dragType;
        int dragAxis;
        bool LeftMouseDown = false;
        bool movingSelectedObjects = false;
        wxPoint startDrag;
        wxPoint lastMouseDown;
        BinauralProject *myProject = nullptr;
        void (*onClickFuncPtr)(int, int) = nullptr;
        void (*onMouseMovePtr)(int, int) = nullptr;

        wxDECLARE_EVENT_TABLE();

};

#endif // VIEWPORT_H
