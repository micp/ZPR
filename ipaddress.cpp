#include "ipaddress.h"

const string& IPAddress::getIP() throw( InvalidValueException )
{
	if( !valid_) throw InvalidValueException();
	return ip_;
}
void IPAddress::validate()
{
	static const regex ip_regex(
	"(((1[0-9]{2})|(2[0-4][0-9])|(25[0-5])|([1-9][0-9])|[0-9]).){3}"
	"((1[0-9]{2})|(2[0-4][0-9])|(25[0-5])|([1-9][0-9])|[0-9])");

	if( regex_match( ip_ , ip_regex)) valid_ = true;
	else
	if( ip_.empty() ) 
	{
		valid_ = true;
		ip_ = "127.0.0.1";
	}
	else valid_ = false;
}	

istream& operator>>(istream &in , IPAddress &a )
{
	a.ip_.clear();
	char c;
	in.get(c);
	if( c != '\n' )
	{ 
		in.unget();
		in>>a.ip_;
		in.get(c);
	}
	a.validate();
	return in;
}
