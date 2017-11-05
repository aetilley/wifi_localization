#ifndef LOCALIZATION_RECORD
#define LOCALIZATION_RECORD

#include <string>

struct Record {
  std::string bssid;
  std::string last_updated;
  double latitude;
  double longitude;
};

#endif // LOCALIZATION_RECORD
