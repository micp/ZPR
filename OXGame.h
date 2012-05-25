#ifndef __OXGameh__
#define __OXGameh__

#include "fields.h"
#include "endofgamelistener.h"
#include <set>
#include "boost/multi_array.hpp"

//TODO michal
//wciskaj wzorce projektowe i jakies sztuczki programistyczne gdzie tylko sie da bo jak nie tu to gdzie?^^
class OXGame
{
public:
	//TODO michal
	OXGame(int size = 15);
	//TODO michal
	~OXGame();
	//TODO michal
	//startuje nowa gre	
	void startNewGame();
	//TODO michal
	//wstawia obiekt field w dane miejsce, jesli w tym miejscu jest cos innego niz puste pole to wyjatek
	//jesli po wstawieniu znajdzie sie 5 w lini to powiadamiasz action listenera	
	//sprawdzanie zrob na wzorcu wizytatora zeby bylo ciekawiej
	//jak brak mozliwosci ruchu to powiadomic tez
	void put(Field &f , int x , int y) throw FieldTakenException;

	//TODO michal
	void addEndOfGameListener( EndOfGameListener &l );

	//TODO michal
	const Field& getField(int x , int y);

	//TODO michal
	int getSize();

	private:
	boost::multi_array<Field* , 2> fields_;	
	//zbior wyeliminuje powtorzenia bo nie ma sensu kogos 2 razy powiadamiac
	std::set<EndOfGameListener *> listeners_;
	int size_; //obieramy size 15 ale zrob zeby dzialalo dla kazdego
};
#endif
