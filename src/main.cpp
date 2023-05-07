#include "eadkpp.h"
#include "textbox.h"
#include "palette.h"

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "TextBox";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

#define WIDTH EADK::Screen::Width
#define HEIGHT EADK::Screen::Height

int main(int argc, char* argv[]) {
	EADK::Timing::msleep(30);
	TextBox textbox("text", true, EADK::Point(50, HEIGHT - 50), 23);
	while (1)
	{
		EADK::Display::pushRectUniform(EADK::Screen::Rect, PAL::Black);
		//EADK::Display::drawString(text, EADK::Point(50, 50), true, PAL::White, PAL::Black);
		EADK::Keyboard::State state = EADK::Keyboard::scan();

		if (state.keyDown(EADK::Keyboard::Key::Home))
		{
			return 0;
		}
		else if (state.keyDown(EADK::Keyboard::Key::OK))
		{
			textbox.writingMode();
		}
		EADK::Timing::msleep(100);
	}
}