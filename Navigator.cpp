#include "provided.h"
#include <string>
#include <vector>
#include "MyMap.h"
#include <queue>
#include "support.h"
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader ml;
    AttractionMapper am;
    SegmentMapper sm;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    //make map loader
    if (ml.load(mapFile) == false){
        return false;
    }
    
    //make attraction mapper
    am.init(ml);
    
    //make segment mapper
    sm.init(ml);
    
	return true;  
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    
    MyMap<GeoCoord, GeoCoord> route; //maps each coordinate to its parent
    queue<GeoCoord> q; //equivalent of "open list"
    vector<GeoCoord> visited; //list of already visited locations
    
    bool endFound = false;
    bool firstTime = true;
    bool segmentDone = false;
    
    //obtain geocoord of starting location and push onto queue
    GeoCoord startCoord;
    GeoCoord endCoord;
    GeoCoord currentCoord;
    am.getGeoCoord(start, startCoord);
    am.getGeoCoord(end, endCoord);
    
    //check for possible bad start and end points
    if (am.getGeoCoord(start, startCoord) == false)
        return NAV_BAD_SOURCE;
    if (am.getGeoCoord(end, endCoord) == false)
        return NAV_BAD_DESTINATION;
    
    if (startCoord == endCoord)
        return NAV_SUCCESS;
    
    q.push(startCoord);
    visited.push_back(startCoord);
    
    //loop
    while (!q.empty()){
        currentCoord = q.front();
        q.pop();
        
        //check if destination is reached
        if (endFound == true || endCoord == currentCoord){
            endFound = true;
            break;
        }
        
        vector<StreetSegment> neighbors = sm.getSegments(currentCoord);
        
        for (int i = 0; i < neighbors.size(); i++){
            if (firstTime){
                q.push(neighbors[i].segment.start);
                q.push(neighbors[i].segment.end);
                route.associate(neighbors[i].segment.start, currentCoord);
                route.associate(neighbors[i].segment.end, currentCoord);
                visited.push_back(neighbors[i].segment.start);
                visited.push_back(neighbors[i].segment.start);

                firstTime = false;
            }
            
            else if (currentCoord == neighbors[i].segment.end){
                for (int j = 0; j < visited.size(); j++){
                    if (visited[j] == neighbors[i].segment.start){
                        segmentDone = true;
                        break;
                    }
                }
                if (segmentDone == false){
                    q.push(neighbors[i].segment.start);
                    visited.push_back(neighbors[i].segment.end);
                    route.associate(neighbors[i].segment.start, currentCoord);
                }
                else
                    segmentDone = false;
                
            }
            else if (currentCoord == neighbors[i].segment.start){
                for (int j = 0; j < visited.size(); j++){
                    if (visited[j] == neighbors[i].segment.end){
                        segmentDone = true;
                        break;
                    }
                }
                if (segmentDone == false){
                    q.push(neighbors[i].segment.end);
                    visited.push_back(neighbors[i].segment.start);
                    visited.push_back(neighbors[i].segment.end);
                    route.associate(neighbors[i].segment.end, currentCoord);
                }
                else
                    segmentDone = false;
                
            }
            if (neighbors[i].attractions.size() > 0){
                for (int j = 0; j < neighbors[i].attractions.size(); j++){
                    for (int k = 0; k < visited.size(); k++){
                        if (visited[k] == neighbors[i].attractions[j].geocoordinates){
                            segmentDone = true;
                            break;
                        }
                    }
                    if (segmentDone == false){
                        route.associate(neighbors[i].attractions[j].geocoordinates, currentCoord);
                        visited.push_back(neighbors[i].attractions[j].geocoordinates);
                        if (endCoord == neighbors[i].attractions[j].geocoordinates){
                            endFound = true;
                            break;
                        }
                    }
                    else
                        segmentDone = false;
                }
            }
            if (endFound){
                break;
            }
        }
        
    } //end of while loop
    
    
    if (endFound == false)
        return NAV_NO_ROUTE;
    
    vector<NavSegment> path;
    GeoCoord current = endCoord;
    GeoCoord* parent = route.find(current);
    GeoSegment currentSegment;
    
    bool isFirstTime = true;
    
    string prevStreet;
    string street;
    
    while (startCoord != *parent){
        
        vector<StreetSegment> options = sm.getSegments(*parent);
        
        if (isFirstTime){
            prevStreet = options[0].streetName;
            street = prevStreet;
            isFirstTime = false;
        }
        
        for (int i = 0; i < options.size(); i++){
            if (((options[i].segment.end == current) && (options[i].segment.start == *parent)) ||
                ((options[i].segment.end == *parent) && (options[i].segment.start == current))){
                prevStreet = street;
                street = options[i].streetName;
                break;
            }
        }
        

        
        currentSegment.start = *parent;
        currentSegment.end = current;
        double distance = distanceEarthMiles(currentSegment.start, currentSegment.end);
        string direction;
        string turn;
        
        if (prevStreet == street){
            //push a proceed segment
            direction = directionOfLine(currentSegment);
            
            NavSegment n (direction, street, distance, currentSegment);
            path.push_back(n);
        }
        else{
            //push turn nav segment
            double dirAngle = angleOfLine(currentSegment);
            direction = directionOfLine(currentSegment);
            if (0 <= dirAngle && dirAngle < 180){
                turn = "left";
            }
            else{
                turn = "right";
            }
            
            
            NavSegment t (turn, prevStreet);
            path.push_back(t);
            NavSegment n (direction, street, distance, currentSegment);
            path.push_back(n);
            
        }
        

        current = *parent;
        parent = route.find(current);
    }
    
    *parent = startCoord;
    double distance = distanceEarthMiles(*parent, current);
    GeoSegment temp;
    temp.start = *parent;
    temp.end = current;
    string direction = directionOfLine(temp);
    NavSegment n (direction, street, distance, temp);
    path.push_back(n);
    
    //somewhat forceful way of fixing the starting segment by integrating first 2 segments
    current = path[path.size() - 2].m_geoSegment.end;
    distance = distanceEarthMiles(*parent, current);
    temp.start = *parent;
    temp.end = current;
    direction = directionOfLine(temp);
    NavSegment newN (direction, street, distance, temp);
    path.pop_back();
    path.pop_back();
    path.push_back(newN);
    
    for (size_t i = path.size(); i > 0; i--){
        directions.push_back(path[i-1]);
    }
    
    return NAV_SUCCESS;
    
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
