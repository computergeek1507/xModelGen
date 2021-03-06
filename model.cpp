/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "model.h"

#include <algorithm>
#include <fstream>

model::model() :
    m_sizeX(0),
    m_sizeY(0)
{

}

 void model::addNode( node _node )
{
     if( std::find( m_nodes.begin(), m_nodes.end(), _node ) == m_nodes.end() ) {
        m_nodes.push_back( _node );
     }
 }

void model::SortNodes()
{
     std::sort( std::begin( m_nodes ),
                std::end( m_nodes ),
                []( node a, node b ) { return a.nodeNumber > b.nodeNumber; } );
}

void model::clearWiring()
{
    for( auto& n : m_nodes ) {
        n.clearWiring();
    }
}

void model::setBoundingBox( int minX, int maxX, int minY, int maxY )
{
    auto const width = ( maxX - minX ) + 1;
    auto const heigth = ( maxY - minY ) + 1;

    m_sizeX = width;
    m_sizeY = heigth;

    for( auto & node : m_nodes ) {
        auto scaleX = node.gridX - minX;
        auto scaleY = node.gridY - minY;
        node.gridX = scaleX;
        node.gridY = scaleY;
    }
}

void model::exportModel( std::string const& filename ) 
{
    std::ofstream f;
    f.open( filename.c_str(), std::ios::out );
    if( !f.good() ) {
        return;
    }

    std::string cm;

    for( int x = 0; x <= m_sizeX + 1; x++ ) {
        for( int y = 0; y <= m_sizeY + 1; y++ ) {
            std::string cell;
            
            if( auto node = FindNode( y, x ); node ) {
                if( node->get().isWired() ) {
                    cell = std::to_string( node->get().nodeNumber );
                } else {
                    cell = "1";
                }
            }
            cm += cell + ",";
        }
        cm += ";";
    }

    cm.pop_back();//remove last ";"

    f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<custommodel \n" ;
    f << "name=\"" << m_name << "\" ";
    f << "parm1=\"" << m_sizeX << "\" ";
    f << "parm2=\"" << m_sizeY << "\" ";
    f << "StringType=\"RGB Nodes\" ";
    f <<  "Transparency=\"0\" ";
    f <<  "PixelSize=\"2\" ";
    f <<  "ModelBrightness=\"\" ";
    f <<  "Antialias=\"1\" ";
    f <<  "StrandNames=\"\" ";
    f <<  "NodeNames=\"\" ";

    f <<  "CustomModel=\"";
    f <<  cm ;
    f <<  "\" ";
    f << "SourceVersion=\"2021.5\" ";
    f <<  " >\n";
    f <<  "</custommodel>";

    f.close();
}