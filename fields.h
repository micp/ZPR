#ifndef __Fieldsh__
#define __Fieldsh__

//absrakcyjna bazowa
class Field
{}; 

//te trzy modyfikuj do woli tylko nazwy zostaw takie same bo bede wykorzystywane do rysowania w widoku
class FieldX: public Field
{};

class FieldO: public Field
{};

class FieldEmpty: public Field
{};

//nie wiem co tu bedzie potrzebne dodaj co uwazasz za sluszne
class FieldTakenException {};
#endif
