#include "endofgamelistener.h"

EndOfGameListener::EndOfGameListener(Game* g): g_(g) {}

void EndOfGameListener::gameEnded(FieldEmpty f, int startX , int startY , int finishX , int finishY )
{
	g_->turn_ = NULL;
	g_->startX_ = false;
	g_->startO_ = false;
	WServer::instance()->post(g_->refX_->getSessionID() , boost::bind( &GameListRefresher::endedWithDraw,
	g_->refX_ , startX , startY , finishX , finishY) ); 
	WServer::instance()->post(g_->refO_->getSessionID() , boost::bind( &GameListRefresher::endedWithDraw,
	g_->refO_ , startX , startY , finishX , finishY) ); 
}

void EndOfGameListener::gameEnded(FieldX f, int startX , int startY , int finishX , int finishY )
{
	g_->turn_ = NULL;
	g_->startX_ = false;
	g_->startO_ = false;
	
	g_->updateScore(g_->refX_ , g_->refO_ );
	
	WServer::instance()->post(g_->refX_->getSessionID() , boost::bind( &GameListRefresher::endedWithWin,
	g_->refX_ , startX , startY , finishX , finishY) ); 
	WServer::instance()->post(g_->refO_->getSessionID() , boost::bind( &GameListRefresher::endedWithLose,
	g_->refO_ , startX , startY , finishX , finishY) ); 
	//g_->refX_->endedWithWin(startX, startY, finishX , finishY);
	//g_->refO_->endedWithLose(startX, startY, finishX , finishY);
}

void EndOfGameListener::gameEnded(FieldO f, int startX , int startY , int finishX , int finishY )
{
	g_->turn_ = NULL;
	g_->startX_ = false;
	g_->startO_ = false;
	
	g_->updateScore(g_->refO_ , g_->refX_ );
	
	WServer::instance()->post(g_->refX_->getSessionID() , boost::bind( &GameListRefresher::endedWithLose,
	g_->refX_ , startX , startY , finishX , finishY) ); 
	WServer::instance()->post(g_->refO_->getSessionID() , boost::bind( &GameListRefresher::endedWithWin,
	g_->refO_ , startX , startY , finishX , finishY) ); 
	//g_->refO_->endedWithWin(startX, startY, finishX , finishY);
	//g_->refX_->endedWithLose(startX, startY, finishX , finishY);
}
