#ifndef CVPICTUREVIEW_H
#define CVPICTUREVIEW_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include "wx/wxprec.h"
#include "wx/wx.h"

#include <memory>
#include <vector>

// external classes

class CVPictureView : public wxWindow
{
public: 
	CVPictureView( wxWindow* frame, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, wxString const& name );
    virtual ~CVPictureView() = default;

    void LoadPicture(std::string const& file);

	void DrawPicture();

    void SaveTemplate(wxPoint start, wxPoint end);

    void FindTemplateInPic();

    void ClearPoints() { m_foundPoints.clear(); }
    void AddPoint( wxPoint point ) { m_foundPoints.emplace_back( point ); }
    void AddPoints( std::vector< wxPoint > points ) 
    { 
        //m_foundPoints.emplace_back( points ); 
        m_foundPoints.insert( m_foundPoints.end(), points.begin(), points.end() );
    }

	void Draw( wxDC& dc );

private:
	wxImage m_image;
    std::string m_filePath;

	cv::Mat m_frame;
    cv::Mat m_template;

	int m_nWidth;
	int m_nHeight;
    double m_scale{ 1.0 };

	int m_blur{11};
    int m_threshold{200};
    int m_erode{2};
    int m_dilate{4};
    int m_greySize{1};
    int m_focus{ 125 };

	std::vector< wxPoint > m_foundPoints;
    std::vector< wxPoint > m_skipPoints;

	wxMutex  imageMutex_;

    wxImage wx_from_mat( cv::Mat& img );

	void OnPaint( wxPaintEvent& event );
    void OnSize( wxSizeEvent& even );

public:
    void SetBlur( int blur ) { m_blur = blur; }
    void SetThreshold( int threshold ) { m_threshold = threshold; }
    void SetErode( int erode ) { m_erode = erode; }
    void SetDilate( int dilate ) { m_dilate = dilate; }
    void SetGreySize( int greySize ) { m_greySize = greySize; }
    void SetFocus( int focus );

protected:

	DECLARE_EVENT_TABLE()
};

#endif

