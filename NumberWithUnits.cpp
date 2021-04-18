#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
using namespace std;
#include "NumberWithUnits.hpp"
#include <string>
#include <fstream>
#include <sstream>
constexpr double EPS = 0.0001;
namespace ariel {
	
	NumberWithUnits::NumberWithUnits(double measure,const string& type){
           _measure = measure;
           _type = type;
           int temp = NumberWithUnits::checkForObjectClass(type);
           if(temp == -1){
               throw std::invalid_argument("There is no such a type of unit!");
           }
           
           _class = temp;
           
       }
       
       int NumberWithUnits::checkForObjectClass(const string& type){
            map<string, iCell>::iterator it;
                for (it = hm.begin(); it != hm.end(); it++)
                {
                    if(it->first == type  ||
						 it->second._to==type) {
                        return it->second._c;
                    }
                }
                return -1;
        }
        
        
	ifstream& NumberWithUnits::read_units(ifstream& file){
	    
	   counterForClass=0;

            if (!file) {
				cout<<"FAILED TO READ THE FILE !!"<<endl;
                exit(1);
            }

            string line;

            while (getline(file, line)) {
                // new_line -> 1
                int new_line=0;
		double number=1;
                string from;
                string to;
                string eq;
                stringstream ss(line);
                ss >> new_line >> from >> eq >> number >> to;

                bool flag=false;
                iCell _aux;
				_aux._m=number;
				_aux._to = to;

                // Searching in the map's values & keys:
                map<string, iCell>::iterator it;
                for (it = hm.begin(); it != hm.end(); it++)
                {
                    if(it->first == from || it->first == to ||
						it->second._to == from || it->second._to==to) {
                        _aux._c = it->second._c;
						_aux._c = it->second._c;
                        flag=true;
						break;
                    }
                   
                }
                if (!flag){
                    _aux._c=++(counterForClass);
                }
                hm.insert(std::pair<string ,iCell>(from,_aux));

            }
			// cout<<hm["km"]._to<<endl;
			// cout<<hm["m"]._m<<endl;
            // cout<<hm["USD"]._m<<endl;

            return file;
	}
	
	
	    static istream& getAndCheckNextCharIs(istream& input, char expectedChar) {
    char actualChar=0;
    input >> actualChar;
    if (!input){ 
    	return input;
    }	

    if (actualChar!=expectedChar){ 
        input.setstate(ios::failbit);
        }
    return input;
}

    istream& operator>> (istream& input, NumberWithUnits& nwu){
        
        char ch=0;
        string type;
        double measure=1;
        bool error=false;
        ios::pos_type startPosition = input.tellg();
        input>>measure;
        // measure will have the amount units of the given input
        //Check measure is a number

        //check chFirst == '['
        if(!getAndCheckNextCharIs(input,'[')){
            error=true;
        }
        input>>type;
        if(!getAndCheckNextCharIs(input,']')){
            error=true;
        }
        
         if (error)
          {
		// rewind on error
		auto errorState = input.rdstate(); // remember error state
		input.clear(); // clear error so seekg will work
		input.seekg(startPosition); // rewind
		input.clear(errorState); // set back the error flag
   	 }
	if (!error)
	{
		nwu._measure=measure;
		nwu._type=type;
		int temp = NumberWithUnits::checkForObjectClass(type);
		nwu._class = temp;
		
	}
    return input;
    }
	
	
	
	
        
        double NumberWithUnits::convertFromTo(const NumberWithUnits& from,const NumberWithUnits& to){
            // we want how many measure units in from to to
            // from = X*to
            double product=1;
            bool flag=true;
            string temp;
            map<string, iCell>::iterator it;
            // FROM as a key of the HM
                for (it = hm.begin(); it != hm.end() && flag; it++)
                {
                    if(it->first == to._type){
                        temp = it->first;
                        while(true){
                            // CHECK IF THERE IS hm[temp] !!!!8
                            //my_map.find( key ) != my_map.end()
                            
                            if(hm.find(temp) == hm.end()){
                                product=1;
                                flag=false;
                                break;
                            }
                            product*=hm[temp]._m;
                            if(hm[temp]._to == from._type){
                                return product; 
                            }
                            
                            temp = hm[temp]._to;
                        }

                    }
                    
                }
            // FROM as a value of the HM
            flag = true;
            for (it = hm.begin(); it != hm.end() && flag; it++)
                {
                    if(it->first == from._type){
                        temp = from._type;
                        while(true){
                            if (temp == to._type){
                                return product;
                            }
                            if(hm.find(temp) == hm.end()){
                                product=-1;
                                break;
                            }
                            product*= (1/hm[temp]._m);
                            if(temp == to._type){
                                return product;
                            }

                            temp = hm[temp]._to;
                        }
                        break;
                    }
                    
                }
                return product;
        }
        
        
     ostream& operator<<(ostream& os, const NumberWithUnits& nwu) {
         os << nwu._measure << '[' << nwu._type << ']';
         return os;
        }
    NumberWithUnits operator- (const NumberWithUnits& nwu){
        NumberWithUnits toReturn{(-1)*nwu._measure,nwu._type};
        return toReturn;
    }
    NumberWithUnits operator* (double a,const NumberWithUnits& nwu1){
        NumberWithUnits toReturn{(a)*nwu1._measure,nwu1._type};
        return toReturn;
    }
    NumberWithUnits operator+(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        // convert nwu2 to nwu1
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        NumberWithUnits toReturn{(nwu1._measure+(1/rate)*nwu2._measure),nwu1._type};
        return toReturn;
    }
    NumberWithUnits operator-(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        NumberWithUnits toReturn{(nwu1._measure-(1/rate)*nwu2._measure),nwu1._type};
        return toReturn;
    }
    bool operator<(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = ((rate)*nwu1._measure);
        double second = nwu2._measure;
        return first<second;
    }
    bool operator>(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = ((rate)*nwu1._measure);
        double second = nwu2._measure;
        return first>second;
    }
    bool operator>=(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = ((rate)*nwu1._measure);
        double second = nwu2._measure;
        return first>=second;
    }
    bool operator<=(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        return (nwu1==nwu2 || nwu1 < nwu2);
        // if(nwu1._class != nwu2._class){
        //     string str = "Units do not match - [";
        //     str += nwu2._type;
        //     str += "] cannot be converted to [";
        //     str+= nwu1._type;
        //     str+="]";
        //     throw std::invalid_argument(str);
        // }
        // double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        // double first = ((rate)*nwu1._measure);
        // double second = nwu2._measure;
        // return first<=second;
    }
    bool operator==(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
        
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        
         if(nwu1._type == nwu2._type){
            return (abs(nwu1._measure - nwu2._measure) < EPS);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = ((rate)*nwu1._measure);
        double second = nwu2._measure;
        return (first==second);
    }

    bool operator!=(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2){
        if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = ((rate)*nwu1._measure);
        double second = nwu2._measure;
        return first!=second;
    }
    static istream& getAndNextCharIs(istream& input, char expectedChar) {
    char actualChar='0';
    input >> actualChar;
    if (!input){
     return input;
     }

    if (actualChar!=expectedChar) {
        input.setstate(ios::failbit);
       }
    return input;
}

    
       
};
