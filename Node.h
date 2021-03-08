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

struct Node 
{
    Node()
    { }

    Node(int x, int y ) :
        X( x ), Y( y )
    {}

    inline bool operator==( const Node& lhs) { return lhs.X == X && lhs.Y == Y; }


    int X{-1};
    int Y{-1};
    int GridX{ -1 };
    int GridY{ -1 };
    int NodeNumber{0};

    [[nodiscard]] bool IsWired() const { return NodeNumber != 0; }

    [[nodiscard]] std::string GetText() const
    {
        return "Node:" + std::to_string( NodeNumber ) + "     X:" + std::to_string( X ) + "     Y:" + std::to_string( Y );
    }

    void ClearWiring()
    {
        NodeNumber = 0;
    }
};

#endif