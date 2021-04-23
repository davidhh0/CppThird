
#ifndef NUMBERWITHUNITS_HPP
#define NUMBERWITHUNITS_HPP
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
using namespace std;
#include "NumberWithUnits.hpp"
#include <string>
#include <fstream>
#include <sstream>


namespace ariel{


class iCell{
        public:
            string _to;
            double _m;
            int _c;
    };
static map<string,iCell> hm;
static int counterForClass;    
class NumberWithUnits{
	
	private:
		double _measure;
		string _type;
		int _class;
		static int checkForObjectClass(const string& type);
		static double convertFromTo(const NumberWithUnits& from,const NumberWithUnits& to);
        static double convertFromToASCombination(const NumberWithUnits& from,const NumberWithUnits& to);
	public:
		
	
	NumberWithUnits(double measure,const string& type);
	static ifstream& read_units(ifstream& file);
	NumberWithUnits& operator+=(const NumberWithUnits nwu1){
        if(_type == nwu1._type){
            double temp = _measure + nwu1._measure;
            _measure=temp;
            return *this;
        }
          
          if(nwu1._class != _class){
            string str = "Units do not match - [";
            str += _type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        NumberWithUnits me = NumberWithUnits{_measure,_type};
        double rate = convertFromTo(me,nwu1);
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,me));
        double temp = _measure + (rate*nwu1._measure);
         _measure = temp;
         return *this;
        }
    NumberWithUnits& operator-=(const NumberWithUnits nwu1){
        if(_type == nwu1._type){
            double temp = _measure - nwu1._measure;
            _measure=temp;
            return *this;
        }
        if(nwu1._class != this->_class){
            string str = "Units do not match - [";
            str += _type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        NumberWithUnits me = NumberWithUnits{_measure,_type};
        double rate = convertFromTo(me,nwu1);
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,me));
        double temp = _measure - (rate*nwu1._measure);
         _measure = temp;
         return *this;
        }
	
	
	ostream& operator<<(ostream& os) {
         os << this->_measure << '[' << this->_type << ']';
         return os;
        }
    	
    	
    	// --------------Friend methods---------------------- 
        friend ostream& operator<<(ostream& os, const NumberWithUnits& nwu1);
        friend NumberWithUnits operator- (const NumberWithUnits& nwu);
        friend NumberWithUnits operator+ (const NumberWithUnits& nwu);
        friend NumberWithUnits operator* (double a,const NumberWithUnits& nwu1);
        friend NumberWithUnits operator* (const NumberWithUnits& nwu1,double a);
        friend NumberWithUnits operator+(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend NumberWithUnits operator-(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend bool operator>=(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend bool operator<=(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend bool operator>(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend bool operator<(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend bool operator==(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2);
        friend bool operator!=(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);
        NumberWithUnits& operator++(){
            _measure++;
            return *this;

        }
        const NumberWithUnits operator++(int dummy){
            NumberWithUnits copy = *this;
            _measure++;
            return copy;
        }
        NumberWithUnits& operator--(){
            _measure--;
            return *this;
        }
        NumberWithUnits operator--(int dummy){
            NumberWithUnits copy = *this;
            _measure--;
            return copy;
        }
        friend istream& operator>> (istream& input, NumberWithUnits& nwu);


        // --------------------------------------------------

    	
    	
    	
    };
};
#endif
