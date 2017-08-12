//
//  support.h
//  Project4
//
//  Created by Grace Y on 3/13/17.
//  Copyright © 2017 CS32. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"
#include <string>

using namespace std;

//overloaded comparison operator for geo coordinates

struct orderedNode{
    
};
inline bool operator<(const GeoCoord& me, const GeoCoord& other) {
    if (me.latitudeText < other.latitudeText)
        return true;
    else if (me.latitudeText > other.latitudeText)
        return false;
    //compare longitude
    else{
        if (me.longitudeText < other.longitudeText)
            return true;
        else
            return false;
    }
}

inline bool operator==(const GeoCoord& me, const GeoCoord& other) {
    if (me.latitudeText == other.latitudeText &&
        me.longitudeText == other.longitudeText){
        return true;
    }
    return false;
    
}

inline bool operator!=(const GeoCoord& me, const GeoCoord& other) {
    if (me.latitudeText == other.latitudeText &&
        me.longitudeText == other.longitudeText){
        return false;
    }
    return true;
    
}

inline string directionOfLine(const GeoSegment& gs){
    
    double dirAngle = angleOfLine(gs);
    
    if (0 <= dirAngle && dirAngle <= 22.5){
        return "east";
    }
    else if (22.5 < dirAngle && dirAngle <= 67.5){
        return "northeast";
    }
    else if (67.5 < dirAngle && dirAngle <= 112.5){
        return "north";
    }
    else if (112.5 < dirAngle && dirAngle <= 157.5){
        return "northwest";
    }
    else if (157.5 < dirAngle && dirAngle <= 202.5){
        return "west";
    }
    else if (202.5 < dirAngle && dirAngle <= 247.5){
        return "southwest";
    }
    else if (247.5 < dirAngle && dirAngle <= 292.5){
        return "south";
    }
    else if (292.5 < dirAngle && dirAngle <= 337.5){
        return "southeast";
    }
    else if (337.5 < dirAngle && dirAngle <= 360){
        return "east";
    }
    else
        return "";
}


#endif /* support_h */
