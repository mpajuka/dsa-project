// Datastructures.cc
//
// Student name: Miska Pajukangas
// Student email: miska.pajukangas@tuni.fi
// Student number: 150281685

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <queue>
#include <iostream>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

/**
 * @brief Datastructures::station_count the amount of stations
 * @return the size of stations_vector
 */
unsigned int Datastructures::station_count()
{
    return stations_vector.size();
}

/**
 * @brief Datastructures::clear_all clears all datastructures
 */
void Datastructures::clear_all()
{
    stations_vector.clear();
    stations_map.clear();
    stations_map_coord.clear();
    departures.clear();
    regions.clear();
    region_stations_set.clear();
    region_stations_vec.clear();
    parent_regions_set.clear();
    subregions_set.clear();
    subregions_vec.clear();
}

/**
 * @brief Datastructures::all_stations reads vector<Station> and
 * adds each station to StationID vector
 * @return all stations in a vector with StationID
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> station_ids;
    for (const auto &i : stations_vector) {
        station_ids.push_back(i.station_id);
    }
    return station_ids;
}

/**
 * @brief Datastructures::add_station
 * add a station with the given id, name and
 * coordinates
 * @param id stations id
 * @param name the name of the station
 * @param xy the coordinates of the station
 * @return true if station not yet added, false if
 * already added
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    auto found = stations_map.find(id);
    if (found != stations_map.end())
        return false;
    else {
        Station s;
        s.station_id = id;
        s.station_name = name;
        s.station_coord = xy;
        stations_map.insert(std::make_pair(id, s));
        stations_map_coord.insert(std::make_pair(xy, s));
        stations_vector.push_back(s);
        return true;
    }
}

/**
 * @brief Datastructures::get_station_name
 * returns the name of the station based on
 * station id
 * @param id station's id
 * @return name of station
 */
Name Datastructures::get_station_name(StationID id)
{
    auto found = stations_map.find(id);
    if (found != stations_map.end())
        return found->second.station_name;
    else
        return NO_NAME;
}

/**
 * @brief Datastructures::get_station_coordinates
 * returns the stations coordinates based on
 * station id
 * @param id station's id
 * @return coordinates of the station
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto found = stations_map.find(id);
    if (found != stations_map.end())
        return found->second.station_coord;
    return NO_COORD;
}

/**
 * @brief Datastructures::stations_alphabetically
 * returns the stations added in alphabetical order
 * @return an alphabetically ordered vector with station id's
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> temp;
    std::sort(stations_vector.begin(), stations_vector.end(), []
              (const Station &s1, const Station &s2) {
        return s1.station_name < s2.station_name;
    });
    std::transform(stations_vector.begin(), stations_vector.end(), std::back_inserter(temp),
              [](Station const &s) -> StationID {return s.station_id; });
    return temp;
}

/**
 * @brief Datastructures::stations_distance_increasing
 * returns the station with distance increasing order from
 * the origin
 * @return a distance increasingly ordered vector with station id's
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> temp;
    std::sort(stations_vector.begin(), stations_vector.end(),
              [temp](const Station& st1, const Station& st2)
    {
        if (sqrt((st1.station_coord.x * st1.station_coord.x) +
                 (st1.station_coord.y * st1.station_coord.y)) ==
             sqrt((st2.station_coord.x * st2.station_coord.x)+
                  (st2.station_coord.y * st2.station_coord.y)))
            return st1.station_name < st2.station_name;
        else
            return sqrt((st1.station_coord.x * st1.station_coord.x) +
                   (st1.station_coord.y * st1.station_coord.y)) <
               sqrt((st2.station_coord.x * st2.station_coord.x)+
                    (st2.station_coord.y * st2.station_coord.y));
    });
    std::transform(stations_vector.begin(), stations_vector.end(),
                   std::back_inserter(temp),
                   [](Station const &s) -> StationID {return s.station_id; });
    return temp;
}

/**
 * @brief Datastructures::find_station_with_coord
 * return station id based on coordinate provided
 * @param xy the station's coordinates
 * @return id of the station
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto found = stations_map_coord.find(xy);
    if (found != stations_map_coord.end())
        return found->second.station_id;
    else
        return NO_STATION;
}

/**
 * @brief Datastructures::change_station_coord
 * change station's coordinates based on the
 * station id provided
 * @param id the station's id
 * @param newcoord the new coordinates for the station
 * @return true if successful, false if unsuccessful
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto found = stations_map.find(id);

    if (found != stations_map.end()) {
        auto previous_coord = found->second.station_coord;
        found->second.station_coord = newcoord;
        auto nh = stations_map_coord.extract(previous_coord);
        nh.key() = newcoord;
        stations_map_coord.insert(std::move(nh));
        return true;
    }
    return false;
}

/**
 * @brief Datastructures::add_departure
 * add a departure with the specific station, train
 * and time provided
 * @param stationid the station of the departure
 * @param trainid the train's id
 * @param time the time of departure
 * @return true if given train is not due to depart somewhere else
 * simultaneously false if else
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto found = stations_map.find(stationid);

    if (found == stations_map.end()) {
        return false;
    }

    auto find_departure = std::find_if(departures.begin(), departures.end(),
                           [trainid, stationid, time] (Departure dt) {
        return (trainid == dt.train_id
                && stationid == dt.departure_station)
                ||
               (stationid == dt.departure_station &&
                time == dt.departure_time);
    });

    if (find_departure != departures.end())
        return false;

    else {
        Departure d;
        d.departure_station = stationid;
        d.train_id = trainid;
        d.departure_time = time;
        departures.push_back(d);
        return true;
    }
}

/**
 * @brief Datastructures::remove_departure
 * removes specific departure provided
 * with the given station, train and time of
 * departure
 * @param stationid the station of departure
 * @param trainid the train's id
 * @param time the time of departure
 * @return true if such departure existed false if not
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto find_departure = std::find_if(departures.begin(), departures.end(),
                           [trainid, stationid, time] (Departure dt) {
        return trainid == dt.train_id && stationid == dt.departure_station
                && time == dt.departure_time;
    });

    if (find_departure != departures.end()) {
        departures.erase(find_departure);
        return true;
    }
    else
        return false;
}

/**
 * @brief Datastructures::station_departures_after
 * returs a vector of pairs with time and train id
 * provided based upon the given station and time
 * with every departure after that time
 * @param stationid the station's id
 * @param time the time which after departures are requested
 * @return a vector of pairs with time and train id
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after
    (StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> no_station { {NO_TIME, NO_TRAIN} };
    std::vector<std::pair<Time, TrainID>> no_departures;
    std::vector<std::pair<Time, TrainID>> train_times;

    auto found = stations_map.find(stationid);

    if (found == stations_map.end()) {
        return no_station;
    }

    for (auto &i : departures) {
        if (stationid == i.departure_station && time <= i.departure_time) {
            train_times.push_back(std::make_pair(i.departure_time, i.train_id));
        }
    }
    return train_times;
}

/**
 * @brief Datastructures::add_region
 * adds a region with the given id, name
 * and coordinates
 * @param id the region's id
 * @param name the region's name
 * @param coords the region's coordinates
 * @return true if a region didn't already exist with the given id, false if did
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    auto it = regions.find(id);

    if (it == regions.end()) {
        Region r;
        r.parent = nullptr;
        r.region_coords = coords;
        r.region_id = id;
        r.region_name = name;
        regions.insert(std::pair(id, r));
        return true;
    }
    else
        return false;
}

/**
 * @brief Datastructures::all_regions
 * returns a vector with all regions added
 * @return vector of region id's
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> region_ids;
    for (auto &i : regions) {
        region_ids.push_back(i.second.region_id);
    }
    return region_ids;

}

/**
 * @brief Datastructures::get_region_name
 * return name of region with the given id
 * @param id the region's id
 * @return name of region
 */
Name Datastructures::get_region_name(RegionID id)
{
    auto it = regions.find(id);

    if (it != regions.end())
        return it->second.region_name;
    else
        return NO_NAME;

}

/**
 * @brief Datastructures::get_region_coords
 * return coordinates of region with the given id
 * @param id the region's id
 * @return name of the region
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    std::vector<Coord> no_coords {{NO_COORD}};

    auto it = regions.find(id);
    if (it != regions.end())
        return it->second.region_coords;
    else
        return no_coords;
}

/**
 * @brief Datastructures::add_subregion_to_region
 * adds a subregion to a given region
 * @param id the id of the child region to be added
 * @param parentid the parent region's id
 * @return true if both regions exist false if not
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto find_region = regions.find(parentid);
    auto find_sub = regions.find(id);

    if (find_region != regions.end() && find_sub != regions.end()) {
        find_region->second.children.push_back(&find_sub->second);
        find_sub->second.parent = &find_region->second;
        return true;
    }
    else
        return false;
}

/**
 * @brief Datastructures::add_station_to_region
 * add a station to be a part of given region
 * @param id the id of the station
 * @param parentid the id of the region
 * @return true if given station and region exist, false if not
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto found = stations_map.find(id);

    if (found == stations_map.end())
        return false;

    auto find_region = regions.find(parentid);

    if (find_region != regions.end()) {
        find_region->second.stations.insert(std::pair(id, found->second));
        return true;
    }
    return false;
}

/**
 * @brief Datastructures::station_in_regions
 * returns a vector of regions in which a given
 * station belongs to
 * @param id the id of the station
 * @return vector of region id's
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    region_stations_vec.clear();
    region_stations_set.clear();

    auto find_station = stations_map.find(id);

    if (find_station == stations_map.end())
        return no_region_vec;

    for (auto &i : regions) {
        if (i.second.stations.find(id) != i.second.stations.end()) {
            find_parent(&i.second);

        }
    }
    return region_stations_vec;
}

/**
 * @brief Datastructures::all_subregions_of_region
 * returns a vector with all the subregions of given
 * region
 * @param id the 'parent' region's id
 * @return vector of region id's with the 'children'
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    subregions_vec.clear();

    auto it = regions.find(id);

    if (it != regions.end())
        find_children(&it->second);
    else
        return no_region_vec;

    return subregions_vec;
}

/**
 * @brief Datastructures::stations_closest_to
 * returns a vector with 3 (or less) stations closest to given
 * coordinate
 * @param xy the given random coordinate
 * @return vector of station id's
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> stations_closest;
    std::sort(stations_vector.begin(), stations_vector.end(), [xy]
              (const Station &a, const Station &b) {
        return sqrt(((a.station_coord.x - xy.x) * (a.station_coord.x - xy.x)) +
                    ((a.station_coord.y - xy.y) * (a.station_coord.y - xy.y))) <
                sqrt(((b.station_coord.x - xy.x) * (b.station_coord.x - xy.x)) +
                     ((b.station_coord.y - xy.y) * (b.station_coord.y - xy.y)));
    });
    for (auto &i : stations_vector) {
        if (stations_closest.size() == 3)
            break;
        stations_closest.push_back(i.station_id);
    }
    return stations_closest;
}

/**
 * @brief Datastructures::remove_station
 * remove a statino with the given station id
 * @param id the station's id
 * @return true if given station existed, false if not
 */
bool Datastructures::remove_station(StationID id)
{
    auto found = std::find_if(stations_vector.begin(), stations_vector.end(), [id]
                              (const Station &s) {
       return s.station_id == id;
    });

    if (found != stations_vector.end()) {
        stations_vector.erase(found);
        stations_map.erase(id);
        stations_map_coord.erase(found->station_coord);
        return true;
    }
    return false;
}

/**
 * @brief Datastructures::common_parent_of_regions
 * returns the common 'closest' parent of two given region's
 * @param id1 the id of region No.1
 * @param id2 the id of region No.2
 * @return the common parents region if such exists
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    parent_regions_set.clear();
    has_common = false;

    auto it1 = regions.find(id1);
    auto it2 = regions.find(id2);

    if (it1 == regions.end() || it2 == regions.end())
            return NO_REGION;

    common_parent(it1->second.parent);
    common_parent(it2->second.parent);

    if (has_common == true)
        return common_parent_id;
    else
        return NO_REGION;
}

/**
 * @brief Datastructures::add_train
 * adds a train with pre-determined destination stations and times
 * @param trainid the id of the train
 * @param stationtimes the stations which the train stops at and arrival times
 * @return true if adding the train was successful, false if not
 */
bool Datastructures::add_train
(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes)
{
    auto find_train = trains_uo_map.find(trainid);


    if (find_train != trains_uo_map.end())
        return false;

    for (auto &i : stationtimes) {
        if (get_station_name(i.first) == NO_NAME)
            return false;
    }


    for (auto it = stationtimes.begin(); it != stationtimes.end(); ++it) {
        if (!add_departure(it->first, trainid, it->second)) {
            return false;
        }
        add_departure(it->first, trainid, it->second);
        if (it != stationtimes.end() - 1) {
            auto next = it;
            std::advance(next, 1);
            destinations.insert({it->first, next->first});
        }

    }
    Train t;
    t.id = trainid;
    t.station_times = stationtimes;
    trains_uo_map.insert(std::make_pair(trainid, t));
    trains_vector.push_back(t);
    return true;

}

/**
 * @brief Datastructures::next_stations_from
 * returns a vector of stations which the station determined, has direct connections to it
 * @param id the stations id
 * @return vector of direct connections to station
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<StationID> stations_next_to;

    if (get_station_name(id) == NO_NAME)
        return std::vector<StationID> {NO_STATION};

    for (auto it = destinations.begin(); it != destinations.end(); ++it) {
        if (it->first == id)
            stations_next_to.push_back(it->second);
    }
    return stations_next_to;
}

/**
 * @brief Datastructures::train_stations_from
 * returns a vector of stations which a train passes through
 * @param stationid the station of departure
 * @param trainid the train to depart
 * @return vector of stations in a trains route after given station
 */
std::vector<StationID> Datastructures::train_stations_from
    (StationID stationid, TrainID trainid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<StationID> not_found {NO_STATION};
    std::vector<StationID> stations_from;

    if (get_station_name(stationid) == NO_NAME)
        return not_found;

    auto found_train = trains_uo_map.find(trainid);
    if (found_train == trains_uo_map.end())
        return not_found;


    bool starting_point = false;
    for (auto &stations : found_train->second.station_times) {
        if (stations.first == stationid) {
            starting_point = true;
            continue;
        }
        if (starting_point == true) {
            stations_from.push_back(stations.first);
        }
    }
    if (stations_from.empty())
        return not_found;

    return stations_from;

}

/**
 * @brief Datastructures::clear_trains
 * clears all the data structures storing trains, destinations and departures
 */
void Datastructures::clear_trains()
{
    trains_uo_map.clear();
    trains_vector.clear();
    destinations.clear();
    departures.clear();

}

/**
 * @brief Datastructures::route_any
 * returns a vector of pairs with stations and distances from a departure station to
 * an arrival station, if such route is possible
 * @param fromid the station of departure
 * @param toid the station of arrival
 * @return vector of stations and distances in an unspecific route between two stations
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_any
    (StationID fromid, StationID toid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<std::pair<StationID, Distance>> route_with_distance;
    std::vector<std::pair<StationID, Distance>> not_found {{NO_STATION, NO_DISTANCE}};
    std::vector<std::pair<StationID, Distance>> no_route;
    std::deque<StationID> path;

    if (get_station_name(fromid) == NO_NAME)
        return not_found;

    if (get_station_name(toid) == NO_NAME)
        return not_found;

    path.push_front(toid);

    StationID end = toid;
    unsigned long counter = 0;

    while (end != fromid) {

        for (auto iter = destinations.begin(); iter != destinations.end(); ++iter) {
            if (iter->second == end) {
                path.push_front(iter->first);
                end = iter->first;
                counter = 0;
            }
            if (end == fromid)
                break;
            if (counter > destinations.size()) {
                return no_route;
            }
            counter++;
        }
    }

    StationID previous = "";
    Distance current_dist = 0;

    for (auto &i : path) {
        if (i == fromid) {
            route_with_distance.push_back(std::make_pair(i, 0));
            previous = i;
            continue;
        }
        Coord previous_coord = get_station_coordinates(previous);
        Coord current_coord = get_station_coordinates(i);
        current_dist += calculate_distance(previous_coord, current_coord);

        route_with_distance.push_back(std::make_pair(i, current_dist));
        previous = i;
    }


    return route_with_distance;
}



/**
 * @brief Datastructures::route_least_stations
 * @return
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations
    (StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_least_stations()");
}

/**
 * @brief Datastructures::route_with_cycle
 * @return
 */
std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_with_cycle()");
}

/**
 * @brief Datastructures::route_shortest_distance
 * @return
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance
    (StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_shortest_distance()");
}

/**
 * @brief Datastructures::route_earliest_arrival
 * @return
 */
std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival
    (StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_earliest_arrival()");
}
