/*
* Data structure used create a text input using the external api 
* of a numwork calculator.
*/

#ifndef TEXTBOX
#define TEXTBOX

#include "eadkpp.h"
#include "palette.h"
#include "customstr.h"
#include "keyboard.h"
#include <math.h>

using namespace std;

const int height = EADK::Screen::Height;
const int width = EADK::Screen::Width;
const int bigCharWidth = 10;
const int bigCharHeight = 17;
const int littleCharWidth = 7;
const int littleCharHeight = 11;
const int maxContentLenght = 32;

/*Convert an int to a char array (decimal notation)
*/
void intToChar(int t, char* arr) {
	if (t == 0) {
		arr[0] = '0';
		arr[1] = '\0';
		return;
	}
	bool negative = false;
	if (t < 0) {
		negative = true;
		t = -t;
	}
	int i = 0;
	while (t > 0) {
		arr[i++] = t % 10 + '0';
		t /= 10;
	}
	if (negative) {
		arr[i++] = '-';
	}
	arr[i] = '\0';
	reverse(arr, arr + i);
}

/*Convert a char array to an int (decimal notation)
*/
int charToInt(char* arr) {
	int n = strlen(arr);
	bool negative = false;
	int result = 0;
	int i = 0;
	if (arr[0] == '-') {
		negative = true;
		i = 1;
	}
	for (; i < n; i++) {
		int digit = arr[i] - '0';
		result += digit * pow(10, n - i - 1);
	}
	return negative ? -result : result;
}

class TextBox
{
private:
	CustomSTR content; //Content of the textbox
	bool writing; //Flag for the writing loop
	bool bigChar; //True if the textbox uses big letters, false otherwise
	int cursorOffset; //In other words, the width of a letter
	int cursorHeight; //The height of the cursor
	Rect rect = Rect(0, 0, EADK::Screen::Width, 20); //Dimensions of the textbox
	uint16_t cooldown_time = 900; //Min time between two actions if the buttons stay pressed
	uint16_t maxLenght = 23; //Maximum number of char the textbox can display

public:
	TextBox() {
		writing = false;
		bigChar = true;
		content = CustomSTR("Hello world");
		cursorOffset = bigCharWidth;
	}
	TextBox(char* content_, bool bigChar_, EADK::Point pos, int maxLenght_) {

		content = CustomSTR(content_);
		writing = false;
		bigChar = bigChar_;
		cursorOffset = bigChar ? bigCharWidth : littleCharWidth;
		cursorHeight = (bigChar ? bigCharHeight : littleCharHeight)+2;
		maxLenght = maxLenght_;
		rect = Rect(pos.x(), pos.y(), cursorOffset * maxLenght + 2, cursorHeight +4);
	}
	TextBox(int content_, bool bigChar_, EADK::Point pos, int maxLenght_) {
		writing = false;
		char buffer[maxContentLenght + 1];
		intToChar(content_, buffer);
		content = CustomSTR(buffer);
		bigChar = bigChar_;
		cursorOffset = bigChar ? bigCharWidth : littleCharWidth;
		cursorHeight = (bigChar ? bigCharHeight : littleCharHeight) + 2;
		maxLenght = maxLenght_;
		rect = Rect(pos.x(), pos.y(), cursorOffset * maxLenght + 2, cursorHeight + 4);
	}

	void set(int content_) {
		char buffer[maxContentLenght + 1];
		intToChar(content_, buffer);
		content = CustomSTR(buffer);
	}

	void set(char* content_)
	{
		content = CustomSTR(content_);
	}

	void draw(int cursorPos, CustomSTR content_, int charOffset) {
		char buffer[maxContentLenght + 1];
		content_.window(charOffset, charOffset + maxLenght-1, buffer, sizeof(buffer));
		EADK::Display::pushRectUniform(rect, EADK::Color(0xd0d0d0));
		EADK::Display::drawString(buffer, EADK::Point(rect.x() + 2, rect.y()+2), bigChar, PAL::Black, EADK::Color(0xd0d0d0));
		EADK::Display::pushRectUniform(EADK::Rect(rect.x() + 2 + (cursorPos * cursorOffset), rect.y() + 2, 1, cursorHeight), PAL::Black);
	}

	void reDrawCursor(int cursorPos, int prev_cursorPos)
	{
		EADK::Display::pushRectUniform(EADK::Rect(rect.x() + 2 + (cursorPos * cursorOffset), rect.y() + 2, 1, cursorHeight), PAL::Black);
		EADK::Display::pushRectUniform(EADK::Rect(rect.x() + 2 + (prev_cursorPos * cursorOffset), rect.y() + 2, 1, cursorHeight), EADK::Color(0xd0d0d0));
	}

	/* Enter in writing mode : listen to user inputs and update the textbox until the 'ok' button is pressed.
	* When the 'ok' button is pressed, return the content of the textbox as a char array.
	*/
	char* writingMode() {
		int timer = 0;
		writing = true;
		bool contentChanged = true;
		uint16_t cooldown = 0;
		int charOffset = 0; //In case of a string longer than the size of the textbox, this is the index of the fisrt char showed in the textbox (starting from 0)

		int cursorPos = (content.length()); //Position of the cusor in the textbox

		EADK::Timing::msleep(500);
		while (writing)
		{
			EADK::Keyboard::State state = EADK::Keyboard::scan();
			char t = getCharFromState(state);
			if (state.keyDown(EADK::Keyboard::Key::OK))
			{
				writing = false;	
			}
			else if (state.keyDown(EADK::Keyboard::Key::Left) && cooldown == 0)
			{
				if (cursorPos > 0)
				{
					reDrawCursor(cursorPos - 1, cursorPos);
					cursorPos--;
					cooldown = cooldown_time;
				}
				else if (charOffset > 0)
				{
					charOffset--;
					contentChanged = true;
					cooldown = cooldown_time;
				}
			}
			else if (state.keyDown(EADK::Keyboard::Key::Right) && cooldown == 0)
			{
				if (cursorPos < maxLenght && cursorPos < content.length())
				{
					reDrawCursor(cursorPos + 1, cursorPos);
					cursorPos++;
					cooldown = cooldown_time;
				}
				else if (maxLenght + charOffset < content.length())
				{
					charOffset++;
					contentChanged = true;
					cooldown = cooldown_time;
				}
			}
			else if(state.keyDown(EADK::Keyboard::Key::Backspace) && cooldown == 0)
			{
				if (cursorPos > 0)
				{
					content.erase(cursorPos-1+charOffset, 1);
					contentChanged = true;
					cursorPos -= 1;
					cooldown = cooldown_time;
				}
			}
			else if (t !='\0' && cooldown == 0 && content.length() < maxContentLenght)
			{
				content.insert(t, cursorPos + charOffset);
				contentChanged = true;
				cooldown = cooldown_time;
				if (cursorPos == maxLenght)
				{
					charOffset++;
				}
				else
				{
					cursorPos += 1;
				}
			}
			else if (cooldown < cooldown_time-100)
			{
				cooldown = 0;
			}

			if (contentChanged)
			{
				draw(cursorPos, content, charOffset);
				contentChanged = false;
			}
			EADK::Timing::msleep(20);
			timer += 20;
			if (cooldown > 0) cooldown -= 20;
		}
		return content.get();
	}
};



#endif // !TEXTBOX
