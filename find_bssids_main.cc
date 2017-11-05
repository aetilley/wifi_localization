#include <string>
#include <iostream>
#include <vector>

#include "lib/query_functions.h"

using std::vector;
using std::string;
using std::stod;

int main(int argc, char* argv[]) {

  string usage = "./a.out <latitude> <longitude> <lat_delta> <lon_delta>";

  if (argc != 5) {
    std::cout << usage <<std::endl;
    return 1;
  }

  double latitude = stod(argv[1]);
  double longitude = stod(argv[2]);
  double lat_delta = stod(argv[3]);
  double lon_delta = stod(argv[4]);

  vector<Record> records =
    get_records_for_coordinates(latitude, longitude,
					lat_delta, lon_delta);

  if (records.size() == 0) {
    std::cout <<
      "Could not find any records within given delta of given coordinates.\n";
    return 1;
  }
  
  std::cout << "Found the following nearby bssids:" << std::endl;
  for (Record record : records) {
    std::cout << "bssid: " << record.bssid << std::endl;
    std::cout << "last_updated: " << record.last_updated << std::endl;
    std::cout << "latitude: " << record.latitude << std::endl;
    std::cout << "longitude: " << record.longitude << std::endl;
  }

  return 0;

}
    
    

  
