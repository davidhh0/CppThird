#include <iostream>
#include <stdexcept>
#include <string>
#include "doctest.h"
#include <map>
#include <stdexcept>
using namespace std;
#include "NumberWithUnits.hpp"
#include <string>
#include <fstream>
#include <sstream>
using namespace ariel;

TEST_CASE("Addition"){

	istringstream input_700kg{"700 [ kg ]"};
	istringstream input_3ton{"3 [ ton ]"};
	istringstream input_5555g{"5555 [ g ]"};
	istringstream input_750kg{"750 [ kg ]"};
	istringstream input_12345678g{"12345678 [ g ]"};
	

	istringstream input_2km{"5 [ km ]"};
	istringstream input_40000m{"40000 [ m ]"};


	ifstream units_file{"/home/david/Desktop/MessageBoard_DEBUG/projects/helloworld/text.txt"};
  	NumberWithUnits::read_units(units_file);
  	NumberWithUnits a{2, "km"};
  	NumberWithUnits b{230,"m"};
  	NumberWithUnits temp{2.23,"km"};
	// 2.23km == 2km + 230m
  	CHECK((temp == (a+b)));
  	
	// 3700kg = 700kg + 3ton
  	input_700kg >> a;
	input_3ton >> b;
	NumberWithUnits temp2{3700,"kg"};
	CHECK((a+b) == temp2);

	// 305555g = 5555g + 3ton
	input_5555g >> a;
	input_3ton >> b;
	NumberWithUnits temp3{3005555,"g"};
	CHECK((a+b) == temp3);

	// 700kg + 5555g = 705.55kg
	
	input_750kg >> a;
	input_12345678g >> b;
	NumberWithUnits temp4{13095.678,"kg"};
	CHECK((a+b)==temp4); 
	

}


TEST_CASE("Subtraction"){
	istringstream input_700kg{"700 [ kg ]"};
	istringstream input_3ton{"3 [ ton ]"};
	istringstream input_5555g{"5555 [ g ]"};
	istringstream input_750kg{"750 [ kg ]"};
	istringstream input_12345678g{"12345678 [ g ]"};
	

	istringstream input_5km{"5 [ km ]"};
	istringstream input_40000m{"40000 [ m ]"};

	// 15.5km - 2309m = 13.191km
	NumberWithUnits a{15.5, "km"};
  	NumberWithUnits b{2309,"m"};
  	NumberWithUnits temp{13.191,"km"};
	CHECK((a-b)==temp);
	
	// 5km - 40,000m = -35km
	// 40,000m - 5km == -(-35km) 
	input_5km>>a;
	input_40000m>>b;
	NumberWithUnits temp1{-35,"km"};
	CHECK((a-b)==temp1);
	CHECK((b-a) == (-temp1));
	CHECK((++(b-a)) >= (-temp1));
	temp1++;temp1++;
	CHECK((-(b-a)) <= temp1);
}

TEST_CASE("< == >"){
	istringstream input_millionDollarBaby{"1000000 [ USD ]"};
	istringstream input_666S{"666 [ ILS ]"};
	NumberWithUnits a{750, "ILS"};
  	NumberWithUnits b{200,"USD"};
	
	CHECK((a>b));
	CHECK((a<(2*b)));
	input_666S>>a;
	CHECK(((4*b)==(4*a)));
	CHECK((a++)==b);
	CHECK(a>b);
	input_millionDollarBaby>>b;
	CHECK((10000*a)>=b);
	CHECK(a<=b);
	CHECK(a>=a);
	CHECK_FALSE(a>a);
	NumberWithUnits temp{2,"km"};
	CHECK_THROWS((temp + a));
	CHECK_THROWS((temp - a));
	NumberWithUnits temp2{2,"km"};
	CHECK((temp==temp2));
	temp+=temp2; // temp->2*temp
	CHECK((temp)==(2*temp2));
	temp-=temp2;
	CHECK((temp==temp2));
	temp--;
	CHECK(temp == (0.5*temp2));
	--temp2;
	CHECK(temp==temp2);

	CHECK(((temp<=temp2) && (temp>=temp2)));

}

TEST_CASE("THROWING"){
	/*
	1 km = 1000 m
	1 m = 100 cm
	1 kg = 1000 g
	1 ton = 1000 kg
	1 hour = 60 min
	1 min = 60 sec
	1 USD = 3.33 ILS
	*/

	NumberWithUnits dis{5,"km"};
	NumberWithUnits dis1{5,"m"};
	NumberWithUnits dis2{5,"cm"};
	NumberWithUnits weight{5,"ton"};
	NumberWithUnits weight1{5,"kg"};
	NumberWithUnits time{5,"hour"};
	NumberWithUnits time2{5,"min"};
	NumberWithUnits time3{5,"sec"};
	NumberWithUnits curr{5,"USD"};
	NumberWithUnits curr1{5,"ILS"};


	CHECK_THROWS(dis + weight);
	CHECK_THROWS((weight-time));
	CHECK_NOTHROW(weight+weight1);
	CHECK_THROWS(curr-dis2);
	CHECK_THROWS((time+=weight1));
	CHECK_THROWS((curr-=dis2));





}
