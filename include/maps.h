#pragma once

typedef enum {
    MAP_PALLET_TOWN,
    MAP_ROUTE_1,
    MAP_ROUTE_1_HOUSE_A,
    MAP_ROUTE_2,
    MAP_ROUTE_12,
    MAP_NATIONAL_PARK,
    //
    MAP_COUNT,
} MapID;

static const char *mapTable[] = {
    [MAP_PALLET_TOWN] = "assets/data/maps/pallet-town.dat",
    [MAP_NATIONAL_PARK] = "assets/data/maps/national-park.dat",
};
