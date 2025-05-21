#pragma once

#include "TextEncrypter.h"

using namespace System;

namespace TextEncrypterCLR {
	public ref class CTextEncrypterWrapper
	{
	public:
		CTextEncrypterWrapper()
		{
			m_TextEncrypter = new CTextEncrypter;
		}

		~CTextEncrypterWrapper()
		{
			this->!CTextEncrypterWrapper();
		}

		!CTextEncrypterWrapper()
		{
			delete m_TextEncrypter;
			m_TextEncrypter = nullptr;
		}

		bool LoadImg(String^ path)
		{
			msclr::interop::marshal_context ctx;
			const char* NativePath = ctx.marshal_as<const char*>(path);
			return m_TextEncrypter->LoadImg(NativePath);
		}

		void AddTextToImage(String^ text)
		{
			msclr::interop::marshal_context ctx;
			const char* NativeText = ctx.marshal_as<const char*>(text);
			m_TextEncrypter->AddTextToImage(NativeText);
		}

		String^ ReadTextFromImage()
		{
			std::string NativeText = m_TextEncrypter->ReadTextFromImage();
			return gcnew String(NativeText.c_str());
		}

	private:
		CTextEncrypter* m_TextEncrypter;
		// TODO: Add your methods for this class here.
	};
}
