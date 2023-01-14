// Datastructures.hh
//
// Student name: Miska Pajukangas
// Student email: miska.pajukangas@tuni.fi
// Student number: 150281685

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //
    // Old assignment 1 operations
    //

    // Estimate of performance: O(1)
    // Short rationale for estimate: "vector.size()" operation returns
    // the vector's size in constant time, as it doesn't require looping
    // through
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear in time complexity as it
    // requires looping through to erase each element individually with std::erase
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear in time, as one for-loop
    // was used to add the contents to another data structure
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. Other operations are constant
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: std::sort has a worst case time complexity of n log n
    // and std::transform has a linear worst case time complexity, thus the estimate.
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: std::sort has a worst case time complexity of n log n
    // and std::transform has a linear worst case time complexity, thus the estimate.
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: std::map::find has a logarithmic complexity based on
    // the size of the container
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. Other operations work in constant time
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case and std::find_if has at most linear time
    // complexity. Other operations are constant
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find and std::find_if have at most linear time
    // complexity as well as std::erase
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. for-loop works in linear time
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. Other operations are constant
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-loop works in linear time, push_back is constant
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. Other operations are constant
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. Other operations are constant
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. for-loop and std::clear are linear in time complexity
    // helper function find_parent has also a linear std::unordered_set::find and constant operations
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case. helper function find_children has a for-loop so it
    // is linear
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: std::sort has a worst case time complexity of n log n
    // and the for-loop works in linear time
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::find_if has a worst case linear time complexity
    // and the other operations work in constant time
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find works average in constant time
    // and linear in worst case, helper function common_parent uses std::unordered_set::find
    // which has the time complexity, other operations are constant
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(n)
    // Short rationale for estimate: 'find_train' works worst case in linear time, other than
    // that the two for loops have a linear complexity and other operations are constant
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance: O(n)
    // Short rationale for estimate: get_station_name works worst case in linear time, looping
    // through 'destinations' is at most linear as in the size of the container
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: get_station_name works worst case in linear time as does
    // 'found_train', the for loop requires iterating through and complexity is dependent on
    // the size of container, thus the complexity is together at most linear
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: All operations are at most linear, as clearing the elements
    // depends on the sizes of the containers
    void clear_trains();

    // Estimate of performance: O(n)
    // Short rationale for estimate: get_station_name works worst case in linear time, other
    // operations depend on the size of the containers 'path' or 'destinations' and thus is
    // linear
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:
    // Add stuff needed for your class implementation here
    struct Station {
            StationID station_id;
            Name station_name;
            Coord station_coord;
    };
    std::vector<Station> stations_vector;
    std::unordered_map<StationID, Station> stations_map;
    std::map<Coord, Station> stations_map_coord;


    struct Departure {
        StationID departure_station;
        TrainID train_id;
        Time departure_time;
    };
    std::vector<Departure> departures;


    struct Region {
        RegionID region_id;
        Name region_name;
        std::vector<Coord> region_coords;
        std::vector<Region*> children;
        std::map<StationID, Station> stations;
        Region* parent;
    };
    std::unordered_map<RegionID, Region> regions;

    struct Train {
        TrainID id;
        std::vector<std::pair<StationID, Time>> station_times;
    };

    std::vector<Train> trains_vector;
    std::unordered_map<TrainID, Train> trains_uo_map;
    std::unordered_multimap<StationID, StationID> destinations;


    void find_parent(Region* r) {
        if (r == NULL)
            return;

        auto found_station = region_stations_set.find(r->region_id);

        if (found_station != region_stations_set.end()) {
            return;
        }
        region_stations_set.insert(r->region_id);
        region_stations_vec.push_back(r->region_id);
        find_parent(r->parent);
    }
    std::unordered_set<RegionID> region_stations_set;
    std::vector<RegionID> region_stations_vec;


    void find_children(Region* r) {
        for (auto &i : r->children) {
            subregions_set.insert(i->region_id);
            subregions_vec.push_back(i->region_id);
            find_children(i);
        }
    }
    std::unordered_set<RegionID> subregions_set;
    std::vector<RegionID> subregions_vec;


    void common_parent(Region* r) {
        if (r == NULL) {
            return;
        }
        auto find_region = parent_regions_set.find(r->region_id);

        if (find_region != parent_regions_set.end()) {
            common_parent_id = r->region_id;
            has_common = true;
            return;
        }
        parent_regions_set.insert(r->region_id);
        common_parent(r->parent);
    }
    std::unordered_set<RegionID> parent_regions_set;
    RegionID common_parent_id;
    bool has_common;

    std::vector<RegionID> no_region_vec {NO_REGION};

    Distance calculate_distance(Coord coord1, Coord coord2) {
        return sqrt(pow(coord1.x - coord2.x, 2) + pow(coord1.y - coord2.y, 2));
    }
};

#endif // DATASTRUCTURES_HH
