set +e
echo "\ntest static_checker.h"
g++ -o test_static_checker.o test_static_checker.cpp -std=c++11
echo "\ntest type_manip.h"
g++ -o test_type_manip.o test_type_manip.cpp -std=c++11
echo "\ntest type_list.h"
g++ -o test_type_list.o test_type_list.cpp -std=c++11
set -e