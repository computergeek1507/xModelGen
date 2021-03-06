#ifndef DXF_DATA_H
#define DXF_DATA_H

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "dl_entities.h"

//container class to store dxf data.
struct dxf_data {

    std::vector<DL_LineData>lines;
    std::vector<DL_CircleData>circles;
    std::vector<DL_ArcData>arcs;
    std::vector<DL_TextData>texts;
};

#endif // DXF_DATA_H
