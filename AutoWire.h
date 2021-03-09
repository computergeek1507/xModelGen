#ifndef AUTOWIRE_H
#define AUTOWIRE_H

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
#include <vector>
#include <cmath>

class Model;
struct Node;

class AutoWire
{
public:
    AutoWire( Model* model, int wireGap ) :
        m_model( model ), m_wireGap( wireGap )
    { }

    void WireModel( int startX, int startY );

    [[nodiscard]] bool GetWorked() { return m_worked; }

    [[nodiscard]] std::vector< int > GetIndexes() { return m_doneIndexs; }

private:
    Model* m_model;
    int m_wireGap{ 5 };
    std::vector< int > m_doneIndexs;
    bool m_worked { false };

    void WireNode( std::vector< Node > nodes, int count, std::vector< int > wiredIndex );
};
#endif