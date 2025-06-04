#include "TextEncrypter.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ImageLib/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageLib/stb_image_write.h"

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

void CTextEncrypter::SetByteData(std::uint8_t* data, size_t size)
{
	m_ByteData.clear();
	m_ByteData.assign(data, data + size);
}

void CTextEncrypter::AddMessageTag()
{
	m_CurrentByte = 0;
	EncodeTextInByteData(m_Tag);
}

void CTextEncrypter::EncodeTextInByteData(std::u16string text)
{
	std::uint16_t l = text.length();

	for (size_t i = 0; i < l; ++i)
		WriteToByteData(text[i]);
}

bool CTextEncrypter::HasMessage()
{
	m_CurrentByte = 0;
	std::u16string str;

	for (int i = 0; i < m_Tag.length(); ++i)
	{
		char16_t c = ReadFromByteData<char16_t>();
		str += c;
	}

	return (str == m_Tag);
}

void CTextEncrypter::AddTextToImage(std::u16string text)
{
	AddMessageTag();

	std::uint16_t l = text.length();
	WriteToByteData(l);
	
	EncodeTextInByteData(text);

	int i = std::remove(m_ImagePath.c_str());
	stbi_write_png(m_ImagePath.c_str(), m_ImageWidth, m_ImageHeight, 3, &(m_ByteData[0]), m_ImageWidth * 3);
}

std::u16string CTextEncrypter::ReadTextFromImage()
{
	if (!HasMessage())
		return u"";

	std::uint16_t TextLength = ReadFromByteData<std::uint16_t>();
	std::u16string res;

	for (int i = 0; i < TextLength; ++i)
	{
		char16_t c = ReadFromByteData<char16_t>();
		res += c;
	}

	return res;
}
