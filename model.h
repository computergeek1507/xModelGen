#ifndef MODELUTILES_H
#define MODELUTILES_H

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "node.h"

#include <string>
#include <vector>
#include <optional>
#include <cmath>

class model
{
public:
    model();
    ~model() {}

    void clearWiring();

    void exportModel( std::string const& filename );

    void setBoundingBox( int minX, int maxX, int minY, int maxY );

    void addNode( node _node );

    void SortNodes();

    [[nodiscard]] std::optional< std::reference_wrapper< node > > GetNode( int i )
    {
        if (i > m_nodes.size()) {
            return std::nullopt;
        }
        return m_nodes.at( i );
    }


    [[nodiscard]] std::vector< node > const& GetNodes() const
    {
        return m_nodes;
    }

    [[nodiscard]] int GetNodeCount() const { return m_nodes.size(); }

    [[nodiscard]] std::optional< std::reference_wrapper< node > > FindNode( int x, int y )
    {
        if( auto found{ std::find_if( m_nodes.begin(), m_nodes.end(), [ &x, &y ]( node const& n ) { return n.gridX == x && n.gridY == y; } ) };
            found != m_nodes.end() ) {
            return *found;
        }

        return std::nullopt;
    }

    [[nodiscard]] int FindNodeIndex( int x, int y ) const
    {
        int i = 0;
        for( auto node : m_nodes ) {
            if( x == node.gridX && y == node.gridY ) {
                return i;
            }
            i++;
        }
        return -1;
    }

    void DeleteNode( int x, int y )
    {
        int idx = FindNodeIndex( x, y );
        if( idx == -1 ) {
            return;
        }

        m_nodes.erase( m_nodes.begin() + idx );
    }

    void SetNodeNumber( int x, int y, int node )
    {
        int idx = FindNodeIndex( x, y );
        if( idx == -1 ) {
            return;
        }
        m_nodes.at( idx ).nodeNumber = node;
    }

    void SetNodeNumber( int i, int node )
    {
        if( i > m_nodes.size() ) {
            return;
        }
        m_nodes.at( i ).nodeNumber = node;
    }

    void setName( std::string name ) { m_name = std::move( name ); }
    [[nodiscard]] std::string const& getName() const { return m_name; }

private:
    std::vector<node> m_nodes;
    std::string m_name;
    int m_sizeX;
    int m_sizeY;

};
#endif