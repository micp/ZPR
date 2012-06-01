#include "endofgamelistener.h"

void EndOfGameListener::gameEnded(FieldEmpty f, int startX , int startY , int finishX , int finishY )
{
	g_.turn_ = NULL;
	g_.startX_ = false;
	g_.startO_ = false;
	//TODO Co z punktami przy remisie?
	g_.refX_->endedWithDraw(startX , startY , finishX , finishY);	
	g_.refO_->endedWithDraw(startX , startY , finishX , finishY);	
}

void EndOfGameListener::gameEnded(FieldX f, int startX , int startY , int finishX , int finishY )
{
	g_.turn_ = NULL;
	g_.startX_ = false;
	g_.startO_ = false;
	//TODO
	//odwolanie do bazy danych z update rankingu
	g_.refX_->endedWithWin(startX, startY, finishX , finishY);
	g_.refO_->endedWithLose(startX, startY, finishX , finishY);
}

void EndOfGameListener::gameEnded(FieldO f, int startX , int startY , int finishX , int finishY )
{
	g_.turn_ = NULL;
	g_.startX_ = false;
	g_.startO_ = false;
	//TODO
	//odwolanie do bazy danych z update rankingu
	g_.refO_->endedWithWin(startX, startY, finishX , finishY);
	g_.refX_->endedWithLose(startX, startY, finishX , finishY);
}
