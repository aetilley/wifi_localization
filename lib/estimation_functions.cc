#include "estimation_functions.h"

#include <math.h>

using std::string;
using std::vector;
using std::pair;
using std::map;

const double RANGE_FACTOR = 1.0;

/* 
To estimate distance from RSSI, we use the standard
log-distance path loss model.
https://en.wikipedia.org/wiki/Log-distance_path_loss_model
*/
const double reference_rssi = -100; // dBm
const double reference_distance = 1000.0; // m
const double  path_loss_exponent = 6.0;

double get_distance_upper_bound_from_rssi(double rssi) {
  return reference_distance *
    pow(10., (1.0/(10*path_loss_exponent))*(reference_rssi - rssi));
}


// From original code.
// Probably don't use.
float old_get_distance_upper_bound_from_rssi(float rssi) {
  return reference_distance *
    pow(2.0, 6.0 / (reference_rssi - rssi));
}


pair<double, double> estimate_coordinates_from_records(const vector<Record>& records,
						    const map<string, double>& rssis) {
  double var;
  double lat;
  double lon;
  double k;
  for (int i = 0; i < records.size(); i++) {

    Record record = records.at(i);
    string bssid = record.bssid;
    double rssi = rssis.at(bssid);
    double dist_max = get_distance_upper_bound_from_rssi(rssi);
    double var_tmp = (dist_max)*(dist_max) / (RANGE_FACTOR*RANGE_FACTOR);    
    double lat_tmp = record.latitude;
    double lon_tmp = record.longitude;

    if (i == 0) {
      var = var_tmp;
      lat = lat_tmp;
      lon = lon_tmp;
    } else {
      k = var/(var+var_tmp);
      lat = (1-k)*lat + k*(lat_tmp);
      lon = (1-k)*lon + k*(lon_tmp);
      var = (1-k)*var;
    }
  }

  var = sqrt(var);
  
  return pair<double, double>(lat, lon);
    
} 
  


