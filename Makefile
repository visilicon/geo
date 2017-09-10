
all: clean geo_hash


geo_hash:
	g++ -g -I. deps/geohash/geohash.c  deps/geohash/geohash_helper.c  geo_hash.cpp -o geo_hash


clean:
	rm -rf geo_hash
