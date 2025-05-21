#include "TextEncrypter.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

bool CTextEncrypter::LoadImg(const char* path)
{
	int channels;

	if (!stbi_info(path, &m_ImageWidth, &m_ImageHeight, &channels))
		return false;

	m_ImagePath = path;
	size_t DataSize = m_ImageWidth * m_ImageHeight * 3;
	m_ByteData.clear();
	stbi_uc* data = stbi_load(path, &m_ImageWidth, &m_ImageHeight, &channels, 3);
	m_ByteData.assign(data, data + DataSize);

	stbi_image_free(data);

	return true;
}

void CTextEncrypter::AddTextToImage(const char* text)
{
	m_CurrentByte = 0;
	std::uint16_t l = strlen(text);
	WriteToByteData(l);
	const char* p = text;

	while (*p)
		WriteToByteData(*(p++));

	//m_ImagePath = m_ImagePath.substr(0, m_ImagePath.find_last_of(".") + 1) + "bmp";
	int i = std::remove(m_ImagePath.c_str());
	stbi_write_png(m_ImagePath.c_str(), m_ImageWidth, m_ImageHeight, 3, &(m_ByteData[0]), m_ImageWidth * 3);
}

std::string CTextEncrypter::ReadTextFromImage()
{
	m_CurrentByte = 0;
	std::uint16_t TextLength = ReadFromByteData<std::uint16_t>();
	std::string res;

	for (int i = 0; i < TextLength; ++i)
	{
		char c = ReadFromByteData<char>();
		res += c;
	}

	return res;
}
