#include "CVPictureView.h"

#include "Model.h"

#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

#include "wx/wxprec.h"
#include <wx/image.h>
#include <wx/confbase.h>

#include <sys/timeb.h>

// system header
#include <cmath>
#include <cstdio>

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

void CVPictureView::LoadPicture( std::string const& file, Model * model )
{
    m_model    = model;
    m_filePath = file;
    m_frame    = cv::imread( m_filePath, cv::IMREAD_COLOR );
    ClearSelection();
    DrawPicture();
}

bool CVPictureView::SaveTemplate( wxPoint start, wxPoint end )
{
    if( m_frame.empty() ) {
        return false;
    }

    if( start == end ) {
        return false;
    }
    wxPoint newStart = start; //ScreenToClient( start );
    wxPoint newEnd   = end;   //ScreenToClient( end );

    wxPoint scale_start = ScaleXY( newStart );
    wxPoint scale_end   = ScaleXY( newEnd );

    int width = std::abs(scale_start.x - scale_end.x);
    int height = std::abs( scale_start.y - scale_end.y );

    //int newy = m_frame.rows - scale_start.y;

    auto rect = cv::Rect( scale_start.x, scale_start.y, width, height );

    cv::Mat image_roi = m_frame( rect );
    //cv::imshow( "canvasOutput", image_roi.clone() );

    //cv::imwrite( "D:\\Test.png", image_roi );

    m_template = image_roi.clone();
    ClearSelection();
    bool found = FindTemplateInPic();

    DrawPicture();
    return found;
}

void CVPictureView::SetSelection(int selection, bool redraw)
{
    if(m_selected == selection) {
        m_selected = -1;
    }else {
        m_selected = selection;
    }
    if(redraw) {
        DrawPicture();
    }
}

bool CVPictureView::SetNodeNumber(wxPoint point, int nodeNum ) {
    if(!m_model) {
        return false;
    }

    if(m_template.empty()) {
        return false;
    }

    if( m_model->GetNodeCount() == 0 ) {
        return false;
    }
    ClearSelection();

    wxPoint scaled_p = ScaleXY( point );

    for( auto & node : m_model->GetEditNodes() ) {
        if ((std::abs(scaled_p.x - node.X) < (m_template.cols/2)) && 
            (std::abs(scaled_p.y - node.Y) < (m_template.rows/2) ))
        {
            node.NodeNumber = nodeNum;
            DrawPicture();
            return true;
        }
    }

    return true;
}

wxPoint CVPictureView::ScaleXY( wxPoint const& point ) const
{
    return wxPoint( point.x / m_scale, point.y / m_scale );
}

bool CVPictureView::FindTemplateInPic()
{
    if( !m_template.empty() ) {

        if( m_frame.empty() ) {
            return false;
        }

        if( !m_model ) {
            return false;
        }

        m_model->ClearNodes();

        cv::Mat ogFrame = m_frame.clone();

        cv::Mat res_32f( ogFrame.rows - m_template.rows + 1, ogFrame.cols - m_template.cols + 1, CV_32FC1 );
        cv::matchTemplate( ogFrame, m_template, res_32f, cv::TM_CCOEFF_NORMED );

        cv::Mat res;
        res_32f.convertTo( res, CV_8U, 255.0 );

        int size = ( ( m_template.cols + m_template.rows ) / 4 ) * 2 + 1; //force size to be odd
        cv::adaptiveThreshold( res, res, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, size, -128 );
        //imshow( "result_thresh", res );

         wxConfigBase* config = wxConfigBase::Get();

        int iThreshold = config->ReadLong( "MatchThreshold", 80 );

        double threshold = iThreshold/100.0;

        for( int i =0; i < 2000; ++i ) {
            double minval, maxval;
            cv::Point minloc, maxloc;
            cv::minMaxLoc( res, &minval, &maxval, &minloc, &maxloc );

            if( maxval >= threshold ) {
                //cv::rectangle( ogFrame, maxloc, cv::Point( maxloc.x + m_template.cols, maxloc.y + m_template.rows ), CV_RGB( 0, 255, 0 ), 2 );
                cv::floodFill( res, maxloc, 0 ); //mark drawn blob
                m_model->AddNode(Node( maxloc.x + ( m_template.cols / 2 ), maxloc.y + ( m_template.rows / 2 ) ));
            } else {
                break;
            }
        }
        return true;
        //m_mainFrame->RefreshNodes();
    }
    return false;
}

std::optional< wxImage > CVPictureView::GetTemplate()
{
    if( m_frame.empty() ) {
        return {};
    }
    return wx_from_mat( m_template );
}

void CVPictureView::DrawPicture()
{
    if( m_frame.empty() ) {
        return;
    }
    cv::Mat ogFrame = m_frame.clone();

    if( m_model && m_model->GetNodeCount() != 0 ) {
        wxConfigBase* config = wxConfigBase::Get();
        int font_size = config->ReadLong( "FontSize", 8 );
        int font_thick = config->ReadLong( "FontThickness", 2 );

        int index = 0;
        for( auto const& node : m_model->GetNodes() ) {
            cv::drawMarker( ogFrame, cv::Point( node.X, node.Y ),
                            cv::Scalar( 0, 0, 255 ) );
            int start_X = node.X - ( m_template.cols / 2 );
            int start_Y = node.Y - ( m_template.rows / 2 );
            int end_X = node.X + ( m_template.cols / 2 );
            int end_Y = node.Y + ( m_template.rows / 2 );
            start_X     = std::max( 0, start_X );
            start_Y     = std::max( 0, start_Y );
            end_X       = std::min( ogFrame.cols, end_X );
            end_Y       = std::min( ogFrame.rows, end_Y );

            auto color = CV_RGB(255, 0, 0 );
            if( node.IsWired() ) {
                cv::String mess( std::to_string( node.NodeNumber ) );

                cv::putText( ogFrame, mess, cv::Point( node.X, node.Y ), cv::FONT_HERSHEY_PLAIN, font_size, cv::Scalar( 0, 255, 0 ), 2, cv::LINE_8, false );
                color = CV_RGB(0, 255, 0 );
            }
            int thickness = 2;
            if(index == m_selected){
                 color = CV_RGB(0, 0, 255 );
                 thickness = 5;
            }
            cv::rectangle( ogFrame, cv::Point( start_X, start_Y ), cv::Point( end_X, end_Y ), color, thickness );
            ++index;
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

    //wxImage wx( im2.cols, im2.rows, im2.data, false );
    return wx;
}

void CVPictureView::OnPaint( wxPaintEvent& event )
{
    wxPaintDC dc( this );
    Draw( dc );
}

void CVPictureView::Draw( wxDC& dc )
{
    if( !dc.IsOk() ) {
        return;
    }

    wxBitmap bitmap;

    imageMutex_.Lock();
    if( m_image.IsOk() ) {
        int const width  = m_image.GetWidth();
        int const height = m_image.GetHeight();

        double x_Scale  = (double)m_nWidth / (double)( width );
        double y_Scale  = (double)m_nHeight / (double)height;
        m_scale         = std::min( x_Scale, y_Scale );
        wxBitmap bitmap = wxBitmap( m_image.Scale( width * m_scale, height * m_scale ) );

        int x, y, w, h;
        dc.GetClippingBox( &x, &y, &w, &h );
        dc.DrawBitmap( bitmap, x, y );
    }
    imageMutex_.Unlock();
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
