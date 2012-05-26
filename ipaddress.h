#ifndef __IPAddressh__
#define __IPAddressh__

#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;
//This error is thrown when trying to get invalid value of ip
class InvalidValueException{};

/*
	read, check correctness and store IP address
*/
class IPAddress
{
public:
	IPAddress(): valid_(false) {};

	friend istream& operator>>(istream &in , IPAddress &a );

	bool isValid() { return valid_; }

	const string& getIP() throw( InvalidValueException );

private:
	void validate();
	string ip_;
	bool valid_;
};
#endif
