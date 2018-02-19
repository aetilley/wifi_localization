#include "drop_old.h"
#include <iostream>

using std::map;
using std::pair;
using std::string;
using std::vector;
using std::tm;


vector<Record> drop_old_records(vector<Record> all_records) {

  map<string, time_t> bssid_to_latest_time;
  map<string, Record> bssid_to_record;
  
  for (Record record : all_records) {
    string bssid = record.bssid;

    tm date_tm = {0,0,0,0,0,0,0,0,0};    
    strptime(record.last_updated.c_str(), "%Y-%m-%d", &date_tm);

    time_t date_t = timegm(&date_tm);
    
    auto it = bssid_to_latest_time.find(bssid);
    if ((it == bssid_to_latest_time.end()) || (it->second < date_t)) {
      bssid_to_latest_time[bssid] = date_t;
      bssid_to_record[bssid] = record;
    }
  }

  vector<Record> result;
  for(pair<string, Record> item : bssid_to_record) {
    result.push_back(item.second);
  }
  return result;
}
