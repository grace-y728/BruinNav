 #include "provided.h"
#include "support.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    
    //helper functions
    void getStartEndCoords(string line, string &startLat, string &startLong, string &endLat, string &endLong);
    void getAttraction(string line, Attraction &a);
private:
    vector<StreetSegment> segmentVector;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
    //delete every segment from vector
}

bool MapLoaderImpl::load(string mapFile)
{

    ifstream infile(mapFile);
    if (!infile){
        return false;
    }
    else{
        
        //process line by line
        string currentLine = "";
        
        //while end of file is not reached
        while (getline(infile, currentLine)){
            //get string of street name
            string streetName = currentLine;
            
            //get start and end coordinates, make geosegment
            getline(infile, currentLine);
            string startLat;
            string startLong;
            string endLat;
            string endLong;
            getStartEndCoords(currentLine, startLat, startLong, endLat, endLong);
            GeoCoord start(startLat, startLong);
            GeoCoord end(endLat, endLong);
            GeoSegment gSeg(start, end);
            
            //get number of attractions
            getline(infile, currentLine);
            int numAttractions = stoi(currentLine);
            
            //record number of attractions
            vector<Attraction> vecAttractions;
            for (int i = 0; i < numAttractions; i++){
                getline(infile, currentLine);
                //parse the attraction
                Attraction currentA;
                getAttraction(currentLine, currentA);
                
                //add attraction to vector
                vecAttractions.push_back(currentA);

            }
            //make street segment
            StreetSegment sSeg;
            sSeg.streetName = streetName;
            sSeg.segment = gSeg;
            sSeg.attractions = vecAttractions;
            
            //add to vector of street segments
            segmentVector.push_back(sSeg);
            
        }
        
        return true;
    }
}

//////////HELPERS FOR LOAD FUNCTION //////////
void MapLoaderImpl::getStartEndCoords(string line, string &startLat, string &startLong, string &endLat, string &endLong){
    string coordArr[4];
    int currentNum = 0;
    for (int i = 0; i < line.size(); i++){
        string current;
        if (isdigit(line[i]) || line[i] == '.' || line[i] == '-'){
            while (isdigit(line[i]) || line[i] == '.' || line[i] == '-'){
                current += line[i];

                i++;
            }
            coordArr[currentNum] = current;
            currentNum++;
        }
        /*else{
            i++;
        }*/
    }
    startLat = coordArr[0];
    startLong = coordArr[1];
    endLat = coordArr[2];
    endLong = coordArr[3];
    
}

void MapLoaderImpl::getAttraction(string line, Attraction &a){
    //get attraction name
    string name;
    int counter = 0;
    for (counter = 0; counter < line.length(); counter++){
        if (line[counter] == '|'){
            counter++;
            break;
        }
        else{
            name += line[counter];
        }
    }
    a.name = name;
    
    //get attraction coords
    string latitude;
    string longitude;
    while (isdigit(line[counter]) || line[counter] == '.' || line[counter] == '-'){
        latitude += line[counter];
        counter++;
    }
    while (!(isdigit(line[counter]) || line[counter] == '-')){
        counter++;
    }

    while (counter < line.size()){
        longitude += line[counter];
        counter++;
    }

    GeoCoord coord (latitude, longitude);
    a.geocoordinates = coord;
}

///////// END OF HELPER FUNCTIONS /////////////



size_t MapLoaderImpl::getNumSegments() const
{
	return segmentVector.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum >= segmentVector.size()){
        return false;
    }
    else{
        seg = segmentVector[segNum];
        return true;
    }
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
