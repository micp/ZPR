#include "game.h"

void Game::join( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );

	if( ( refX_ == NULL )||( refO_ != NULL ) ) throw IllegalCommand();
	refO_ = &ref;
}

void Game::start( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( refX_ == NULL )||( refO_ == NULL ) ) throw IllegalCommand();

	if( refX_ == &ref ) if( startX_ == true ) throw IllegalComand();
				else startX_ = true; 
	
	if( refO_ == &ref ) if( startO_ == true ) throw IllegalComand();
				else startO_ = true; 

	if( startX_ && startO_ )
	{
		turn_ = refX_;
		game_.resetGame();
		refX_->gameStarted(); //TODO
		refO_->gameStarted(); //TODO
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
		if( &ref == refX_ )
		{
			FieldX f;
			game_.put( f , x , y );
			refX_->fieldChanged(f , x , y ); //TODO 
			refO_->fieldChanged(f , x , y); //TODO 
		} 
		else
		{
			FieldO f;
			game_.put( f , x , y);
			refX_->fieldChanged(f , x , y ); //TODO 
			refO_->fieldChanged(f , x , y); //TODO 
		}

	}
	catch( FieldTakenException &e ) {}

	synch_.writterLeave();
}

void Game::giveUp( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( turn_ == NULL )||!( startX_ && startO_ ) ) throw IllegalComand();

	startX_ = false;
	startO_ = false;
	turn_ = NULL;

	if( refX_ == ref )
	{	//update pkt
		refX_->givedUp();	//TODO
		refO_->wonByGivingUp();	//TODO
	}
	else
	{	//update pkt
		refX_->wonByGivingUp();
		refO_->givedUp()
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
		if( ref0_ != NULL ) refO_.playerExited();//TODO
	}
	else
	{
		refO_ = NULL;
		if( refX_ != NULL ) refX_.playerExited(); //TODO
	}
}
		
void Game::revange( GameListRefresher &ref )
{
	scoped_lock<interprocess_mutex>( mutexTurn_ );
	scoped_lock<interprocess_mutex>( mutexStart_ );
	
	if( ( turn == NULL )|| startX_ || startO_ ) throw IllegalComand();

	GameListRefresher *tmp = refX_;
	refX_ = refO_;
	refO_ = tmp;

	refX_->revangeProposed(); //TODO
	refO_->revangeProposed(); //TODO
}

Game::Game( GameListRefresher &ref ): refX_( &ref ) , startX_(false), startO_( false ), refO_(NULL) , turn_(NULL)
{
	end_ = new EndOfGameListener(this);
}
	










































	
