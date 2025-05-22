#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

class CTextEncrypter
{
public:
	bool LoadImg(const char* path);

	void AddTextToImage(std::u16string text);
	std::u16string ReadTextFromImage();

	void ResetCursor() { m_CurrentByte = 0; }

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
	std::vector<std::uint8_t> m_ByteData;
	int m_ImageWidth;
	int m_ImageHeight;
	std::string m_ImagePath;
	size_t m_CurrentByte = 0;
};