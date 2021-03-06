/***************************************************************
 * Name:      xModelGenMain.h
 * Purpose:   Defines Application Frame
 * Author:    xLights ()
 * Created:   2016-12-30
 * Copyright: xLights (http://xlights.org)
 * License:
 **************************************************************/

#pragma once

#ifdef _MSC_VER

#include <stdlib.h>

//#define VISUALSTUDIO_MEMORYLEAKDETECTION
#ifdef VISUALSTUDIO_MEMORYLEAKDETECTION
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#endif

//(*Headers(xModelGenFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
//*)

#include "dxf/dxf_data.h"
#include "model.h"

#include <list>
#include <wx/socket.h>
#include <wx/treelist.h>
#include <wx/file.h>

#include <memory>

class CVPictureView;
class wxDebugReportCompress;

class xModelGenFrame : public wxFrame
{
    void ValidateWindow();

public:
        xModelGenFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~xModelGenFrame();
        void SendReport(const wxString &loc, wxDebugReportCompress &report);
        void CreateDebugReport(wxDebugReportCompress* report);

private:

        //(*Handlers(xModelGenFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
        void OnMenuItem_LogSelected(wxCommandEvent& event);
        void OnMenuItem_Load_DxfSelected(wxCommandEvent& event);
        void OnMenuItem_Auto_WireSelected(wxCommandEvent& event);
        void OnMenuItem_Save_XModelSelected(wxCommandEvent& event);
        void OnListBoxNodesDClick(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnGrid_NodesLeftDClick(wxMouseEvent& event);
        void OnMenuItem_Load_PictureSelected(wxCommandEvent& event);
        void OnButtonSelectClick(wxCommandEvent& event);
        void OnPanelPictureViewLeftDown(wxMouseEvent& event);
        void OnPanelPictureViewLeftUp(wxMouseEvent& event);
        void OnPanelPictureViewLeftDClick(wxMouseEvent& event);
        void OnPanelPictureViewRightDown(wxMouseEvent& event);
        //*)

        void OnTreeRClick(wxTreeListEvent& event);
        void OnPopup(wxCommandEvent& event);

        void Load_Dxf_Items( std::string const& filePath );
        void DrawGrid();
        void ClearGrid();

        std::unique_ptr<dxf_data> m_dxf_data;
        std::unique_ptr< model > m_model;

        bool m_Selecting{ false };
        wxPoint m_startPoint;

        //(*Identifiers(xModelGenFrame)
        static const long ID_LISTBOX_NODES;
        static const long ID_PROPERTY_GRID_VISION;
        static const long ID_NOTEBOOK1;
        static const long ID_PICTURE_VIEW;
        static const long ID_SPLITTERWINDOW1;
        static const long ID_BUTTON_SELECT;
        static const long ID_SLIDER_SPACING;
        static const long ID_STATUSBAR1;
        static const long ID_MNU_LOAD_PICTURE;
        static const long ID_MNU_LOAD_DXF;
        static const long ID_MNU_AUTO_WIRE;
        static const long ID_MNU_SAVE_XMODEL;
        static const long ID_MNU_QUIT;
        static const long ID_MNU_ABOUT;
        static const long ID_MNU_LOG;
        //*)

        static const long ID_MNU_CLEAR;

        //(*Declarations(xModelGenFrame)
        CVPictureView* PanelPictureView;
        wxButton* ButtonSelect;
        wxListBox* ListBoxNodes;
        wxMenu* Menu1;
        wxMenu* Menu2;
        wxMenuBar* MenuBar1;
        wxMenuItem* MenuItem_About;
        wxMenuItem* MenuItem_Auto_Wire;
        wxMenuItem* MenuItem_Load_Dxf;
        wxMenuItem* MenuItem_Load_Picture;
        wxMenuItem* MenuItem_Log;
        wxMenuItem* MenuItem_Quit;
        wxMenuItem* MenuItem_Save_XModel;
        wxNotebook* Notebook1;
        wxPropertyGrid* PropertyGrid_Vision;
        wxSlider* SliderSpacing;
        wxSplitterWindow* SplitterWindow1;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};
