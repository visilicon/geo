#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

#include "deps/geohash/geohash.h"
#include "deps/geohash/geohash_helper.h"
#include "deps/tree/RTreeTemplate.h"

using namespace std;

const static int STEP = 26;
const static double LNG_PRECISION = 1;
const static double LAT_PRECISION = 1;
const static int SCALE = 36e5;
const static int LAT_RANGE = 33;
const static int LNG_RANGE = 34;


void geo_scale(double lat, double lng, uint64_t& tlat, uint64_t& tlng)
{
    tlat = lat * SCALE;
    tlng = lng * SCALE;		
}

void query_scale(double lat, double lng, uint64_t* qlat, uint64_t* qlng, int meters)
{
    uint64_t tlat, tlng;
    geo_scale(lat, lng, tlat, tlng);
    qlat[0] = tlat - meters * LAT_RANGE;
    qlat[1] = tlat + meters * LAT_RANGE;
    qlng[0] = tlng - meters * LNG_RANGE;
    qlng[1] = tlng + meters * LNG_RANGE;
}

int split_by_char( const char* strSource, char splitchar, vector<string>& vec_str)
{
    const char* beg=strSource;
    const char* end=beg;
    while ( *beg != '\0'  && *end != '\0')
    {
        if ( *end == splitchar )
        {
            if ( end !=beg )
            {
                vec_str.push_back( string( beg, end-beg));
            }
            end++;
            beg=end;
        }else
        {
            end++;
        }
    }
    if ( end !=beg )
    {
        vec_str.push_back( string( beg, end-beg));
    }
    return 0;
}


int main(int argc, char *argv[]) {
    char filename[] = "location.18";
    FILE *fp;
    fp=fopen(filename, "r");
    if (fp == NULL) {
        printf("open file:%s failed! \n", filename);
        return -1;
    }
    RTree<uint64_t, uint64_t, 2, float, 8> geo_rtree;
    double center_lng = 106.559370;
    double center_lat = 29.604410;
    double meters = 500.0;

    char line[64];
    memset(line, '\0', 64);
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strlen(line)-1] = '\0';
	vector<string> split_vec;
	split_by_char(line, '\t', split_vec);
        GeoHashBits hash;
	double lat = atof(split_vec[0].c_str());
	double lng = atof(split_vec[1].c_str());
        geohashEncodeWGS84(lat, lng, STEP, &hash);
        uint64_t tlat, tlng;
        geo_scale(lat, lng, tlat, tlng);
        uint64_t min[2] = {tlat, tlng};
        uint64_t max[2] = {tlat, tlng};
        geo_rtree.Insert(min, max, hash.bits);
	double distance = distanceEarth(lat, lng, center_lat, center_lng);
	if(distance < meters)
	printf("lat:%f,lng:%f,%lld,%lld,%lld,distance:%f\n", lat, lng, tlat, tlng, hash.bits, distance);
    	memset(line, '\0', 64);
    }
    fclose(fp);    
    printf("tree count:%d\n", geo_rtree.Count());
    uint64_t qlat[2], qlng[2];
    query_scale(center_lat, center_lng, qlat, qlng, (int)meters);
    vector<uint64_t> searchids;
    uint64_t min[2] = {qlat[0], qlng[0]};
    uint64_t max[2] = {qlat[1], qlng[1]};
    int count = geo_rtree.Search(min, max, searchids);

    printf("\nlat:%lld-%lld, lng:%lld-%lld\n", qlat[0], qlat[1], qlng[0], qlng[1]);
    printf("\nsearchids:%d\n", count);
    for(int i = 0; i < count; i++)
    {
	printf("  %lld\n", searchids[i]);
    }

    
    
    return 0;
}

