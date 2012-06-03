#include "game.h"

void Game::join( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );

	if( ( refX_ == NULL )||( refO_ != NULL ) ) throw IllegalCommand();
		refO_ = &ref;
	WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::playerJoined, refX_) ); 
}

void Game::start( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( refX_ == NULL )||( refO_ == NULL ) ) throw IllegalCommand();

	if( refX_ == &ref ) if( startX_ == true ) throw IllegalCommand();
				else startX_ = true; 
	
	if( refO_ == &ref ) if( startO_ == true ) throw IllegalCommand();
				else startO_ = true; 

	if( startX_ && startO_ )
	{
		turn_ = refX_;
		game_.resetGame();
		WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::gameStarted, refX_) ); 
		WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::gameStarted, refO_) ); 
	}
}

void Game::putField( GameListRefresher &ref , int x , int y )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( !(startX_ && startO_ ) ) throw IllegalCommand();
	if( turn_ != &ref )return;

	if( ( x >= game_.getSize() )||( y >= game_.getSize() ) ) throw IllegalCommand();
	
	synch_.writterEnter();
	
	try
	{
		void (GameListRefresher::*fX)(FieldX &, int , int) = 
	static_cast<void (GameListRefresher::*)(FieldX &, int , int)>(&GameListRefresher::fieldChanged);
		
		void (GameListRefresher::*fO)(FieldO &, int , int) = 
	static_cast<void (GameListRefresher::*)(FieldO &, int , int)>(&GameListRefresher::fieldChanged);
		if( &ref == refX_ )
		{
			FieldX f;
			game_.put( f , x , y );
			WServer::instance()->post(refX_->getSessionID() , boost::bind( fX, refX_ , f , x , y) ); 
			WServer::instance()->post(refO_->getSessionID() , boost::bind( fX, refO_ , f , x , y) ); 
			turn_ = refO_;
		} 
		else
		{
			FieldO f;
			game_.put( f , x , y);
			WServer::instance()->post(refX_->getSessionID() , boost::bind( fO, refX_ , f , x , y) ); 
			WServer::instance()->post(refO_->getSessionID() , boost::bind( fO, refO_ , f , x , y) ); 
			turn_ = refX_;
		}
		
	}
	catch( FieldTakenException &e ) {}

	synch_.writterLeave();
}

void Game::giveUp( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( turn_ == NULL )||!( startX_ && startO_ ) ) throw IllegalCommand();

	startX_ = false;
	startO_ = false;
	turn_ = NULL;

	if( refX_ == &ref )
	{	
		updateScore( refO_ , refX_ );
		WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::wonByGivingUp, refO_) ); 
	}
	else
	{	
		updateScore( refO_ , refX_ );
		WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::wonByGivingUp, refX_) ); 
	}
}

void Game::exit( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );

	if( turn_ != NULL ) giveUp( ref);

	if( refX_ == &ref )
	{

		refX_ = NULL;
		if( refO_ != NULL ) 
		WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::playerExited, refO_) ); 
	}
	else
	{
		refO_ = NULL;
		if( refX_ != NULL ) 
		WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::playerExited, refX_) ); 
	}
}
		
void Game::revenge( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( turn_ != NULL )|| startX_ || startO_ ) throw IllegalCommand();

	GameListRefresher *tmp = refX_;
	refX_ = refO_;
	refO_ = tmp;

	WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::revengeProposed, refX_) ); 
	WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::revengeProposed, refO_) ); 
}

Game::Game( GameListRefresher &ref ): refX_( &ref ) , startX_(false), startO_( false ), refO_(NULL) , turn_(NULL)
{
	end_ = new EndOfGameListener(this);
	game_.addEndOfGameListener( *end_ );
}
	
std::string& Game::getServerUserName()
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	
	return refX_->getUserName();
}

Game::~Game()
{
  delete end_;
}

void Game::updateScore( GameListRefresher* winner, GameListRefresher* looser )
{
	winner->getSession()->updateScore( 10 );
	looser->getSession()->updateScore( -5 );
}





































	
