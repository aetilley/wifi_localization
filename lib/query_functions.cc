#include "query_functions.h"

#include <math.h>
#include <sqlite3.h>
#include <iostream>
#include <map>
#include <cstring>

using std::string;
using std::vector;
using std::pair;
using std::map;

const char* database_path = "./data/us.sqlite";
const char* table_name = "wifi_zone";
const char* latitude_column_name = "latitude";
const char* longitude_column_name = "longitude";
const char* bssid_column_name = "bssid";
const char* last_updated_column_name = "last_updated";


static int get_record_callback(void *result_void, int argc,
				char **argv, char **azColName){
  char* val;
  char null[] = "NULL";
  Record record;
  
  std::cout << "Found record: " << std::endl;
  for (int i = 0; i < argc; i++) {
    val = argv[i]? argv[i] : null;
    printf("%s = %s\n", azColName[i], val);

    if (strcmp(azColName[i],bssid_column_name)==0) {
      record.bssid = val;
    }
    if (strcmp(azColName[i],last_updated_column_name)==0) {
      record.last_updated = val;
    }
    if (strcmp(azColName[i],latitude_column_name)==0) {
      record.latitude = std::stod(val);
    }
    if (strcmp(azColName[i],longitude_column_name)==0) {
      record.longitude = std::stod(val);
    }
  }
  
  printf("\n");
  vector<Record>* result =
    static_cast<vector<Record>*>(result_void);
  result->push_back(record);

  return 0;
}
    

vector<Record> query_bssids(const vector<string>& bssids,
			  sqlite3* pDB) {

  char* exec_error_message = 0;
  int exec_status;

  vector<Record> result = {};
  
  int num_bssids = bssids.size();
  if (num_bssids == 0) {
    std::cout << "Must give at least one bssid." << std::endl;
    return result;
  }

  string table_name_string = string(table_name);
  string sql_query = "SELECT * FROM " + table_name_string + " WHERE ";
  sql_query += "bssid=\"" + bssids[0] + "\"";
  for (int i=1; i<num_bssids ; i++) {
    sql_query += "OR bssid=\"" + bssids[i] + "\"";
  }
  
  const char* sql_query_cstring =  sql_query.data();
  
  exec_status = sqlite3_exec(pDB, sql_query_cstring, get_record_callback,
			&result, &exec_error_message);

  if(exec_status != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", exec_error_message);
    sqlite3_free(exec_error_message);
  }

  return result;
}




std::vector<Record>
  get_records_for_bssids(const vector<string>& bssids) {

  sqlite3* pDB;
  int status;
  std::vector<Record> query_results = {};
  
  status = sqlite3_open(database_path, &pDB);

  if(status != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDB));
    return query_results;
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  query_results = query_bssids(bssids, pDB);
    
  sqlite3_close(pDB);
  
  return query_results;
}



// FUNCTIONS FOR REVERSE LOOKUP (COORDINATES -> BSSIDS)

vector<Record> query_coordinates(float latitude, float longitude,
				 float lat_delta, float lon_delta,
				 sqlite3* pDB) {

  char* exec_error_message = 0;
  int exec_status;
  
  string table_name_string = string(table_name);
  string latitude_string = std::to_string(latitude);
  string longitude_string = std::to_string(longitude);
  string lat_delta_string = std::to_string(lat_delta);
  string lon_delta_string = std::to_string(lon_delta);
	
  string sql_query = "SELECT * FROM " + table_name_string + " WHERE ";
  sql_query += "abs(longitude - " + longitude_string +
    ") < " + lon_delta_string;
  sql_query += " AND ";
  sql_query += "abs(latitude - " + latitude_string +
    ") < " + lat_delta_string;
  
  const char* sql_query_cstring =  sql_query.data();
  
  vector<Record> result;
  exec_status = sqlite3_exec(pDB, sql_query_cstring, get_record_callback,
			&result, &exec_error_message);

  if(exec_status != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", exec_error_message);
    sqlite3_free(exec_error_message);
  }

  return result;
}




std::vector<Record>
get_records_for_coordinates(double latitude, double longitude,
			    double lat_delta, double lon_delta) {

  sqlite3* pDB;
  int status;
  std::vector<Record> query_results = {};
  
  status = sqlite3_open(database_path, &pDB);

  if(status != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDB));
    return query_results;
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  query_results =
    query_coordinates(latitude, longitude, lat_delta, lon_delta, pDB);
    
  sqlite3_close(pDB);
  
  return query_results;
}
