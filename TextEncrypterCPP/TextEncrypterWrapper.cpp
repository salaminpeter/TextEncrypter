#include "TextEncrypter.h"
#include <emscripten.h>
#include <string>

static CTextEncrypter Encrypter;

extern "C" {

	EMSCRIPTEN_KEEPALIVE
	bool LoadImage(const char* path) {
		return Encrypter.LoadImg(path);
	}

	EMSCRIPTEN_KEEPALIVE
	void AddText(const char16_t* text, int length) {
		std::u16string utext(text, text + length);
		Encrypter.AddTextToImage(utext);
	}

	EMSCRIPTEN_KEEPALIVE
	int ReadText(char16_t* outBuffer, int maxLength) {
		std::u16string result = Encrypter.ReadTextFromImage();
		int len = std::min((int)result.length(), maxLength);
		std::copy(result.begin(), result.begin() + len, outBuffer);
		return len;
	}

}
