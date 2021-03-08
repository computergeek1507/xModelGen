/***************************************************************
 * Name:      xModelGenMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    xLights ()
 * Created:   2016-12-30
 * Copyright: xLights (http://xlights.org)
 * License:
 **************************************************************/

#include "xModelGenMain.h"
#include "CVPictureView.h"

#include "dxf/dxf_reader.h"

#include "../xLights/osxMacUtils.h"
#include "../xLights/xLightsVersion.h"
#include "../xLights/UtilFunctions.h"

#include <log4cpp/Category.hh>

#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcscreen.h>
#include <wx/dnd.h>
#include <wx/dcbuffer.h>
#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/debugrpt.h>
#include <wx/protocol/http.h>
#include <wx/filedlg.h>
#include <wx/numdlg.h>
#include <wx/progdlg.h>
#include <wx/dataview.h>
#include <wx/dir.h>
#include <wx/mimetype.h>
#include <wx/propgrid/advprops.h>


//std libraries
#include <filesystem>
#include <iostream>

#include "../include/xLights.xpm"
#include "../include/xLights-16.xpm"
#include "../include/xLights-32.xpm"
#include "../include/xLights-64.xpm"
#include "../include/xLights-128.xpm"

//(*InternalHeaders(xModelGenFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

class xlImageProperty : public wxImageFileProperty
{
public:
    xlImageProperty( const wxString& label,
                     const wxString& name,
                     const wxString& value,
                     const wxImage* img ) :
        wxImageFileProperty( label, name, value )
    {

        if( img != nullptr ) {
            m_pImage = new wxImage( *img );
        }
    }
    virtual ~xlImageProperty() {}

    void OnSetValue() override
    {

    }
};

wxPen __backgroundPen( *wxWHITE );
wxBrush __backgroundBrush( *wxWHITE );

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

//(*IdInit(xModelGenFrame)
const long xModelGenFrame::ID_LISTBOX_NODES = wxNewId();
const long xModelGenFrame::ID_STATICTEXT_NODES = wxNewId();
const long xModelGenFrame::ID_BUTTON_SELECT = wxNewId();
const long xModelGenFrame::ID_PANEL_NODES = wxNewId();
const long xModelGenFrame::ID_CHECKBOX_ENABLE_WIRING = wxNewId();
const long xModelGenFrame::ID_STATICTEXT1 = wxNewId();
const long xModelGenFrame::ID_SPINCTRL_NEXT_NODE_NUMBER = wxNewId();
const long xModelGenFrame::ID_PANEL_MANUAL_WIRING = wxNewId();
const long xModelGenFrame::ID_PROPERTY_GRID_VISION = wxNewId();
const long xModelGenFrame::ID_NOTEBOOK1 = wxNewId();
const long xModelGenFrame::ID_PICTURE_VIEW = wxNewId();
const long xModelGenFrame::ID_SPLITTERWINDOW1 = wxNewId();
const long xModelGenFrame::ID_STATUSBAR1 = wxNewId();
const long xModelGenFrame::ID_MNU_LOAD_PICTURE = wxNewId();
const long xModelGenFrame::ID_MNU_AUTO_WIRE = wxNewId();
const long xModelGenFrame::ID_MNU_SAVE_XMODEL = wxNewId();
const long xModelGenFrame::ID_MNU_QUIT = wxNewId();
const long xModelGenFrame::ID_MNU_ABOUT = wxNewId();
const long xModelGenFrame::ID_MNU_LOG = wxNewId();
//*)

const long xModelGenFrame::ID_MNU_CLEAR = wxNewId();
const long xModelGenFrame::ID_MNU_CLEAR_SELECTION = wxNewId();
const long xModelGenFrame::ID_MNU_ADD_NODE = wxNewId();

//wxDEFINE_EVENT(EVT_SCANPROGRESS, wxCommandEvent);

BEGIN_EVENT_TABLE(xModelGenFrame,wxFrame)
    //(*EventTable(xModelGenFrame)
    //*)
    //EVT_COMMAND(wxID_ANY, EVT_SCANPROGRESS, xModelGenFrame::ScanUpdate)
END_EVENT_TABLE()

xModelGenFrame::xModelGenFrame(wxWindow* parent, wxWindowID id)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    //(*Initialize(xModelGenFrame)
    wxBoxSizer* BoxSizer2;
    wxFlexGridSizer* FlexGridSizer1;
    wxGridBagSizer* GridBagSizer1;

    Create(parent, wxID_ANY, _("xModel Gen"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(400,400));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableRow(0);
    SplitterWindow1 = new wxSplitterWindow(this, ID_SPLITTERWINDOW1, wxDefaultPosition, wxDefaultSize, wxSP_3D, _T("ID_SPLITTERWINDOW1"));
    SplitterWindow1->SetMinSize(wxSize(50,50));
    SplitterWindow1->SetMinimumPaneSize(10);
    SplitterWindow1->SetSashGravity(0.2);
    Notebook1 = new wxNotebook(SplitterWindow1, ID_NOTEBOOK1, wxPoint(-38,2), wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
    PanelNodes = new wxPanel(Notebook1, ID_PANEL_NODES, wxPoint(21,34), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_NODES"));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    ListBoxNodes = new wxListBox(PanelNodes, ID_LISTBOX_NODES, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX_NODES"));
    BoxSizer2->Add(ListBoxNodes, 1, wxALL|wxEXPAND, 5);
    StaticTextNodes = new wxStaticText(PanelNodes, ID_STATICTEXT_NODES, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT_NODES"));
    BoxSizer2->Add(StaticTextNodes, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ButtonSelect = new wxButton(PanelNodes, ID_BUTTON_SELECT, _("Set Node Template"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SELECT"));
    BoxSizer2->Add(ButtonSelect, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PanelNodes->SetSizer(BoxSizer2);
    BoxSizer2->Fit(PanelNodes);
    BoxSizer2->SetSizeHints(PanelNodes);
    PanelManualWiring = new wxPanel(Notebook1, ID_PANEL_MANUAL_WIRING, wxPoint(11,15), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_MANUAL_WIRING"));
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    CheckBoxEnableWiring = new wxCheckBox(PanelManualWiring, ID_CHECKBOX_ENABLE_WIRING, _("Enable Manual Wiring"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_ENABLE_WIRING"));
    CheckBoxEnableWiring->SetValue(false);
    GridBagSizer1->Add(CheckBoxEnableWiring, wxGBPosition(0, 0), wxGBSpan(1, 2), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(PanelManualWiring, ID_STATICTEXT1, _("Node"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    GridBagSizer1->Add(StaticText1, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrlNextNodeNumber = new wxSpinCtrl(PanelManualWiring, ID_SPINCTRL_NEXT_NODE_NUMBER, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 2000, 1, _T("ID_SPINCTRL_NEXT_NODE_NUMBER"));
    SpinCtrlNextNodeNumber->SetValue(_T("1"));
    GridBagSizer1->Add(SpinCtrlNextNodeNumber, wxGBPosition(1, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    PanelManualWiring->SetSizer(GridBagSizer1);
    GridBagSizer1->Fit(PanelManualWiring);
    GridBagSizer1->SetSizeHints(PanelManualWiring);
    PropertyGrid_Vision = new wxPropertyGrid(Notebook1,ID_PROPERTY_GRID_VISION,wxPoint(72,36),wxDefaultSize,wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE,_T("ID_PROPERTY_GRID_VISION"));
    Notebook1->AddPage(PanelNodes, _("Nodes"), true);
    Notebook1->AddPage(PanelManualWiring, _("Manual Wiring"), false);
    Notebook1->AddPage(PropertyGrid_Vision, _("Vision Settings"), false);
    PanelPictureView = new CVPictureView(SplitterWindow1, ID_PICTURE_VIEW, wxPoint(195,31), wxSize(100,100), wxTAB_TRAVERSAL, _T("ID_PICTURE_VIEW"));
    SplitterWindow1->SplitVertically(Notebook1, PanelPictureView);
    SplitterWindow1->SetSashPosition(100);
    FlexGridSizer1->Add(SplitterWindow1, 1, wxALL|wxEXPAND, 5);
    SetSizer(FlexGridSizer1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -10 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem_Load_Picture = new wxMenuItem(Menu1, ID_MNU_LOAD_PICTURE, _("Load Picture"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_Load_Picture);
    MenuItem_Auto_Wire = new wxMenuItem(Menu1, ID_MNU_AUTO_WIRE, _("Start Auto Wire"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_Auto_Wire);
    MenuItem_Save_XModel = new wxMenuItem(Menu1, ID_MNU_SAVE_XMODEL, _("Save xModel"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_Save_XModel);
    Menu1->AppendSeparator();
    MenuItem_Quit = new wxMenuItem(Menu1, ID_MNU_QUIT, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem_Quit);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem_About = new wxMenuItem(Menu2, ID_MNU_ABOUT, _("About\tF1"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItem_About);
    MenuItem_Log = new wxMenuItem(Menu2, ID_MNU_LOG, _("View Log"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItem_Log);
    MenuBar1->Append(Menu2, _("&Help"));
    SetMenuBar(MenuBar1);
    SetSizer(FlexGridSizer1);
    Layout();

    Connect(ID_LISTBOX_NODES,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&xModelGenFrame::OnListBoxNodesDClick);
    Connect(ID_BUTTON_SELECT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xModelGenFrame::OnButtonSelectClick);
    PanelPictureView->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&xModelGenFrame::OnPanelPictureViewLeftDown,0,this);
    PanelPictureView->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&xModelGenFrame::OnPanelPictureViewLeftUp,0,this);
    PanelPictureView->Connect(wxEVT_LEFT_DCLICK,(wxObjectEventFunction)&xModelGenFrame::OnPanelPictureViewLeftDClick,0,this);
    PanelPictureView->Connect(wxEVT_RIGHT_DOWN,(wxObjectEventFunction)&xModelGenFrame::OnPanelPictureViewRightDown,0,this);
    Connect(ID_MNU_LOAD_PICTURE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xModelGenFrame::OnMenuItem_Load_PictureSelected);
    Connect(ID_MNU_AUTO_WIRE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xModelGenFrame::OnMenuItem_Auto_WireSelected);
    Connect(ID_MNU_SAVE_XMODEL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xModelGenFrame::OnMenuItem_Save_XModelSelected);
    Connect(ID_MNU_QUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xModelGenFrame::OnQuit);
    Connect(ID_MNU_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xModelGenFrame::OnAbout);
    Connect(ID_MNU_LOG,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xModelGenFrame::OnMenuItem_LogSelected);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&xModelGenFrame::OnClose);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&xModelGenFrame::OnResize);
    //*)

    SetTitle("xModel Generator " + GetDisplayVersionString());

    wxIconBundle icons;
    icons.AddIcon(wxIcon(xlights_16_xpm));
    icons.AddIcon(wxIcon(xlights_32_xpm));
    icons.AddIcon(wxIcon(xlights_64_xpm));
    icons.AddIcon(wxIcon(xlights_128_xpm));
    icons.AddIcon(wxIcon(xlights_xpm));
    SetIcons(icons);

    SetMinSize(wxSize(400, 300));
    SetSize(800, 600);

    logger_base.debug("Loading...");

    DrawPropertyGrid();


    PropertyGrid_Vision->Connect( wxEVT_PG_CHANGED, (wxObjectEventFunction)&xModelGenFrame::OnPropertyGridChange, 0, this );
    //PropertyGrid_Vision->Connect( wxEVT_PG_SELECTED, (wxObjectEventFunction)&LayoutPanel::OnPropertyGridSelection, 0, this );
    //PropertyGrid_Vision->Connect( wxEVT_PG_ITEM_COLLAPSED, (wxObjectEventFunction)&LayoutPanel::OnPropertyGridItemCollapsed, 0, this );
    //PropertyGrid_Vision->Connect( wxEVT_PG_ITEM_EXPANDED, (wxObjectEventFunction)&LayoutPanel::OnPropertyGridItemExpanded, 0, this );
    //PropertyGrid_Vision->Connect( wxEVT_PG_RIGHT_CLICK, (wxObjectEventFunction)&LayoutPanel::OnPropertyGridRightClick, 0, this );
    //PropertyGrid_Vision->SetValidationFailureBehavior( wxPG_VFB_MARK_CELL | wxPG_VFB_BEEP );

    ValidateWindow();


}


void xModelGenFrame::DrawPropertyGrid()
{
    //PropertyGrid_Vision->Append( new wxBoolProperty( "Fill", "BkgFill", false ) )->SetAttribute( "UseCheckbox", 1 );
    PropertyGrid_Vision->Freeze();
    PropertyGrid_Vision->Clear();

    wxConfigBase* config = wxConfigBase::Get();

    wxPGProperty* ms = PropertyGrid_Vision->Append( new wxPropertyCategory( "Match Setting", "MatchSetting" ) );

    wxPGProperty* prop = PropertyGrid_Vision->AppendIn( ms, new wxIntProperty( "Match Threshold", "MatchThreshold", config->ReadLong( "MatchThreshold", 80 ) ) );
    prop->SetAttribute( "Min", 0 );
    prop->SetAttribute( "Max", 100 );
    prop->SetEditor( "SpinCtrl" );
    prop->SetHelpString( "Percentage Threshold For Template Matching" );

    if( m_tempateImg ) {
        wxPGProperty* p = PropertyGrid_Vision->AppendIn( 
            ms, new xlImageProperty( "Template Image",
            "TemplateImage",
            "",
            m_tempateImg.get() ) );
        p->ChangeFlag( wxPG_PROP_READONLY, true );
        p->SetHelpString( "Template Image" );
    }

    prop = PropertyGrid_Vision->AppendIn( ms, new wxIntProperty( "Font Size", "FontSize", config->ReadLong( "FontSize", 8 ) ) );
    prop->SetAttribute( "Min", 1 );
    prop->SetAttribute( "Max", 100 );
    prop->SetEditor( "SpinCtrl" );
    prop->SetHelpString( "Font Size of Node Labels" );

    prop = PropertyGrid_Vision->AppendIn( ms, new wxIntProperty( "Font Thickness", "FontThickness", config->ReadLong( "FontThickness", 2 ) ) );
    prop->SetAttribute( "Min", 1 );
    prop->SetAttribute( "Max", 10 );
    prop->SetEditor( "SpinCtrl" );
    prop->SetHelpString( "Doesn't Really do Much" );

    wxPGProperty* es = PropertyGrid_Vision->Append( new wxPropertyCategory( "Export Setting", "ExportSetting" ) );

    prop = PropertyGrid_Vision->AppendIn( es, new wxIntProperty( "Model Width", "ModelWidth", config->ReadLong( "ModelWidth", 100 ) ) );
    prop->SetAttribute( "Min", 10 );
    prop->SetAttribute( "Max", 1000 );
    prop->SetEditor( "SpinCtrl" );

    prop = PropertyGrid_Vision->AppendIn( es, new wxIntProperty( "Model Height", "ModelHeight", config->ReadLong( "ModelHeight", 100 ) ) );
    prop->SetAttribute( "Min", 10 );
    prop->SetAttribute( "Max", 1000 );
    prop->SetEditor( "SpinCtrl" );

    //prop = PropertyGrid_Vision->Append( new wxBoolProperty( "Trim White Space", "TrimWhiteSpace", config->ReadBool( "TrimWhiteSpace", true ) ) );
    //prop->SetAttribute( "UseCheckbox", true );

    wxPGProperty* aw = PropertyGrid_Vision->Append( new wxPropertyCategory( "Auto Wire Setting", "AutoWireSetting" ) );

    prop = PropertyGrid_Vision->AppendIn( aw, new wxIntProperty( "Max Wiring Jump", "MaxWiringJump", config->ReadLong( "MaxWiringJump", 100 ) ) );
    prop->SetAttribute( "Min", 1 );
    prop->SetAttribute( "Max", 10000 );
    prop->SetEditor( "SpinCtrl" );
    prop->SetHelpString( "Spacing in Image Pixel Resolution of Acceptable Wire Jumps" );

    PropertyGrid_Vision->Thaw();
}

xModelGenFrame::~xModelGenFrame()
{
    //(*Destroy(xModelGenFrame)
    //*)
}

void xModelGenFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void xModelGenFrame::OnAbout(wxCommandEvent& event)
{
    auto about = wxString::Format(wxT("xModelGen v%s, the xModel Generation App."), GetDisplayVersionString());
    wxMessageBox(about, _("Welcome to..."));
}

void xModelGenFrame::ValidateWindow()
{
}

void xModelGenFrame::OnResize(wxSizeEvent& event)
{
    Layout();
}

void xModelGenFrame::CreateDebugReport(wxDebugReportCompress *report) {
    if (wxDebugReportPreviewStd().Show(*report)) {
        report->Process();
        SendReport("crashUpload", *report);
        wxMessageBox("Crash report saved to " + report->GetCompressedFileName());
    }
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.crit("Exiting after creating debug report: " + report->GetCompressedFileName());
    delete report;
    exit(1);
}

void xModelGenFrame::SendReport(const wxString &loc, wxDebugReportCompress &report) {
    wxHTTP http;
    http.Connect("dankulp.com");

    const char *bound = "--------------------------b29a7c2fe47b9481";

    wxDateTime now = wxDateTime::Now();
    int millis = wxGetUTCTimeMillis().GetLo() % 1000;
    wxString ts = wxString::Format("%04d-%02d-%02d_%02d-%02d-%02d-%03d", now.GetYear(), now.GetMonth()+1, now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond(), millis);

    wxString fn = wxString::Format("xModelGen-%s_%s_%s_%s.zip", wxPlatformInfo::Get().GetOperatingSystemFamilyName().c_str(), xlights_version_string, GetBitness(), ts);
    const char *ct = "Content-Type: application/octet-stream\n";
    std::string cd = "Content-Disposition: form-data; name=\"userfile\"; filename=\"" + fn.ToStdString() + "\"\n\n";

    wxMemoryBuffer memBuff;
    memBuff.AppendData(bound, strlen(bound));
    memBuff.AppendData("\n", 1);
    memBuff.AppendData(ct, strlen(ct));
    memBuff.AppendData(cd.c_str(), strlen(cd.c_str()));

    wxFile f_in(report.GetCompressedFileName());
    wxFileOffset fLen = f_in.Length();
    void* tmp = memBuff.GetAppendBuf(fLen);
    size_t iRead = f_in.Read(tmp, fLen);
    memBuff.UngetAppendBuf(iRead);
    f_in.Close();

    memBuff.AppendData("\n", 1);
    memBuff.AppendData(bound, strlen(bound));
    memBuff.AppendData("--\n", 3);

    http.SetMethod("POST");
    http.SetPostBuffer("multipart/form-data; boundary=------------------------b29a7c2fe47b9481", memBuff);
    wxInputStream * is = http.GetInputStream("/" + loc + "/index.php");
    char buf[1024];
    is->Read(buf, 1024);
    //printf("%s\n", buf);
    delete is;
    http.Close();
}

void xModelGenFrame::OnMenuItem_LogSelected(wxCommandEvent& event)
{
    static log4cpp::Category& logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    wxString dir;
    wxString fileName = "xModelGen_l4cpp.log";
#ifdef __WXMSW__
    wxGetEnv("APPDATA", &dir);
    if (dir.EndsWith("/") || dir.EndsWith("\\"))
    {
        dir = dir.Left(dir.Length() - 1);
    }
    wxString filename = dir + "/" + fileName;
#endif
#ifdef __WXOSX__
    wxFileName home;
    home.AssignHomeDir();
    dir = home.GetFullPath();
    if (dir.EndsWith("/"))
    {
        dir = dir.Left(dir.Length() - 1);
    }
    wxString filename = dir + "/Library/Logs/" + fileName;
#endif
#ifdef __LINUX__
    wxString filename = "/tmp/" + fileName;
#endif
    wxString fn = "";
    if (wxFile::Exists(filename))
    {
        fn = filename;
    }
    /*else if (wxFile::Exists(wxFileName(_showDir, fileName).GetFullPath()))
    {
        fn = wxFileName(_showDir, fileName).GetFullPath();
    }*/
    else if (wxFile::Exists(wxFileName(wxGetCwd(), fileName).GetFullPath()))
    {
        fn = wxFileName(wxGetCwd(), fileName).GetFullPath();
    }

    wxFileType* ft = wxTheMimeTypesManager->GetFileTypeFromExtension("txt");
    if (fn != "" && ft)
    {
        wxString command = ft->GetOpenCommand("foo.txt");
        command.Replace("foo.txt", fn);

        logger_base.debug("Viewing log file %s.", (const char*)fn.c_str());

        wxUnsetEnv("LD_PRELOAD");
        wxExecute(command);
        delete ft;
    }
    else
    {
        DisplayError(wxString::Format("Unable to show log file '%s'.", fn).ToStdString(), this);
    }
}

void xModelGenFrame::OnMenuItem_Load_DxfSelected(wxCommandEvent& event)
{
    wxFileDialog dlg(this, "Load DXF", wxEmptyString, wxEmptyString, "Mapping Files (*.dxf)|*.dxf|All Files (*.)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK) {
        dxf_reader reader;
        if (reader.openFile(dlg.GetPath().ToStdString())) {
            m_dxf_data = reader.moveData();
            Load_Dxf_Items( dlg.GetPath().ToStdString() );
        }
        else {
            DisplayError("Failed to Read DXF: " + dlg.GetPath(), this);
        }
    }
}

void xModelGenFrame::OnMenuItem_Auto_WireSelected(wxCommandEvent& event)
{
}

void xModelGenFrame::OnMenuItem_Save_XModelSelected(wxCommandEvent& event)
{
    if(!m_model){
        DisplayError( "No Model Was Loaded", this );
        return;
    }
    if( m_model->GetNodeCount() == 0 ) {
        DisplayError( "No Nodes Were Generated", this );
        return;
    }
    wxFileDialog dlg( this, "Save xModel File", wxEmptyString, m_model->GetName(), "xModel Files (*.xmodel)|*.xmodel", wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

    if (dlg.ShowModal() == wxID_OK) {
        wxConfigBase* config = wxConfigBase::Get();
        int grid_width = config->ReadLong( "ModelWidth", 100 );
        int grid_height = config->ReadLong( "ModelHeight", 100 );
        m_model->ExportModel( dlg.GetPath(), grid_width, grid_height);
    }
}

void xModelGenFrame::OnListBoxNodesDClick(wxCommandEvent& event)
{
    ListBoxNodes->GetSelection();
    PanelPictureView->SetSelection( ListBoxNodes->GetSelection(), true);
}

void xModelGenFrame::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void xModelGenFrame::OnGrid_NodesLeftDClick(wxMouseEvent& event)
{
}

void xModelGenFrame::Load_Dxf_Items( std::string const& filePath )
{
    if( !m_dxf_data ) {
        return;
    }

    std::filesystem::path p = filePath;

    std::string name = p.filename().replace_extension().string();

    m_model = std::make_unique< Model >();

    m_model->SetName(name);
    int minX    = INT32_MAX;
    int minY    = INT32_MAX;
    int maxX    = 0;
    int maxY    = 0;
    /*
    foreach( LwPolyline entity in _doc.LwPolylines ) {
        if( !entity.IsClosed )
            continue;
        int count = entity.Vertexes.Count;
        if( count == 10 ) {
            double minNodeX = 10000000.0;
            double minNodeY = 10000000.0;
            double maxNodeX = 0.0;
            double maxNodeY = 0.0;

            foreach( LwPolylineVertex pt in entity.Vertexes ) {
                if( pt.Position.X < minNodeX )
                    minNodeX = pt.Position.X;
                if( pt.Position.Y < minNodeY )
                    minNodeY = pt.Position.Y;

                if( pt.Position.X > maxNodeX )
                    maxNodeX = pt.Position.X;
                if( pt.Position.Y > maxNodeY )
                    maxNodeY = pt.Position.Y;
            }

            double dist = maxNodeX - minNodeX;
            if( 0.4 > dist || dist > 0.7 )
                continue;

            double centerX = ( maxNodeX + minNodeX ) / 2.0;
            double centerY = ( maxNodeY + minNodeY ) / 2.0;

            int t = count;

            var newX = (int)( centerX * 2.0 );
            var newY = (int)( centerY * 2.0 );

            if( newX < minX )
                minX = newX - 1;
            if( newY < minY )
                minY = newY - 1;

            if( newX > maxX )
                maxX = newX + 1;
            if( newY > maxY )
                maxY = newY + 1;

            var newNode = new Node{
                GridX = newX,
                GridY = newY
            };
            _model.AddNode( newNode );
        }
    }
    */

    for( auto const& c : m_dxf_data->circles ) {
        //dc.DrawCircle( c.cx, c.cy, c.radius );
        //pixel hole are about 0.5 inches or 0.25
        if( 0.2 > c.radius || c.radius > 0.3 ) {
            continue;
        }

        int newX = (int)( c.cx * 2.0 );
        int newY = (int)( c.cy * 2.0 );

        if( newX < minX ) {
            minX = newX - 1;
        }
        if( newY < minY ) {
            minY = newY - 1;
        }

        if( newX > maxX ) {
            maxX = newX + 1;
        }
        if( newY > maxY ) {
            maxY = newY + 1;
        }

        //Node newNode(newX, newY );
        //m_model->AddNode( newNode );
    }

    for( auto const& l : m_dxf_data->lines ) {
        //dc.DrawLine( l.x1, l.y1, l.x2, l.y2 );
    }

    for( auto const& t : m_dxf_data->texts ) {
        //dc.SetClippingRegion( pt, size );
        //dc.DrawText( t.text, t.height, t.xScaleFactor );
        //dc.DestroyClippingRegion();
    }

    minX = std::max( minX, 0 );
    minY = std::max( minY, 0 );
    //m_model->SetBoundingBox( minX, maxX, minY, maxY );
}

void xModelGenFrame::OnMenuItem_Load_PictureSelected(wxCommandEvent& event)
{
    /*
    * OpenCV supported formats
    * Windows bitmaps - *.bmp, *.dib (always supported)
    * JPEG files - *.jpeg, *.jpg, *.jpe (see the Note section)
    * JPEG 2000 files - *.jp2 (see the Note section)
    * Portable Network Graphics - *.png (see the Note section)
    * WebP - *.webp (see the Note section)
    * Portable image format - *.pbm, *.pgm, *.ppm *.pxm, *.pnm (always supported)
    * PFM files - *.pfm (see the Note section)
    * Sun rasters - *.sr, *.ras (always supported)
    * TIFF files - *.tiff, *.tif (see the Note section)
    * OpenEXR Image files - *.exr (see the Note s
    */
    //"(*.bmp;*.jpeg;*.jpg;*.png;*.webp;*.pbm;*.tiff;*.tif)|*.bmp;*.jpeg;*.jpg;*.png;*.webp;*.pbm;*.tiff;*.tif"
    wxFileDialog dlg( this, "Load Picture", wxEmptyString, wxEmptyString, "Image Files (*.bmp;*.jpeg;*.jpg;*.png;*.webp;*.pbm;*.tiff;*.tif)|*.bmp;*.jpeg;*.jpg;*.png;*.webp;*.pbm;*.tiff;*.tif|All Files (*.)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST );
    if( dlg.ShowModal() == wxID_OK ) {
        std::filesystem::path p = dlg.GetPath().ToStdString();

        std::string name = p.filename().replace_extension().string();

        m_model = std::make_unique< Model >();

        m_model->SetName( name );
        PanelPictureView->LoadPicture( dlg.GetPath().ToStdString(), m_model.get() );
    }
}

void xModelGenFrame::OnButtonSelectClick(wxCommandEvent& event)
{
    m_Selecting = true;
}

void xModelGenFrame::OnPanelPictureViewLeftDown(wxMouseEvent& event)
{
    if( m_Selecting ) {
        wxPoint start = event.GetPosition();
        m_startPoint  = start;
        return;
    }

    if (CheckBoxEnableWiring->IsChecked()) {
        int number = SpinCtrlNextNodeNumber->GetValue();
        if(PanelPictureView->SetNodeNumber(event.GetPosition(), number)) {
            SpinCtrlNextNodeNumber->SetValue(++number);
            RefreshNodes();
        }
    }
}

void xModelGenFrame::OnPanelPictureViewLeftUp(wxMouseEvent& event)
{
    if( !m_Selecting ) {
        return;
    }
    if( PanelPictureView->SaveTemplate( m_startPoint, event.GetPosition() ) ) {
        if( auto str = PanelPictureView->GetTemplate( ) ) {
            m_tempateImg = std::make_unique< wxImage >( *str );
            DrawPropertyGrid();
        }
        RefreshNodes();
    }
    m_Selecting = false;
}

void xModelGenFrame::OnPanelPictureViewLeftDClick(wxMouseEvent& event)
{
    //set Node Number
    int number = SpinCtrlNextNodeNumber->GetValue();
    if(PanelPictureView->SetNodeNumber(event.GetPosition(), number)) {
        SpinCtrlNextNodeNumber->SetValue(++number);
        RefreshNodes();
    }
}

void xModelGenFrame::OnPanelPictureViewRightDown(wxMouseEvent& event)
{
    wxMenu mnuLayer;
    mnuLayer.Append(ID_MNU_ADD_NODE, "Add Node");
    mnuLayer.Append(ID_MNU_CLEAR_SELECTION, "Clear Selection");
    mnuLayer.Append(ID_MNU_CLEAR, "Clear Wiring");
    mnuLayer.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&xModelGenFrame::OnPopup, nullptr, this);
    PopupMenu(&mnuLayer);
}

void xModelGenFrame::OnPopup(wxCommandEvent& event)
{
    if (event.GetId() == ID_MNU_ADD_NODE) {
        if(m_model) {
            //TODO
            RefreshNodes();
        }
    }
    else if (event.GetId() == ID_MNU_CLEAR_SELECTION) {
            PanelPictureView->ClearSelection(true);
    }
    else if (event.GetId() == ID_MNU_CLEAR) {
        if(m_model) {
            m_model->ClearWiring();
            PanelPictureView->DrawPicture();
            RefreshNodes();
        }
    }
}

void xModelGenFrame::OnPropertyGridChange( wxPropertyGridEvent& event )
{
    wxString name    = event.GetPropertyName();
    //updatingProperty = true;
    if( name == "MatchThreshold" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "MatchThreshold", event.GetValue().GetLong() );
    }else if( name == "FontSize" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "FontSize", event.GetValue().GetLong() );
        PanelPictureView->DrawPicture();
    }else if( name == "FontThickness" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "FontThickness", event.GetValue().GetLong() );
        PanelPictureView->DrawPicture();
    } else if( name == "ModelWidth" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "ModelWidth", event.GetValue().GetLong() );
    } else if( name == "ModelHeight" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "ModelHeight", event.GetValue().GetLong() );
    } else if( name == "MaxWiringJump" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "MaxWiringJump", event.GetValue().GetLong() );
    } else if( name == "TrimWhiteSpace" ) {
        wxConfigBase* config = wxConfigBase::Get();
        config->Write( "TrimWhiteSpace", event.GetValue().GetBool() );
    }
}

void xModelGenFrame::RefreshNodes()
{
    ListBoxNodes->Clear();

    for( auto const& node : m_model->GetNodes() ) {
        ListBoxNodes->Append( node.GetText() );
    }

    StaticTextNodes->SetLabelText( wxString::Format( "%zu Nodes Found", m_model->GetNodeCount() ) );
}
