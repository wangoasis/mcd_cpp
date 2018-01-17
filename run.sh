set +e
echo "\ntest static_checker.h"
g++ -o test_static_checker.o test_static_checker.cpp -std=c++11
echo "\ntest type_manip.h"
g++ -o test_type_manip.o test_type_manip.cpp -std=c++11
echo "\ntest type_list.h"
g++ -o test_type_list.o test_type_list.cpp -std=c++11
echo "\ntest hierarchy_generators.h"
g++ -o test_gen_scatter_hierarchy.o test_gen_scatter_hierarchy.cpp -std=c++11
echo "\ntest test_callable.cpp"
g++ -o test_callable.o test_callable.cpp -std=c++11
echo "\ntest test_small_obj.cpp"
g++ -o test_small_obj.o test_small_obj.cpp small_obj.cpp -std=c++11
echo "\ntest test_functor.cpp"
g++ -o test_functor.o test_functor.cpp small_obj.cpp -std=c++11
set -e