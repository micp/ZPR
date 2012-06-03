#ifndef __GAMEH__
#define __GAMEH__

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/bind.hpp>
#include <Wt/WServer>

#include "endofgamelistener.h"
#include "user.h"
#include "OXGame.h"
#include "gamesconnector.h"
class GamesConnector;
class GameListRefresher;
class OXGame;
using namespace boost::interprocess;
/*EXAMPLE TO forFields
class ViewFunctor
{
public:
	void operato()( const Field &f , int x , int y );
};*/
class IllegalCommand{};
#ifndef __GAMESCONNECTOR_TEST__
class Game
{
	friend class EndOfGameListener;
public:

	Game( GameListRefresher &ref );

	~Game();// { delete end_; } 
	
	void join( GameListRefresher &ref );//done

	void putField( GameListRefresher &ref , int x , int y );//done

	void start( GameListRefresher &ref );//done

	void giveUp( GameListRefresher &ref );//done

	void exit( GameListRefresher &ref );//done

	void revenge( GameListRefresher &ref );//done
	
	std::string& getServerUserName();

	template<class T>
	void forFields( T &f )
	{
		synch_.readerEnter();

		for( int y = 0 ; y < game_.getSize() ; ++y )
			for( int x = 0 ; x < game_.getSize() ; ++x )
				f( game_.getField( x , y ) , x , y );

		synch_.readerLeave();
	}
private:
	void updateScore( GameListRefresher* winner, GameListRefresher* looser );
	GameListRefresher* refX_;
	GameListRefresher* refO_;
	
	GameListRefresher* turn_;
	interprocess_mutex mutexTurn_;

	GamesConnector::Synchronizer synch_;
	OXGame game_;

	interprocess_mutex mutexStart_;
	bool startX_;
	bool startO_;
	
	EndOfGameListener *end_;
};
#endif
#include "gamelistrefresher.h"
#endif
