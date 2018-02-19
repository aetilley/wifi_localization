#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "lib/query_functions.h"
#include "lib/estimation_functions.h"
#include "lib/drop_old.h"

using std::string;
using std::vector;
using std::map;
using std::to_string;

int main(int argc, char* argv[]) {

  if (argc % 2 != 1) {
    std::cout << "Wrong number of inputs.  Input must be in the form <BSSID_1> <RSSI_1> ... <BSSID_n> <RSSI_n>\n";
    return 1;
  }

  if (argc == 1) {
    std::cout << "No BSSIDs were provided.\n";
    return 1;
  }
  
  vector<string> input(argv + 1, argv + argc);
  
  vector<string> bssids;
  map<string, double> rssis;

  for (int i = 0; i < argc - 1; i+=2) {
    string bssid = input[i];
    double rssi = atof(input[i+1].c_str());
    bssids.push_back(bssid);
    rssis[bssid] = rssi;
  }  
  
  std::cout << "Querying database for the following BSSIDs:\n";
  
  for (int i = 0 ; i < bssids.size() ; i++){
    string bssid = bssids[i];
    std::cout << "BSSID " << bssid << std::endl;
    std::cout << "RSSI " << rssis[bssid] << std::endl;
  }
  std::cout << "\n";

  vector<Record> records =
    get_records_for_bssids(bssids);

  if (records.size() == 0) {
    std::cout << "Found no matching records.\n";
    return 1;
  }
  
  records = drop_old_records(records);


  std::cout << "Found the following matching records (taking latest for each BSSID):" << std::endl;
  for (Record record : records) {
    std::cout << "bssid: " << record.bssid << std::endl;
    std::cout << "last_updated: " << record.last_updated << std::endl;
    std::cout << "latitude: " << record.latitude << std::endl;
    std::cout << "longitude: " << record.longitude << std::endl;
  }
  std::cout << "\n";

    
  std::pair<double, double> coordinates =
    estimate_coordinates_from_records(records, rssis);

  std::cout << "Coordinate estimate: \n";
  std::cout << "Latitude = ";
  std::cout << to_string(coordinates.first) + "\n";
  std::cout << "Longitude = ";
  std::cout << to_string(coordinates.second) + "\n";
  
  return 0;
}



