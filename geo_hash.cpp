#include <stdio.h>
#include "deps/geohash/geohash.h"
#include "deps/geohash/geohash_helper.h"

const static int STEP = 26;

void radius_caculate(double lat, double lng, double radius){
    GeoHashRadius ghr;
    ghr = geohashGetAreasByRadiusWGS84(lat, lng, radius);
    printf("radisu:%f lat:%f,lng:%f\n", radius, lat, lng);
    GeoHashBits hash = ghr.hash;
    printf("  geohash:%X, step:%d\n", hash.bits, hash.step);
    GeoHashArea area = ghr.area;
    printf("  geoarea:lat-min:%f, lat-max:%f, lng-min:%f, lng-max:%f\n",area.latitude.min, area.latitude.max, area.longitude.min, area.longitude.max);
    GeoHashNeighbors neighbors = ghr.neighbors;
    printf("  geonearby:\n");
    printf("   north_west:%X   north:%X,  north_east:%X\n", neighbors.north_west.bits, neighbors.north.bits, neighbors.north_east.bits);
    printf("                       \\         |             / \n");
    printf("         west:%X  center:%X,        east:%X\n", neighbors.west.bits, hash.bits, neighbors.east.bits);
    printf("                       /         |             \\ \n");
    printf("  north_south:%X   south:%X,  south_east:%X\n", neighbors.south_west.bits, neighbors.south.bits, neighbors.south_east.bits);

    printf("\n");
}

int main(int argc, char *argv[]) {
    double center_lng = 116.41667;
    double center_lat = 39.91667;
    double lat_range = 0.7;
    double lng_range = 0.3;
    for (double lat = center_lat - lat_range; lat < center_lat + lat_range; lat += 0.001) {
        for (double lng = center_lng - lng_range; lng < center_lng + lng_range; lng += 0.001) {
            GeoHashBits hash;
            geohashEncodeWGS84(lat, lng, STEP, &hash);
//            printf("lat:%f\tlng:%f, GEOHASH:%X\n", lat, lng, hash.bits);
        }
    }
    double lat = 39.310670;
    double lng = 116.247670;
    radius_caculate(lat, lng, 500);

    return 0;
}

