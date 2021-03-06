#ifndef NODE_H
#define NODE_H

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include <string>

struct node 
{
    node(): 
        nodeNumber(0), gridX(-1), gridY(-1)
    { }

    node(int x, int y ) :
        nodeNumber( 0 ), gridX( x ), gridY( y )
    {}

    inline bool operator==( const node& lhs) { return lhs.gridX == gridX && lhs.gridY == gridY; }


    int gridX;
    int gridY;
    int nodeNumber;

    [[nodiscard]] bool isWired() const { return nodeNumber != 0; }

    [[nodiscard]] std::string getText() const
    {
        return "Node:" + std::to_string( nodeNumber ) + "     X:" + std::to_string( gridX ) + "     Y:" + std::to_string( gridY );
    }

    void clearWiring()
    {
        nodeNumber = 0;
    }
};

#endif