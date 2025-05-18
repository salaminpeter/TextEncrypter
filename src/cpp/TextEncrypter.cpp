#include "TextEncrypter.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

bool CTextEncrypter::LoadImg(const char* path)
{
	int width, height, channels;

	if (!stbi_info(path, &width, &height, &channels))
		return false;

	size_t DataSize = width * height * std::max(3, channels);
	m_ByteData.clear();
	stbi_uc* data = stbi_load(path, &width, &height, &channels, 3);
	m_ByteData.assign(data, data + DataSize);

	return data;
}

bool CTextEncrypter::AddCharToByteData(char chr)
{
	if (m_CurrentByte + 8 >= m_ByteData.size())
		return false;

	for (int i = 0; i < 8; ++i)
	{
		std::uint8_t bit = ((chr >> (7 - i)) & 1);

		if (bit == 0)
			m_ByteData[m_CurrentByte] &= (bit | 254);
		else
			m_ByteData[m_CurrentByte] |= (bit | 254);

		++m_CurrentByte;
	}

	return true;
}

bool CTextEncrypter::AddIntToByteData(std::uint8_t n)
{
	if (m_CurrentByte + 8 >= m_ByteData.size())
		return false;

	for (int i = 0; i < 8; ++i)
	{
		std::uint8_t bit = ((n >> (7 - i)) & 1);

		if (bit == 0)
			m_ByteData[m_CurrentByte] &= (bit | 254);
		else
			m_ByteData[m_CurrentByte] |= (bit | 254);

		++m_CurrentByte;
	}

	return true;
}


void CTextEncrypter::AddTextToImage(const char* text)
{
	m_CurrentByte = 0;
	std::uint8_t l = strlen(text);
	AddIntToByteData(l);
	const char* p = text;

	while (*p)
		AddCharToByteData(*(p++));
}

std::uint8_t CTextEncrypter::ReadIntFromByteData()
{
	m_CurrentByte = 0;
	std::uint8_t res = 0;

	for (int i = 0; i < 8; ++i)
	{
		std::uint8_t bit = m_ByteData[m_CurrentByte++] & 1;
		std::uint8_t bit1 = bit << (7 - i);
		res |= bit1;
	}

	return res;
}

char CTextEncrypter::ReadCharFromByteData()
{
	char res = 0;

	for (int i = 0; i < 8; ++i)
	{
		std::uint8_t bit = m_ByteData[m_CurrentByte++] & 1;
		std::uint8_t bit1 = bit << (7 - i);
		res |= bit1;
	}

	return res;
}

std::string CTextEncrypter::ReadTextFromImage()
{
	m_CurrentByte = 0;
	std::uint8_t TextLength = ReadIntFromByteData();
	std::string res;

	for (int i = 0; i < TextLength; ++i)
	{
		char c = ReadCharFromByteData();
		res += c;
	}

	return res;
}
