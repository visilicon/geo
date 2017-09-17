
all: clean geo_hash geo_rtree


geo_hash:
	g++ -g -I. deps/geohash/geohash.c  deps/geohash/geohash_helper.c  geo_hash.cpp -o geo_hash

geo_rtree:
	g++ -g -I. deps/geohash/geohash.c  deps/geohash/geohash_helper.c  geo_rtree.cpp -o geo_rtree

clean:
	rm -rf geo_hash geo_rtree
