#include "ViewPort.h"
#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(ViewPort, wxWindow)
    EVT_PAINT(ViewPort::OnPaint)
    EVT_MOTION(ViewPort::OnMouseMove)
    EVT_LEFT_DOWN(ViewPort::OnMouseDown)
    EVT_LEFT_UP(ViewPort::OnMouseUp)
    EVT_MOUSEWHEEL(ViewPort::OnMouseWheel)
wxEND_EVENT_TABLE()

ViewPort::ViewPort(wxWindow *parent, const wxPoint &pos, const wxSize &size) : wxWindow(parent, wxID_ANY, pos, size) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

ViewPort::~ViewPort() {

}

void ViewPort::OnDraw(wxDC& dc) {
    int width = GetSize().GetX(), height = GetSize().GetY();
    dc.Clear();

    wxBrush brush = dc.GetBrush();
    wxPen pen = dc.GetPen();

    pen.SetStyle(wxPENSTYLE_SOLID);
    pen.SetWidth(2);
    pen.SetColour(wxColour(0, 0, 0));
    brush.SetColour(vpClrBackground);

    dc.SetPen(pen);
    dc.SetBrush(brush);

    pen.SetWidth(1);
    dc.SetPen(pen);
    dc.DrawRectangle(wxPoint(0, 0), wxSize(width, height));

    /* Grid */
    pen.SetWidth(1);
    dc.SetPen(pen);

    double gridSize = 10000.0;

    if (zoom  / width < 0.0017) {
        gridSize = 0.01;
    } else if (zoom  / width < 0.017) {
        gridSize = 0.1;
    } else if (zoom  / width < 0.17) {
        gridSize = 1.0;
    } else if (zoom  / width < 1.7) {
        gridSize = 10.0;
    } else if (zoom  / width < 17.0) {
        gridSize = 100.0;
    }
    double startPx, endPx, startPy, endPy;
    switch (view) {
        case VIEW_TOP:
            startPx = round(screenPointTo3D(wxPoint(0, 0)).x / gridSize) * gridSize;
            endPx = round(screenPointTo3D(wxPoint(width, 0)).x / gridSize) * gridSize;
            for (double w = startPx; w < endPx; w += gridSize) {
                if ((int)round(w / gridSize) % 10 == 0) {
                    pen.SetColour(vpClrGrid10);
                    dc.SetPen(pen);

                } else {
                    pen.SetColour(vpClrGrid1);
                    dc.SetPen(pen);
                }
                dc.DrawLine(threeDToScreenPoint(c3DPoint(w, 0, 0)).x, 0, threeDToScreenPoint(c3DPoint(w, 0, 0)).x, height);
            }


            endPy = round(screenPointTo3D(wxPoint(0, 0)).y / gridSize) * gridSize;
            startPy = round(screenPointTo3D(wxPoint(0, height)).y / gridSize) * gridSize;
            for (double h = startPy; h < endPy; h += gridSize) {
                if ((int)round(h / gridSize) % 10 == 0) {
                    pen.SetColour(vpClrGrid10);
                    dc.SetPen(pen);
                } else {
                    pen.SetColour(vpClrGrid1);
                    dc.SetPen(pen);
                }
                dc.DrawLine(0, threeDToScreenPoint(c3DPoint(0, h, 0)).y, width, threeDToScreenPoint(c3DPoint(width, h, 0)).y);
            }
            for (double w = startPx; w < endPx; w += gridSize) {
                if ((int)round(w / gridSize) % 10 == 0) {
                    dc.DrawText(fToStr(w)+ "m", wxPoint(threeDToScreenPoint(c3DPoint(w, 0, 0)).x, height - 24));
                }
            }
            for (double h = startPy; h < endPy; h += gridSize) {
                if ((int)round(h / gridSize) % 10 == 0) {
                    dc.DrawText(fToStr(h)+ "m", wxPoint(width - 50, threeDToScreenPoint(c3DPoint(0, h, 0)).y));
                }
            }
            break;
    }

    /* 0, 0, 0 center line */
    pen.SetWidth(1);
    pen.SetColour(vpClrYAxis);
    dc.SetPen(pen);
    c3DPoint pt(0, 0, 0), pt2(0, 0, 0);
    dc.DrawLine(wxPoint(threeDToScreenPoint(pt).x, 0),
                wxPoint(threeDToScreenPoint(pt).x, height));
    pen.SetColour(vpClrXAxis);
    dc.SetPen(pen);
    dc.DrawLine(wxPoint(0, threeDToScreenPoint(pt).y),
                wxPoint(width, threeDToScreenPoint(pt).y));

    brush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetBrush(brush);
    renderObjects(dc, pen, brush);

    /* TEMP */
    dc.SetTextForeground(wxColour(255, 0, 255));
    if (debugString != "") {
        dc.DrawText(debugString, wxPoint(10, 30));
    }
}

void ViewPort::renderObjects(wxDC &dc, wxPen &pen, wxBrush &brush) {
    wxPoint crd, crd2;
    wxCoord x, y, x2, y2;
    double adjZoom = zoom;
    bool targetSelection;

    if (myProject != nullptr) {
        adjZoom = zoom;
        if (adjZoom > 5) {
            adjZoom = 5;
        }
        for (int layerPass = 0; layerPass < 2; layerPass++) {
            targetSelection = (layerPass == 1);
            if (layerPass == 0) {
                pen.SetWidth(2);
            } else if (layerPass == 1) {
                pen.SetColour(vpClrSelObject);
                dc.SetTextForeground(vpClrSelObject);
                pen.SetWidth(3);
            }
            dc.SetPen(pen);

            for (int i = 0; i < BinauralProject::MAX_OBJECTS; i++) {

                if (myProject->guiObjects[i].selected == targetSelection) {

                     /* Only box, speaker and wall comes later */
                    if (myProject->guiObjects[i].name != "" && (myProject->guiObjects[i].objType == BinauralProject::OBJ_RND_SPEAKER || myProject->guiObjects[i].objType ==  BinauralProject::OBJ_WALL)) {
                        if (myProject->guiObjects[i].selected == false && myProject->guiObjects[i].objType ==  BinauralProject::OBJ_WALL) {
                            pen.SetColour(vpClrWall);
                            dc.SetTextForeground(vpClrWall);
                            dc.SetPen(pen);
                        }
                        if (myProject->guiObjects[i].selected == false && myProject->guiObjects[i].objType ==  BinauralProject::OBJ_RND_SPEAKER) {
                            pen.SetColour(vpClrRndSpeaker);
                            dc.SetTextForeground(vpClrRndSpeaker);
                            dc.SetPen(pen);
                        }
                        crd = threeDToScreenPoint(myProject->guiObjects[i].pos);
                        crd2 = threeDToScreenPoint(myProject->guiObjects[i].pos2);
                        x = crd.x;
                        y = crd.y;
                        x2 = crd2.x;
                        y2 = crd2.y;
                        if (myProject->guiObjects[i].objType == BinauralProject::OBJ_WALL) {
                            pen.SetWidth(3);
                        } else {
                            pen.SetWidth(1);
                        }
                        dc.SetPen(pen);

                        int addPos = 0;
                        if (x == x2 || y == y2) {
                            addPos = 1;
                        }
                        dc.DrawRectangle(x, y, x2 - x + addPos, y2 - y + addPos);
                        pen.SetWidth(1);
                        dc.SetPen(pen);
                        dc.DrawRectangle(x2 - 15 / adjZoom, y2 - 15 / adjZoom, 30 / adjZoom, 30 / adjZoom);
                        dc.DrawCircle((x2 + x) / 2, (y2 + y) / 2, 17 / adjZoom);
                        dc.DrawLine((x2 + x) / 2 - 20 / adjZoom, (y2 + y) / 2, (x2 + x) / 2 + 20 / adjZoom, (y2 + y) / 2);
                        dc.DrawLine((x2 + x) / 2, (y2 + y) / 2 - 20 / adjZoom, (x2 + x) / 2, (y2 + y) / 2 + 20 / adjZoom);
                        pen.SetWidth(2);
                        dc.SetPen(pen);
                    }

                    /*speakers and rndspeakers */
                    if (myProject->guiObjects[i].name != "" && (myProject->guiObjects[i].objType == BinauralProject::OBJ_SPEAKER || myProject->guiObjects[i].objType == BinauralProject::OBJ_RND_SPEAKER)) {
                        if (myProject->guiObjects[i].keyFrames != "") {
                            RenderMovementPath(dc, pen, brush, i);
                        }
                        if (myProject->guiObjects[i].objType == BinauralProject::OBJ_SPEAKER && myProject->guiObjects[i].selected == false) {
                            pen.SetColour(vpClrSpeaker);
                            dc.SetTextForeground(vpClrSpeaker);
                            dc.SetPen(pen);
                        }
                        if (myProject->guiObjects[i].objType == BinauralProject::OBJ_RND_SPEAKER && myProject->guiObjects[i].selected == false) {
                            pen.SetColour(vpClrRndSpeaker);
                            dc.SetTextForeground(vpClrRndSpeaker);
                            dc.SetPen(pen);
                        }

                        crd = threeDToScreenPoint(myProject->guiObjects[i].pos);
                        x = crd.x;
                        y = crd.y;

                        dc.DrawRectangle(x - 10 / adjZoom, y - 13 / adjZoom, 10 / adjZoom, 27 / adjZoom);
                        dc.DrawLine(x, y - 10 / adjZoom, x + 20 / adjZoom, y - 27 / adjZoom);
                        dc.DrawLine(x, y + 10 / adjZoom, x + 20 / adjZoom, y + 27 / adjZoom);
                        dc.DrawLine(x + 20 / adjZoom, y - 27 / adjZoom, x + 20 / adjZoom, y + 27 / adjZoom);
                        dc.DrawText(myProject->guiObjects[i].name, x - 13 / adjZoom, y + 27 / adjZoom);
                    }



                    /*heads */
                    if (myProject->guiObjects[i].name != "" && myProject->guiObjects[i].objType == BinauralProject::OBJ_HEAD) {
                        if ( myProject->guiObjects[i].selected == false) {
                            pen.SetColour(vpClrHead);
                            dc.SetTextForeground(vpClrHead);
                            dc.SetPen(pen);
                        }

                        crd = threeDToScreenPoint(myProject->guiObjects[i].pos);
                        x = crd.x;
                        y = crd.y;

                        double radius = myProject->guiObjects[i].earDistance / zoom * GetSize().GetWidth() / 4.0;

                        /* left ear */

                        dc.DrawCircle(x - cos((360 - myProject->guiObjects[i].angle) / 180 * M_PI) * radius * 1.15,
                                      y + sin((360 - myProject->guiObjects[i].angle) / 180 * M_PI) * radius * 1.15,
                                      5 / zoom);
                        /* right ear */
                        dc.DrawCircle(x + cos((360 - myProject->guiObjects[i].angle) / 180 * M_PI) * radius *1.15,
                                      y - sin((360 - myProject->guiObjects[i].angle) / 180 * M_PI) * radius *1.15,
                                      5 / zoom);
                        /* nose */
                        dc.DrawLine(x + sin((myProject->guiObjects[i].angle - 10) / 180 * M_PI) * radius,
                                    y - cos((myProject->guiObjects[i].angle - 10) / 180 * M_PI) * radius,
                                    x + sin((myProject->guiObjects[i].angle) / 180 * M_PI) * radius * 1.4,
                                    y - cos((myProject->guiObjects[i].angle) / 180 * M_PI) * radius * 1.4);
                        dc.DrawLine(x + sin((myProject->guiObjects[i].angle + 10) / 180 * M_PI) * radius,
                                    y - cos((myProject->guiObjects[i].angle + 10) / 180 * M_PI) * radius,
                                    x + sin((myProject->guiObjects[i].angle) / 180 * M_PI) * radius * 1.4,
                                    y - cos((myProject->guiObjects[i].angle) / 180 * M_PI) * radius * 1.4);
                        /* head circle */
                        dc.DrawCircle(x, y, radius);
                        dc.DrawText(myProject->guiObjects[i].name, x - 40 / zoom, y + 30 / zoom);
                    }



                    /*mics */
                    if (myProject->guiObjects[i].name != "" && myProject->guiObjects[i].objType == BinauralProject::OBJ_MICROPHONE) {
                        if (myProject->guiObjects[i].selected == false) {
                            pen.SetColour(vpClrMic);
                            dc.SetTextForeground(vpClrMic);
                            dc.SetPen(pen);
                        }
                        crd = threeDToScreenPoint(myProject->guiObjects[i].pos);
                        x = crd.x;
                        y = crd.y;

                        dc.DrawCircle(x - 0 / adjZoom, y - 0 / adjZoom, 17 / adjZoom);
                        dc.DrawLine(x + 0 / adjZoom, y + 17 / adjZoom, x + 21 / adjZoom, y + 47 / adjZoom);
                        dc.DrawLine(x + 12 / adjZoom, y + 12 / adjZoom, x + 25 / adjZoom, y + 45 / adjZoom);
                        dc.DrawLine(x + 21 / adjZoom, y + 47 / adjZoom, x + 25 / adjZoom, y + 45 / adjZoom);
                        dc.SetPen(pen);
                        dc.DrawText(myProject->guiObjects[i].name, x - 13 / adjZoom, y + 47 / adjZoom);
                    }

                    /* walls */
                    if (myProject->guiObjects[i].name != "" && myProject->guiObjects[i].objType == BinauralProject::OBJ_WALL) {
                        if (myProject->guiObjects[i].selected == false) {
                            pen.SetColour(vpClrWall);
                            dc.SetTextForeground(vpClrWall);
                            dc.SetPen(pen);
                        }
                        crd = threeDToScreenPoint(myProject->guiObjects[i].pos);
                        x = crd.x;
                        y = crd.y;
                        x2 = crd2.x;
                        y2 = crd2.y;
                        pen.SetWidth(1);
                        dc.SetPen(pen);

                        dc.DrawRectangle(x - 15 / adjZoom, y - 15 / adjZoom, 30 / adjZoom, 30 / adjZoom);
                        if (myProject->guiObjects[i].selected == true) {
                            dc.DrawText(myProject->guiObjects[i].name, (x + x2) / 2 + 25 / adjZoom, (y2 + y) / 2 - 15 / adjZoom);
                        }

                        if (myProject->guiObjects[i].modifier != "") {
                            RenderWallModifier(dc, pen, brush, i);
                        }
                    }
                }
            }
        }
    }
}

void ViewPort::RenderWallModifier(wxDC &dc, wxPen &pen, wxBrush &wxBrush, int objNr) {
    int usedMod = 0;
    double amplitude = 0.0, waveLength = 0.0, shift = 0.0;
    c3DPoint screenPos1 = screenPointTo3D(wxPoint(0, 0));
    c3DPoint screenPos2 = screenPointTo3D(wxPoint(GetSize().GetWidth(), GetSize().GetHeight()));

    wxStringTokenizer token(myProject->guiObjects[objNr].modifier, ",");
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

    float start, stop;
    switch (view) {
        case VIEW_TOP:
            if (myProject->getWallType(objNr) == BinauralProject::WT_SIDE) {
                start = std::min(myProject->guiObjects[objNr].pos.y, myProject->guiObjects[objNr].pos2.y);
                stop = std::max(myProject->guiObjects[objNr].pos.y, myProject->guiObjects[objNr].pos2.y);
                for (float p = start; p < stop; p += zoom / 80) {
                    if (p >= screenPos2.y && p <= screenPos1.y) {
                        dc.DrawCircle(
                                      threeDToScreenPoint(c3DPoint(myProject->guiObjects[objNr].pos.x
                                      + myProject->getModifiedWallAmplitude(usedMod, p, amplitude,
                                      shift, waveLength), p, 0)), 1);
                    }
                }
            }
            if (myProject->getWallType(objNr) == BinauralProject::WT_FRONT) {
                start = std::min(myProject->guiObjects[objNr].pos.x, myProject->guiObjects[objNr].pos2.x);
                stop = std::max(myProject->guiObjects[objNr].pos.x, myProject->guiObjects[objNr].pos2.x);
                for (float p = start; p < stop; p += zoom / 80) {
                    if (p >= screenPos1.x && p <= screenPos2.x) {
                        dc.DrawCircle(
                                      threeDToScreenPoint(c3DPoint(p, myProject->guiObjects[objNr].pos.y
                                      + myProject->getModifiedWallAmplitude(usedMod, p, amplitude,
                                      shift, waveLength), 0)), 1);
                    }
                }
            }

            break;
    }
}

void ViewPort::RenderMovementPath(wxDC &dc, wxPen &pen, wxBrush &wxBrush, int objNr) {
    c3DPoint screenPos1 = screenPointTo3D(wxPoint(0, 0));
    c3DPoint screenPos2 = screenPointTo3D(wxPoint(GetSize().GetWidth(), GetSize().GetHeight()));
    c3DPoint pos, lastPos = myProject->guiObjects[objNr].pos;
    int pw = pen.GetWidth();

    pen.SetWidth(1);
    if (myProject->guiObjects[objNr].selected == false) {
        pen.SetColour(vpClrPath);
    }
    dc.SetPen(pen);
    wxStringTokenizer token(myProject->guiObjects[objNr].keyFrames, "/");
    while(token.HasMoreTokens()) {
        wxString keyStr = token.GetNextToken();
        wxStringTokenizer keyToken(keyStr, ",");
        keyToken.GetNextToken();
        pos.x = myProject->guiObjects[objNr].pos.x + atof(keyToken.GetNextToken());
        pos.y = myProject->guiObjects[objNr].pos.y + atof(keyToken.GetNextToken());
        pos.z = myProject->guiObjects[objNr].pos.z + atof(keyToken.GetNextToken());

        //todo: line is not drawn when points are out of view but line should be in view
        if ((pos.x >= screenPos1.x && pos.x <= screenPos2.x && pos.y >= screenPos2.y && pos.y <= screenPos1.y)
            || (lastPos.x >= screenPos1.x && lastPos.x <= screenPos2.x && lastPos.y >= screenPos2.y && lastPos.y <= screenPos1.y)) {
            dc.DrawLine(threeDToScreenPoint(lastPos), threeDToScreenPoint(pos));
            dc.DrawCircle(threeDToScreenPoint(pos), 2);
        }
        lastPos = pos;
    }

    pen.SetWidth(pw);
    dc.SetPen(pen);
}


void ViewPort::OnPaint(wxPaintEvent &evt) {
    wxBufferedPaintDC dc(this);
    this->PrepareDC(dc);
    this->OnDraw(dc);
    evt.Skip();
}

void ViewPort::setCenterOfView(c3DPoint pos) {
    centerOfView = pos;
}

c3DPoint ViewPort::getCenterOfView() {
    return centerOfView;
}

void ViewPort::setZoom(double distance) {
    zoom = distance;
}

wxPoint ViewPort::threeDToScreenPoint(c3DPoint pos) {
    wxPoint ret(0, 0);
    switch (view) {
        case VIEW_TOP:
            ret.x = GetSize().GetWidth() / 2 + (pos.getX() - centerOfView.getX()) / zoom * GetSize().GetWidth() / 2;
            ret.y = GetSize().GetHeight() - (GetSize().GetHeight() / 2 + (pos.getY() - centerOfView.getY()) / zoom * GetSize().GetWidth() / 2);
            return ret;
    }

    return ret;
}

c3DPoint ViewPort::screenPointTo3D(wxPoint pos) {
    c3DPoint ret = centerOfView;

    switch (view) {
        case VIEW_TOP:
            ret.setPoint((pos.x - GetSize().GetWidth() / 2.0) / (GetSize().GetWidth() / 2.0)
                         * zoom + centerOfView.getX(), ret.getY(), ret.getZ());
            ret.setPoint(ret.getX(), ((GetSize().GetHeight() - pos.y) - GetSize().GetHeight() / 2.0) / (GetSize().GetWidth() / 2.0)
                         * zoom + centerOfView.getY(), ret.getZ());

            return ret;
    }

    return ret;
}

void ViewPort::setProject(BinauralProject *project) {
    myProject = project;
}

void ViewPort::OnMouseMove(wxMouseEvent &evt) {
    if (LeftMouseDown == true) {
        if (movingSelectedObjects == false) {
            wxPoint tmp = threeDToScreenPoint(centerOfView) + (startDrag - evt.GetPosition());
            setCenterOfView(screenPointTo3D(tmp));
        } else {
            moveSelectedObjects(evt);
        }
        Refresh();
        evt.Skip();
        startDrag = evt.GetPosition();
    }
    if (onMouseMovePtr != nullptr) {
        onMouseMovePtr(evt.GetPosition().x, evt.GetPosition().y);
    }

    evt.Skip();
}



void ViewPort::OnMouseDown(wxMouseEvent &evt) {
    LeftMouseDown = true;
    startDrag = evt.GetPosition();
    lastMouseDown = startDrag;
    movingSelectedObjects = isSelectedObjectUnderMouse(evt.GetPosition());
    dragType = DRAG_POS1;
    dragAxis = DRAG_AXIS_BOTH;

    if (movingSelectedObjects == true) {
        for (int i = 0; i < BinauralProject::MAX_OBJECTS; i++) {
            if (myProject->guiObjects[i].selected == true && (myProject->guiObjects[i].objType == BinauralProject::OBJ_RND_SPEAKER || myProject->guiObjects[i].objType == BinauralProject::OBJ_WALL)) {
                if (isObjectCenterUnderMouse(i, evt.GetPosition()) == true) {
                    dragType = DRAG_POS1_AND_2;
                } else if (isObjectSizerUnderMouse(i, evt.GetPosition()) == true) {
                    dragType = DRAG_POS2;
                } else if (isObjectSizerUnderMouse(i, evt.GetPosition()) == true) {
                    dragType = DRAG_POS1_AND_2;
                }

            }
            if (myProject->guiObjects[i].selected == true && myProject->guiObjects[i].objType == BinauralProject::OBJ_WALL) {
                dragAxis = getDragAxis(myProject->guiObjects[i].pos, myProject->guiObjects[i].pos2);

            }
        }
    }
    evt.Skip();
}

void ViewPort::OnMouseUp(wxMouseEvent &evt) {
    if (abs(lastMouseDown.x - evt.GetPosition().x) < 2 &&
        abs(lastMouseDown.y - evt.GetPosition().y) < 2) {

        int obj = findNextObjOnPos(lastMouseDown);
        setObjectSelection(true, obj, true);
        myProject->tempEvent.lastEvent = TempEvents::EVT_VP_SELECTION;
        myProject->tempEvent.objNr = obj;
        Refresh();
    }
    LeftMouseDown = false;

    evt.Skip();
}

void ViewPort::OnMouseWheel(wxMouseEvent &evt) {
    zoom *= 1.0 - (evt.GetWheelRotation() / evt.GetWheelDelta() / 7.0);
    Refresh();

    evt.Skip();
}

void ViewPort::BindOnClick(void (*func)(int x, int y)) {
    onClickFuncPtr = func;
}

void ViewPort::BindOnMouseMove(void (*func)(int x, int y)) {
    onMouseMovePtr = func;
}

int ViewPort::findNextObjOnPos(wxPoint pos) {
    int ret = -1;

    for (int objNr = 0; objNr < BinauralProject::MAX_OBJECTS; objNr++) {
        if (myProject->guiObjects[objNr].name != "") {
            if (isObjectUnderMouse(objNr, pos) == true ||
                isObjectSizerUnderMouse(objNr, pos) == true ||
                isObjectCenterUnderMouse(objNr, pos) == true) {

                return objNr;
            }
        }
    }
    return ret;
}

double ViewPort::getTolerance(double def) {
    double ret = def;

    if (zoom / GetSize().GetWidth() < 15) {
        ret += zoom / GetSize().GetWidth() * 15;
    }

    return ret;
}

bool ViewPort::isObjectUnderMouse(int obj, wxPoint mousePos) {
    c3DPoint mouse3D = screenPointTo3D(mousePos);
    double tolerance = getTolerance();

    if (obj < 0 || obj > BinauralProject::MAX_OBJECTS || myProject->guiObjects[obj].name == "") {
        return false;
    }

    switch (view) {
        case VIEW_TOP:
            if (myProject->guiObjects[obj].objType == BinauralProject::OBJ_SPEAKER || myProject->guiObjects[obj].objType == BinauralProject::OBJ_RND_SPEAKER || myProject->guiObjects[obj].objType == BinauralProject::OBJ_WALL) {
                if (abs(mouse3D.x - myProject->guiObjects[obj].pos.x) < tolerance &&
                abs(mouse3D.y - myProject->guiObjects[obj].pos.y) < tolerance) {
                    return true;
                }
            }
            if (myProject->guiObjects[obj].objType == BinauralProject::OBJ_MICROPHONE) {
                if (abs(mouse3D.x - myProject->guiObjects[obj].pos.x) < tolerance &&
                abs(mouse3D.y - myProject->guiObjects[obj].pos.y) < tolerance) {
                    return true;
                }
            }
            if (myProject->guiObjects[obj].objType == BinauralProject::OBJ_HEAD) {
                if (abs(mouse3D.x - myProject->guiObjects[obj].pos.x) < tolerance &&
                abs(mouse3D.y - myProject->guiObjects[obj].pos.y) < tolerance) {
                    return true;
                }
            }

            break;
    }
    return false;
}

bool ViewPort::isObjectSizerUnderMouse(int obj, wxPoint mousePos) {
    double tolerance = getTolerance(0.2);
    c3DPoint mouse3D = screenPointTo3D(mousePos);
    if (obj < 0 || obj > BinauralProject::MAX_OBJECTS || myProject->guiObjects[obj].name == "") {
        return false;
    }

    switch (view) {
        case VIEW_TOP:
            if (myProject->guiObjects[obj].objType == BinauralProject::OBJ_RND_SPEAKER || myProject->guiObjects[obj].objType == BinauralProject::OBJ_WALL) {
                if (abs(mouse3D.x - myProject->guiObjects[obj].pos2.x) < tolerance &&
                abs(mouse3D.y - myProject->guiObjects[obj].pos2.y) < tolerance) {
                    return true;
                }
            }
    }
    return false;
}

bool ViewPort::isObjectCenterUnderMouse(int obj, wxPoint mousePos) {
    c3DPoint mouse3D = screenPointTo3D(mousePos);
    double tolerance = getTolerance(0.2);

    if (obj < 0 || obj > BinauralProject::MAX_OBJECTS || myProject->guiObjects[obj].name == "") {
        return false;
    }

    switch (view) {
        case VIEW_TOP:
            if (myProject->guiObjects[obj].objType == BinauralProject::OBJ_RND_SPEAKER || myProject->guiObjects[obj].objType == BinauralProject::OBJ_WALL) {
                if (abs(mouse3D.x - (myProject->guiObjects[obj].pos2.x + myProject->guiObjects[obj].pos.x) / 2) < tolerance &&
                abs(mouse3D.y - (myProject->guiObjects[obj].pos2.y + myProject->guiObjects[obj].pos.y) / 2) < tolerance) {
                    return true;
                }
            }
    }

    return false;
}

bool ViewPort::isSelectedObjectUnderMouse(wxPoint mousePos) {
    int i;


    for (i = 0; i < BinauralProject::MAX_OBJECTS; i++) {
        if (myProject->guiObjects[i].name != "" && myProject->guiObjects[i].selected == true) {
            if (isObjectUnderMouse(i, mousePos) == true) {
                return true;
            } else if (isObjectCenterUnderMouse(i, mousePos) == true) {
                return true;
            } else if (isObjectSizerUnderMouse(i, mousePos) == true) {
                return true;
            }
        }
    }
    return false;
}

void ViewPort::setObjectSelection(bool value, int obj, bool unselectPrevious) {
    if (unselectPrevious == true) {
        unselectAll();
    }
    myProject->guiObjects[obj].selected = true;
}

void ViewPort::unselectAll() {
    for (int i = 0; i < BinauralProject::MAX_OBJECTS; i++) {
        myProject->guiObjects[i].selected = false;
    }
}

int ViewPort::getDragAxis(c3DPoint pos1, c3DPoint pos2){
    float relation;
    switch (view) {
        case VIEW_TOP:
            relation = abs(pos2.x - pos1.x) /
                       abs(pos2.y - pos1.y);
            if (relation > 2) {
                return DRAG_AXIS_ONLY_X;
            }
            if (relation < .5) {
                return DRAG_AXIS_ONLY_Y;
            }
            return DRAG_AXIS_BOTH;
    }

    return DRAG_AXIS_BOTH;
}

void ViewPort::moveSelectedObjects(wxMouseEvent &evt) {
    c3DPoint p1 = screenPointTo3D(startDrag);
    c3DPoint p2 = screenPointTo3D(evt.GetPosition());
    c3DPoint oldPos1, oldPos2;

    for (int i = 0; i < BinauralProject::MAX_OBJECTS; i++) {
        bool isInGroup = false;
        int dragCoAxis = DRAG_AXIS_BOTH;

        if (myProject->guiObjects[i].objType == BinauralProject::OBJ_WALL) {
            isInGroup = myProject->isWallConnectedToGroup(i);
            if (isInGroup == true) {
                dragCoAxis = getCoAxisForWallGroups(i, dragType);
            }

        }


        if (myProject->guiObjects[i].name != "" && myProject->guiObjects[i].selected == true) {
            myProject->tempEvent.lastEvent = TempEvents::EVT_VP_MOVED;
            myProject->tempEvent.objNr = i;

            oldPos1 = myProject->guiObjects[i].pos;
            oldPos2 = myProject->guiObjects[i].pos2;
            if (dragType == DRAG_POS1 || dragType == DRAG_POS1_AND_2) {
                myProject->guiObjects[i].pos.x += p2.x - p1.x;
                myProject->guiObjects[i].pos.y += p2.y - p1.y;
                myProject->guiObjects[i].pos.z += p2.z - p1.z; //todo: perspective
            }
            if (dragType == DRAG_POS2 || dragType == DRAG_POS1_AND_2) {
                myProject->guiObjects[i].pos2.x += p2.x - p1.x;
                myProject->guiObjects[i].pos2.y += p2.y - p1.y;
                myProject->guiObjects[i].pos2.z += p2.z - p1.z;
            }

            if (myProject->guiObjects[i].objType == BinauralProject::OBJ_WALL) {


                if (dragType == DRAG_POS1) {

                    if (isInGroup == true) {
                        dragAxis = DRAG_AXIS_BOTH;
                    } else {
                        dragAxis = getDragAxis(myProject->guiObjects[i].pos2, p2);
                    }


                    if (dragAxis == DRAG_AXIS_ONLY_X) {
                        myProject->guiObjects[i].pos.y = myProject->guiObjects[i].pos2.y;
                    }
                    if (dragAxis == DRAG_AXIS_ONLY_Y) {
                        myProject->guiObjects[i].pos.x = myProject->guiObjects[i].pos2.x;
                    }

                    if (dragCoAxis == DRAG_AXIS_ONLY_X) {
                        myProject->guiObjects[i].pos2.x = myProject->guiObjects[i].pos.x;
                    }
                    if (dragCoAxis == DRAG_AXIS_ONLY_Y) {
                        myProject->guiObjects[i].pos2.y = myProject->guiObjects[i].pos.y;

                    }
                }


                if (dragType == DRAG_POS2) {
                    if (isInGroup == true) {
                        dragAxis = DRAG_AXIS_BOTH;
                    } else {
                        dragAxis = getDragAxis(myProject->guiObjects[i].pos, p2);
                    }
                    if (dragAxis == DRAG_AXIS_ONLY_X) {
                        myProject->guiObjects[i].pos2.y = myProject->guiObjects[i].pos.y;
                    }
                    if (dragAxis == DRAG_AXIS_ONLY_Y) {
                        myProject->guiObjects[i].pos2.x = myProject->guiObjects[i].pos.x;
                    }

                    if (dragCoAxis == DRAG_AXIS_ONLY_X) {
                        myProject->guiObjects[i].pos.x = myProject->guiObjects[i].pos2.x;
                    }
                    if (dragCoAxis == DRAG_AXIS_ONLY_Y) {
                        myProject->guiObjects[i].pos.y = myProject->guiObjects[i].pos2.y;
                    }
                }

                if (dragType == DRAG_POS1_AND_2) {
                    if (isInGroup == true) {
                        dragAxis = getDragAxisForWallGroups(i, dragType);

                        if (dragAxis == DRAG_AXIS_ONLY_X) {

                            myProject->guiObjects[i].pos2.y = oldPos2.y;
                            myProject->guiObjects[i].pos.y = oldPos1.y;
                        }
                        if (dragAxis == DRAG_AXIS_ONLY_Y) {
                            myProject->guiObjects[i].pos.x = oldPos1.x;
                            myProject->guiObjects[i].pos2.x = oldPos2.x;

                        }

                    }
                }

                if (isInGroup == true) {
                    myProject->movePointsInWallGroup(i, oldPos1, oldPos2);
                }
            }
        }
    }
}

int ViewPort::getDragAxisForWallGroups(int objNr, int dragType) {
    switch (view) {

        case VIEW_TOP:
            switch (myProject->getWallType(objNr)) {
                case  BinauralProject::WT_FRONT:
                    if (dragType == DRAG_POS1_AND_2) {
                        return DRAG_AXIS_ONLY_Y;

                    }
                    break;
                case BinauralProject::WT_SIDE:
                    if (dragType == DRAG_POS1_AND_2) {
                        return DRAG_AXIS_ONLY_X;
                    }
                    break;

            }

            break;

    }
    return DRAG_AXIS_BOTH;
}

int ViewPort::getCoAxisForWallGroups(int objNr, int dragType) {
   switch (view) {

        case VIEW_TOP:
            switch (myProject->getWallType(objNr)) {
                case  BinauralProject::WT_FRONT:
                    if (dragType == DRAG_POS1 || dragType == DRAG_POS2) {
                        return DRAG_AXIS_ONLY_Y;
                    }

                    break;
                case BinauralProject::WT_SIDE:
                    if (dragType == DRAG_POS1 || dragType == DRAG_POS2) {
                        return DRAG_AXIS_ONLY_X;
                    }
                    break;
            }

            break;

    }
    return DRAG_AXIS_BOTH;
}
