#pragma once

typedef enum {
    MAP_PALLET_TOWN,
    MAP_NATIONAL_PARK,
// Ice Path
    MAP_ICE_PATH_1F,
    MAP_ICE_PATH_B1F,
    MAP_ICE_PATH_B2F_BLACKTHORN,
    MAP_ICE_PATH_B2F_MAHOGANY,
    MAP_ICE_PATH_B3F,
    //
    MAP_COUNT,
} MapID;

static const char *mapTable[] = {
    [MAP_PALLET_TOWN]   = "assets/data/maps/pallet-town.dat",
    [MAP_NATIONAL_PARK] = "assets/data/maps/national-park.dat",
// Ice Path
    [MAP_ICE_PATH_1F]  = "assets/data/maps/ice-path-1F.dat",
    [MAP_ICE_PATH_B1F] = "assets/data/maps/ice-path-B1F.dat",
    [MAP_ICE_PATH_B3F] = "assets/data/maps/ice-path-B3F.dat",
    [MAP_ICE_PATH_B2F_BLACKTHORN] = "assets/data/maps/ice-path-B2F-blackthorn.dat",
    [MAP_ICE_PATH_B2F_MAHOGANY]   = "assets/data/maps/ice-path-B2F-mahogany.dat",
};
