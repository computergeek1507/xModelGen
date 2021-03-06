// system header
#include <cmath>
#include <cstdio>

#include <sys/timeb.h>

#include "wx/wxprec.h"
#include <wx/image.h>

#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


// main header
#include "CVPictureView.h"

// implement message map
BEGIN_EVENT_TABLE(CVPictureView, wxWindow)
				EVT_PAINT(CVPictureView::OnPaint)
				EVT_SIZE(CVPictureView::OnSize)
END_EVENT_TABLE()

CVPictureView::CVPictureView( wxWindow* frame, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, wxString const& name ) :
    wxWindow( frame, id, pos, size, wxSIMPLE_BORDER )
{
	// set my canvas width/height
	m_nWidth = size.GetWidth();
	m_nHeight = size.GetHeight();
}

void CVPictureView::LoadPicture( std::string const& file )
{
    m_filePath = file;
    m_frame    = cv::imread( m_filePath, cv::IMREAD_COLOR );
    DrawPicture();
}

void CVPictureView::SaveTemplate( wxPoint start, wxPoint end )
{
    if( start == end ) {
        return;
    }
    wxPoint newStart = start; //ScreenToClient( start );
    wxPoint newEnd   = end;   //ScreenToClient( end );

    wxPoint scale_start = wxPoint( newStart.x / m_scale, newStart.y / m_scale );
    wxPoint scale_end   = wxPoint( newEnd.x / m_scale, newEnd.y / m_scale );

    int width = abs(scale_start.x - scale_end.x);
    int height = abs(scale_start.y - scale_end.y);

    //int newy = m_frame.rows - scale_start.y;

    auto rect = cv::Rect( scale_start.x, scale_start.y, width, height );

    cv::Mat image_roi = m_frame( rect );
    cv::imshow( "canvasOutput", image_roi.clone() );

    //cv::imwrite( "D:\\Test.png", image_roi );

    m_template = image_roi.clone();

    DrawPicture();
}

void CVPictureView::SetFocus( int focus )
{
    m_focus = focus;
    //m_pCamera.set( cv::CAP_PROP_FOCUS, focus );
}

void CVPictureView::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	Draw(dc);
}

void CVPictureView::Draw(wxDC &dc)
{
	if (!dc.IsOk()) {
        return;
    }

	wxBitmap bitmap;

	imageMutex_.Lock();
    if( m_image.IsOk() )
	{
        int const width = m_image.GetWidth();
        int const height = m_image.GetHeight();
        
		double x_Scale = (double)m_nWidth / (double)(width);
        double y_Scale = (double)m_nHeight / (double)height;
        m_scale   = std::min( x_Scale, y_Scale );
        wxBitmap bitmap = wxBitmap( m_image.Scale( width * m_scale, height * m_scale ) );

		int x, y, w, h;
        dc.GetClippingBox( &x, &y, &w, &h );
        dc.DrawBitmap( bitmap, x, y );
	} 
	imageMutex_.Unlock();
}

void CVPictureView::FindTemplateInPic()
{
    if( !m_template.empty() ) {

        if( m_frame.empty() ) {
            return;
        }
        cv::Mat ogFrame = m_frame.clone();

        cv::Mat res_32f( ogFrame.rows - m_template.rows + 1, ogFrame.cols - m_template.cols + 1, CV_32FC1 );
        cv::matchTemplate( ogFrame, m_template, res_32f, cv::TM_CCOEFF_NORMED );

        cv::Mat res;
        res_32f.convertTo( res, CV_8U, 255.0 );

        int size = ( ( m_template.cols + m_template.rows ) / 4 ) * 2 + 1; //force size to be odd
        adaptiveThreshold( res, res, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, size, -128 );
        //imshow( "result_thresh", res );
        double threshold = 0.8;

        for( int i =0; i < 2000; ++i ) {
            double minval, maxval;
            cv::Point minloc, maxloc;
            minMaxLoc( res, &minval, &maxval, &minloc, &maxloc );

            if( maxval >= threshold ) {
                rectangle( ogFrame, maxloc, cv::Point( maxloc.x + m_template.cols, maxloc.y + m_template.rows ), CV_RGB( 0, 255, 0 ), 2 );
                floodFill( res, maxloc, 0 ); //mark drawn blob
            } else {
                break;
            }
        }
    }
}

void CVPictureView::DrawPicture()
{
    if( m_frame.empty() ) {
        return;
    }
    cv::Mat ogFrame = m_frame.clone();

    int nCamWidth  = ogFrame.cols;
    int nCamHeight = ogFrame.rows;

    if( !m_template.empty() ) {
    
        cv::Mat res_32f( ogFrame.rows - m_template.rows + 1, ogFrame.cols - m_template.cols + 1, CV_32FC1 );
        cv::matchTemplate( ogFrame, m_template, res_32f, cv::TM_CCOEFF_NORMED );

        cv::Mat res;
        res_32f.convertTo( res, CV_8U, 255.0 );

        int size = ( ( m_template.cols + m_template.rows ) / 4 ) * 2 + 1; //force size to be odd
        adaptiveThreshold( res, res, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, size, -128 );
        double threshold = 0.8;

        for( ;; ) {
            double minval, maxval;
            cv::Point minloc, maxloc;
            minMaxLoc( res, &minval, &maxval, &minloc, &maxloc );

            if( maxval >= threshold ) {
                rectangle( ogFrame, maxloc, cv::Point( maxloc.x + m_template.cols, maxloc.y + m_template.rows ), CV_RGB( 0, 255, 0 ), 2 );
                floodFill( res, maxloc, 0 ); //mark drawn blob
            } else {
                break;
            }
        }
    }


    if( !m_foundPoints.empty() ) {
        for( auto const& key : m_foundPoints ) {
            cv::drawMarker( ogFrame, cv::Point( key.x, key.y ),
                            cv::Scalar( 0, 0, 255 ) );
        }
    }

    wxImage tmpImage = wx_from_mat( ogFrame );
    //wxImage tmpImage( nCamWidth, nCamHeight, ogFrame.data, true );

    imageMutex_.Lock();
    m_image        = tmpImage.Copy();
    imageMutex_.Unlock();

    Refresh( false );
    //wxPaintEvent event(wxEVT_PAINT);
    //wxPostEvent(this, event);
}

wxImage CVPictureView::wx_from_mat( cv::Mat& img )
{
     cv::Mat im2;
    if( img.channels() == 1 ) {
         cvtColor( img, im2, cv::COLOR_GRAY2RGB );
    } else if( img.channels() == 4 ) {
        cvtColor( img, im2, cv::COLOR_BGRA2RGB );
    } else {
        cvtColor( img, im2, cv::COLOR_BGR2RGB );
    }
    long imsize = im2.rows * im2.cols * im2.channels();
    wxImage wx( im2.cols, im2.rows, (unsigned char*)malloc( imsize ), false );
    unsigned char* s = im2.data;
    unsigned char* d = wx.GetData();
    for( long i = 0; i < imsize; i++ ) {
        d[ i ] = s[ i ];
    }
    return wx;
}

void CVPictureView::OnSize(wxSizeEvent &event)
{
	int nWidth = event.GetSize().GetWidth();
	int nHeight = event.GetSize().GetHeight();

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	Refresh();
	event.Skip();
}
