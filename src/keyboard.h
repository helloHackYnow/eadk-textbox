#ifndef NW_KEYBOARDS
#define NW_KEYBOARDS

#include "eadkpp.h"

using namespace EADK;
using namespace Keyboard;
/*
* Return the char corresponding to a key (if it's pressed).
* If none is pressed, it return the null char '\0'.
* Work only for numbers at the moment as I'm working on a more efficient solution before implementing the rest.
*/
char getCharFromState(State state)
{
	if (state.keyDown(Key::Zero))
	{
		return '0';
	}
	else if(state.keyDown(Key::One))
	{
		return '1';
	}
	else if (state.keyDown(Key::Two))
	{
		return '2';
	}
	else if (state.keyDown(Key::Three))
	{
		return '3';
	}
	else if (state.keyDown(Key::Four))
	{
		return '4';
	}
	else if (state.keyDown(Key::Five))
	{
		return '5';
	}
	else if (state.keyDown(Key::Six))
	{
		return '6';
	}
	else if (state.keyDown(Key::Seven))
	{
		return '7';
	}
	else if (state.keyDown(Key::Eight))
	{
		return '8';
	}
	else if (state.keyDown(Key::Nine))
	{
		return '9';
	}
	return '\0';
}



#endif // !NW_KEYBOARDS
