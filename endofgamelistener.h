#ifndef __EndOfGameListenerh__
#define __EndOfGameListenerh__

//to ja zrobie pozniej bo to ja bede uzywal Ty tylko powadamiaj
class EndOfGameListener
 {
public:
	void gameEnded(FieldEmpty f); //gdy brak mozliwosci ruchu
	void gameEnded(FieldX f); //gdy wygral X
	void gameEnded(FieldO f); //gdy wygral Y
};

#endif
