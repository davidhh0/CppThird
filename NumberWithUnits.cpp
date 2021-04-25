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
           // This function returns the class of the specified type, 
           // for example -> weight has class A and distance has class B , A!=B
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
        input>>ch;
        if(ch != '['){
            error =true;
        }
        input>>type;
        // Checking if the last char is ']' and than take the substring
        if(type.at(type.length()-1) == ']'){
        string C = type.substr(0,type.length()-1);
        ch = type.at(type.length()-1);
        if(NumberWithUnits::checkForObjectClass(C) == -1 || ch != ']'){
            error = true;
        }
        type=C;
        }
        // else: the last char is not ']' which means the next one should be ']'
        else{
            input>>ch;
            if(NumberWithUnits::checkForObjectClass(type) == -1 || ch != ']'){
                error=true;
            }
        }

         if (error) {
        string str = "Invalid input";

        throw std::invalid_argument(str);
        auto errorState = input.rdstate(); // remember error state
        input.clear(); // clear error so seekg will work
        input.seekg(startPosition); // rewind
        input.clear(errorState); // set back the error flag
    }
    
    nwu._measure=measure;
    nwu._type=type;
    nwu._class = NumberWithUnits::checkForObjectClass(type);
    
    
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
                            // CHECK IF THERE IS hm[temp] !!!!
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
            for (it = hm.begin(); it != hm.end(); it++)
                {
                    if(it->first == from._type){
                        temp = from._type;
                        while(true){
                            if (temp == to._type){
                                return product;
                            }
                            if(hm.find(temp) == hm.end()){
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
                return -1;
        }

        double NumberWithUnits::convertFromToASCombination(const NumberWithUnits& from,const NumberWithUnits& to){
            // we want how many measure units in from to to
            // from = X*to
            double product=1;
            string temp;
            map<string, iCell>::iterator it;
            //Combination of KEY ~~ VALUE
            for (it = hm.begin(); it != hm.end(); it++)
                {
                    if(it->first == to._type){
                        temp = it->first;
                        product*=hm[temp]._m;
                        temp = hm[temp]._to;
                        while(true){
                            /// FIND temp AS a value and get its KEY
                            for(it = hm.begin();it!=hm.end() ; it++){
                                if(it->second._to == temp && it->first != to._type){
                                    string bridge = it->first;
                                    product=hm[bridge]._m/product;
                                    while (true)
                                    {
                                        if(hm.find(bridge) == hm.end()){
                                            return product;
                                        }
                                        if(bridge == from._type){
                                            return product;
                                        }
                                        bridge = hm[bridge]._to;
                                        product=hm[bridge]._m/product;
                                    }
                                    
                                }
                            }

                            
                        }

                    }
                    
                }
            return -1;
        }
        
     ostream& operator<<(ostream& os, const NumberWithUnits& nwu) {
         os << nwu._measure << '[' << nwu._type << ']';
         return os;
        }
    NumberWithUnits operator- (const NumberWithUnits& nwu){
        NumberWithUnits toReturn{(-1)*nwu._measure,nwu._type};
        return toReturn;
    }
    NumberWithUnits operator+ (const NumberWithUnits& nwu){
        return nwu;
    }
    NumberWithUnits operator* (double a,const NumberWithUnits& nwu1){
        NumberWithUnits toReturn{(a)*nwu1._measure,nwu1._type};
        return toReturn;
    }
    NumberWithUnits operator* (const NumberWithUnits& nwu1,double a){
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
        if(nwu1._type == nwu2._type){
            NumberWithUnits toReturn{(nwu1._measure+nwu2._measure),nwu1._type};
            return toReturn;
        }
        // convert nwu2 to nwu1
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = rate!=-1?((1/rate)*nwu2._measure):(nwu1._measure)*(1/NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        NumberWithUnits toReturn{nwu1._measure+first,nwu1._type};
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
        if(nwu1._type == nwu2._type){
            NumberWithUnits toReturn{(nwu1._measure-nwu2._measure),nwu1._type};
            return toReturn;
        }
        
       double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = rate!=-1?((1/rate)*nwu2._measure):(nwu1._measure)*(1/NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        NumberWithUnits toReturn{nwu1._measure-first,nwu1._type};
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
        if(nwu1._type == nwu2._type){
            return (nwu1._measure<nwu2._measure);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        double second = nwu2._measure;
        return (second-first>EPS);
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
        if(nwu1._type == nwu2._type){
            return (nwu1._measure>nwu2._measure);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        double second = nwu2._measure;
        return (first-second>EPS);
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
        if(nwu1._type == nwu2._type){
            return (nwu1._measure>=nwu2._measure);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        double second = nwu2._measure;
        return (first>=second);
    }
    bool operator<=(const NumberWithUnits& nwu1, const NumberWithUnits &nwu2){
         if(nwu1._class != nwu2._class){
            string str = "Units do not match - [";
            str += nwu2._type;
            str += "] cannot be converted to [";
            str+= nwu1._type;
            str+="]";
            throw std::invalid_argument(str);
        }
        if(nwu1._type == nwu2._type){
            return (nwu1._measure<=nwu2._measure);
        }
        double rate = NumberWithUnits::convertFromTo(nwu2,nwu1);
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        double second = nwu2._measure;
        return (first<=second);
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
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        double second = nwu2._measure;
        return (abs(first-second)<EPS);
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
        double first = rate!=-1?((rate)*nwu1._measure):(nwu1._measure)*(NumberWithUnits::convertFromToASCombination(nwu1,nwu2));
        double second = nwu2._measure;
        return (abs(first-second)>EPS);
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
