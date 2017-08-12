#include "provided.h"
#include <vector>
#include "MyMap.h"
#include "support.h"

#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    vector<StreetSegment> segmentVector;
    MyMap<GeoCoord, vector<StreetSegment>> segMapper;
    
};



SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    
    //iterate all segments of maploader, add every start and end coord to map if not already added
    size_t numSegments =  ml.getNumSegments();
    
    for (int i = 0; i < numSegments; i++){
        
        //see if segment's coords are already in map
        StreetSegment currentSeg;
        ml.getSegment(i, currentSeg);
        //if segment's start coords are not in map
        if (segMapper.find(currentSeg.segment.start) == nullptr){
            //add to map
            vector<StreetSegment> sv;
            sv.push_back(currentSeg);
            segMapper.associate(currentSeg.segment.start, sv);

        }
        //if they are in map
        else {
            (segMapper.find(currentSeg.segment.start))->push_back(currentSeg);
        }
        
        //if segment's end coords are not in map
        if (segMapper.find(currentSeg.segment.end) == nullptr){
            //add to map
            vector<StreetSegment> sv;
            sv.push_back(currentSeg);
            segMapper.associate(currentSeg.segment.end, sv);
        }
        //if they are in map
        else{
            (segMapper.find(currentSeg.segment.end))->push_back(currentSeg);
        }
        
        //check all attractions coords of this segment
        vector<Attraction> a = currentSeg.attractions;
        for (int j = 0; j < a.size(); j++){
            //if this is not in map
            if (segMapper.find(a[j].geocoordinates) == nullptr){
                vector<StreetSegment> sv;
                sv.push_back(currentSeg);
                segMapper.associate(a[j].geocoordinates, sv);
            }
            //if it is already in the map
            else{
                (segMapper.find(a[j].geocoordinates))->push_back(currentSeg);
            }
        }
        
        
    }
    
    
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector <StreetSegment> empty;
    if (segMapper.find(gc) != nullptr)
    
        return *(segMapper.find(gc));
    return empty;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
