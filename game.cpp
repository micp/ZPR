#include "game.h"

void Game::join( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );

	if( ( refX_ == NULL )||( refO_ != NULL ) ) throw IllegalCommand();
		refO_ = &ref;
	WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::playerJoined, refX_) ); 
//	refX_->playerJoined();
        std::cout<<refO_<<std::endl;
	std::cout<<refX_<<std::endl;
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
//		refX_->gameStarted(); //TODO
//		refO_->gameStarted(); //TODO
	}
}

void Game::putField( GameListRefresher &ref , int x , int y )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( !(startX_ && startO_ ) ) throw IllegalCommand();
	if( turn_ != &ref ) throw IllegalCommand();

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
	//		refX_->fieldChanged(f , x , y ); //TODO 
	//		refO_->fieldChanged(f , x , y); //TODO 
		} 
		else
		{
			FieldO f;
			game_.put( f , x , y);
WServer::instance()->post(refX_->getSessionID() , boost::bind( fO, refX_ , f , x , y) ); 
WServer::instance()->post(refO_->getSessionID() , boost::bind( fO, refO_ , f , x , y) ); 
//			refX_->fieldChanged(f , x , y ); //TODO 
//			refO_->fieldChanged(f , x , y); //TODO 
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
	{	//update pkt
WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::wonByGivingUp, refO_) ); 
//		refO_->wonByGivingUp();	//TODO
	}
	else
	{	//update pkt
WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::wonByGivingUp, refX_) ); 
//		refX_->wonByGivingUp();
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
		if( refO_ != NULL ) //refO_->playerExited();//TODO
WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::playerExited, refO_) ); 
	}
	else
	{
		refO_ = NULL;
		if( refX_ != NULL ) //refX_->playerExited(); //TODO
WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::playerExited, refX_) ); 
	}
}
		
void Game::revenge( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( turn_ == NULL )|| startX_ || startO_ ) throw IllegalCommand();

	GameListRefresher *tmp = refX_;
	refX_ = refO_;
	refO_ = tmp;

WServer::instance()->post(refX_->getSessionID() , boost::bind( &GameListRefresher::revengeProposed, refX_) ); 
WServer::instance()->post(refO_->getSessionID() , boost::bind( &GameListRefresher::revengeProposed, refO_) ); 
//	refX_->revengeProposed(); //TODO
//	refO_->revengeProposed(); //TODO
}

Game::Game( GameListRefresher &ref ): refX_( &ref ) , startX_(false), startO_( false ), refO_(NULL) , turn_(NULL)
{
	end_ = new EndOfGameListener(this);
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






































	
