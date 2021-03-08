#ifndef CVPICTUREVIEW_H
#define CVPICTUREVIEW_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include "wx/wxprec.h"
#include "wx/wx.h"

#include <memory>
#include <vector>
#include <optional>

class Model;

class CVPictureView : public wxWindow
{
public: 
	CVPictureView( wxWindow* frame, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, wxString const& name );
    virtual ~CVPictureView() = default;

    void LoadPicture( std::string const& file, Model* model );

	void DrawPicture();

    bool SaveTemplate(wxPoint start, wxPoint end);

	[[nodiscard]] std::optional< wxImage > GetTemplate();

    bool FindTemplateInPic();

	bool SetNodeNumber( wxPoint point, int nodeNum );

	void SetSelection(int selection, bool redraw = false);

	void ClearSelection(bool redraw = false){ SetSelection(-1, redraw); }

	[[nodiscard]] wxPoint ScaleXY( wxPoint const& point ) const;

	void Draw( wxDC& dc );

private:
    Model* m_model;

	wxImage m_image;
    std::string m_filePath;

	cv::Mat m_frame;
    cv::Mat m_template;

	int m_nWidth;
	int m_nHeight;
    double m_scale{ 1.0 };

	int m_selected{-1};

	wxMutex  imageMutex_;

    wxImage wx_from_mat( cv::Mat& img );

	void OnPaint( wxPaintEvent& event );
    void OnSize( wxSizeEvent& even );

public:


protected:

	DECLARE_EVENT_TABLE()
};

#endif

