#ifndef LOCALIZATION_QUERY_FUNCTIONS
#define LOCALIZATION_QUERY_FUNCTIONS

#include <string>
#include <vector>

#include "record.h"

// FOR REGULAR LOOKUP (BSSIDS -> COORDINATE ESTIMATE)
std::vector<Record> get_records_for_bssids(const std::vector<std::string>& bssids);



// FOR REVERSE LOOKUP (COORDINATES -> NEARBY BSSIDS)
std::vector<Record>
get_records_for_coordinates(double latitude, double longitude,
			    double lat_delta, double lon_delta);


#endif // LOCALIZATION_QUERY_FUNCTIONS
