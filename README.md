Note: The data/ directory must contain an appropriate sqlite database.  See the README in that directory.

1) Find BSSIDs near some corrdinates:

To build:

g++ find_bssids_main.cc lib/query_functions.cc -o find_bssids -std=c++11 -l sqlite3

To run:

./find_bssids 37.8725 -122.2702 .0027 .0027




2) Estimate coordinates from a collection of BSSIDs and RSSIs:

To build:

g++ localization_main.cc lib/estimation_functions.cc lib/query_functions.cc -o localization -std=c++11 -l sqlite3

To run:

./localization 1CAA074394BA -100.0 B8BEBFEFC31C -100.0 DC7FA40BE02E -100.0


