#ifndef __IPAddressh__
#define __IPAddressh__

#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;
//This error is thrown when trying to get invalid value of ip
class InvalidValueException{};

/** Read, check correctness and store IP address */
class IPAddress
{
public:
	IPAddress(): valid_(false) {};

	friend istream& operator>>(istream &in , IPAddress &a );

	/** Check if current value of ip in class is valid or not */
	bool isValid() { return valid_; }

	/**Returns the string that represents IP */
	const string& getIP() throw( InvalidValueException );

private:
	/** Validates if the string is valid value or not */
	void validate();
	string ip_;
	bool valid_;
};
#endif
