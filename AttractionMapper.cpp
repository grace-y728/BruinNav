#include "provided.h"
#include "support.h"
#include <string>
#include "MyMap.h"
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> aMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0; i < ml.getNumSegments(); i++){
        StreetSegment s;
        ml.getSegment(i, s);
        vector<Attraction> a = s.attractions;
        
        for (int j = 0; j < a.size(); j++){
            string upperName;
            for (int k = 0; k < a[j].name.size(); k++){
                upperName += toupper(a[j].name[k]);
            }
            aMap.associate(upperName, a[j].geocoordinates);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string upperName;
    for (int k = 0; k < attraction.size(); k++){
        upperName += toupper(attraction[k]);
    }
    if (aMap.find(upperName) != nullptr){
        gc = *aMap.find(upperName);
        return true;
    }
	return false;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
