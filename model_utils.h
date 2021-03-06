#ifndef MODEL_UTILES_H
#define MODEL_UTILES_H

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

#include <cmath>

namespace model_utils
{
    double getDistance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow (x2 - x1, 2) + pow (y2 - y1, 2));
    }
    double getDistance(node first, node second)
    {
        return getDistance(first.gridX, first.gridY, second.gridX, second.gridY);
    }
}

#endif