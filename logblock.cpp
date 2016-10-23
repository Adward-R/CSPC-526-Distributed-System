#include "logblock.hpp"

Logblock::iterator::iterator(int _i, Logblock *_lb){
	i = _i;
	lb = _lb;
}

bool Logblock::iterator::end(){
	return i + LOG_ENTRY_SIZE > lb->n * LOG_ENTRY_SIZE;
}

void Logblock::iterator::next(){
	i += LOG_ENTRY_SIZE;
}

Logentry* Logblock::iterator::entry(){
	return (Logentry*)(lb->entry_area + i);
}

Logblock::iterator Logblock::begin(){
	return Logblock::iterator(0, this);
}

