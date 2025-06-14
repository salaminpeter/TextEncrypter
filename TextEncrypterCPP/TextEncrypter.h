#pragma once

#include <cstdint>
#include <vector>
#include <string>

class CTextEncrypter
{
public:
	bool LoadImg(const char* path);
	void SetByteData(std::uint8_t* data, size_t size);

	void AddTextToImage(std::u16string text);
	std::u16string ReadTextFromImage();
	bool HasMessage();

	template <typename T>
	bool WriteToByteData(T data)
	{
		if (m_CurrentByte + sizeof(T) * 8 >= m_ByteData.size())
			return false;

		
		for (int j = 0; j < sizeof(T); ++j)
		{
			uint8_t CurrentData = (data >> (j * 8)) & 255;

			for (int i = 0; i < 8; ++i)
			{
				T bit = ((CurrentData >> (7 - i)) & 1);

				if (bit == 0)
					m_ByteData[m_CurrentByte] &= (bit | 254);
				else
					m_ByteData[m_CurrentByte] |= (bit | 254);

				++m_CurrentByte;
			}
		}
	}

	template <typename T>
	T ReadFromByteData()
	{
		T res = 0;

		for (int j = 0; j < sizeof(T); ++j)
		{
			for (int i = 0; i < 8; ++i)
			{
				T bit = m_ByteData[m_CurrentByte++] & 1;
				T bit1 = bit << ((j * 8 + 7 - i));
				res |= bit1;
			}
		}

		return res;
	}

private:
	void EncodeTextInByteData(std::u16string text);
	void AddMessageTag();

private:
	const std::u16string m_Tag = u"[msg]";
	std::vector<std::uint8_t> m_ByteData;
	int m_ImageWidth;
	int m_ImageHeight;
	std::string m_ImagePath;
	size_t m_CurrentByte = 0;
};