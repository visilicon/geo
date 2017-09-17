#include <stdio.h>
#include "deps/geohash/geohash.h"
#include "deps/geohash/geohash_helper.h"
#include "deps/tree/RTreeTemplate.h"

const static int STEP = 26;

void simple_change_accuracy(double lat, double lng, uint64_t& tlat, uint64_t& tlng)
{
	tlat = lat * 1e6;
	tlng = lng * 1e6;		
}

int main(int argc, char *argv[]) {
    double center_lng = 116.41667;
    double center_lat = 39.91667;
    double lat_range = 0.7;
    double lng_range = 0.3;
    RTree<uint64_t, uint64_t, 2, float, 8> geo_rtree;
    for (double lat = center_lat - lat_range; lat < center_lat + lat_range; lat += 0.001) {
        for (double lng = center_lng - lng_range; lng < center_lng + lng_range; lng += 0.001) {
            GeoHashBits hash;
            geohashEncodeWGS84(lat, lng, STEP, &hash);
            uint64_t tlat, tlng;
            simple_change_accuracy(lat, lng, tlat, tlng);
            uint64_t min[2] = {tlat, tlng};
            uint64_t max[2] = {tlat, tlng};
            geo_rtree.Insert(min, max, hash.bits);
        }
    }
    printf("tree count:%d\n", geo_rtree.Count());
    return 0;
}

