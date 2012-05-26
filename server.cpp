/***************************************************************************************
				MAIN SERVER FILE
***************************************************************************************/

#include <Wt/WServer>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WServer>

#include <iostream>
#include <string>
#include <sstream>

#include "ipaddress.h"

using namespace Wt;
using namespace std;

//TODO kubik
//tutaj wstaw wejsciowy widget strony czyli tam gdzie bedzie logowanie
WApplication* create( const Wt::WEnvironment &env )
{
	return new WApplication(env);
}

/**************************************************************************************
			    MAIN SERVER FUNCTION
			This is where everything stars...
**************************************************************************************/
int main(int argc , char** argv)
{
	cout<<endl<<"Starting http server . . . "<<endl;
	

	IPAddress ip_address;
	while(1)
	{	
		cout<<endl<<"Enter IP addres (enter for default - loopback address)"<<endl;
		cin>>ip_address;
		
		if(ip_address.isValid())
		{
			cout<<endl<<"Address " +ip_address.getIP() +" has been set"<<endl;
			break;
		}
		else cout<<endl<<"This is not a valid IP address";
	}
	
	int port;

	while(1)
	{
		cout<<endl<<"Enter port number"<<endl;
		cin>>port;

		if( port > 0 )
		{
			cout<<"Port "<< port << " has been set"<<endl;
			break;
		}
		else cout<<"This is not a valid port";
	}
	
	//server object
	WServer server;
	const char *nolog = "/dev/null";
	const char *logdir = "logs";
	const char *opt1 = "--docroot";
	const char *par1 = ".";	
	string opt2s("--http-address" );
	//opt2s += ip_address.getIP() + " ";
	const char *opt2 = opt2s.c_str();
	
	ostringstream os;
	os<<port;
	string ports = os.str();
	string opt3s("--http-port");
	//opt3s += ports;
	const char *opt3 = opt3s.c_str();

	const char *options[9];
	options[0] = argv[0];
	options[1] = opt1;
	options[2] = par1;
	options[3] = opt2;
	options[4] = ip_address.getIP().c_str();
	options[5] = opt3;
	options[6] = ports.c_str();
	options[7] = "--accesslog";
	options[8] = nolog;
	
	//seting for our server
	server.setServerConfiguration( 9 , const_cast<char**>( options ) , WTHTTP_CONFIGURATION );
	server.addEntryPoint(Wt::Application , create);

	//Let's roll the ball
	server.start();
	
	cout<<endl<<"*****Server started*****"<<endl;
	cin.ignore();
	
	//loop simulating command line
	while(1)
	{

		string command;
		cout<<endl<<"server$ ";
		getline(cin , command);
		if( command == "stop" )	server.stop();
		else if( command == "start") server.start();
		else if(command == "exit" ) { if(server.isRunning()) server.stop(); return 0;}
	}	
}








































	
