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
/** Represents illegal command.
 */
class IllegalCommand{};
#ifndef __GAMESCONNECTOR_TEST__
/** Class responsible for player-player game synchronization */
class Game
{
	friend class EndOfGameListener;
public:
	/**Makes a new game with one player added*/
	Game( GameListRefresher &ref );

	~Game(); 
	/** Join second player to the game */
	void join( GameListRefresher &ref );
	/** Informs game that player would like to put his fild in that place*/
	void putField( GameListRefresher &ref , int x , int y );
	/** One of the player (ref ) would like to start the game */
	void start( GameListRefresher &ref );
	/** One of the player (ref ) would like to give up */
	void giveUp( GameListRefresher &ref );
	/** One of player( ref ) would like to exit*/
	void exit( GameListRefresher &ref );
	/** One of player( ref ) would like to take revange*/
	void revenge( GameListRefresher &ref );
	/** Returns the name of user which establisched this game */
	std::string& getServerUserName();
	/** Execute some functor for each field */
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
	/** Functions which uses the database connections and actualize scores*/
	void updateScore( GameListRefresher* winner, GameListRefresher* looser );
	/** X Player */
	GameListRefresher* refX_;
	/** O Player */
	GameListRefresher* refO_;
	/** Whose turn it is */	
	GameListRefresher* turn_;
	interprocess_mutex mutexTurn_;

	/** Synchronization object */
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
