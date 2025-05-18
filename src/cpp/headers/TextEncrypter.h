#pragma once

#include <cstdint>
#include <vector>
#include <string>

class CTextEncrypter
{
public:
	bool LoadImg(const char* path);
	void AddTextToImage(const char* text);
	bool AddCharToByteData(char chr);
	bool AddIntToByteData(std::uint8_t n);
	
	std::uint8_t ReadIntFromByteData();
	char ReadCharFromByteData();
	std::string ReadTextFromImage();

	void ResetCursor() { m_CurrentByte = 0; }

private:
	std::vector<std::uint8_t> m_ByteData;
	size_t m_CurrentByte = 0;
};