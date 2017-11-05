#ifndef LOCALIZATION_ESTIMATION_FUNCTIONS
#define LOCALIZATION_ESTIMATION_FUNCTIONS

#include <vector>
#include <string>
#include <map>

#include "record.h"

std::pair<double, double>
estimate_coordinates_from_records(const std::vector<Record>& records,
				  const std::map<std::string, double>& rssis);

#endif // LOCALIZATION_ESTIMATION_FUNCTIONS
