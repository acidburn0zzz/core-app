#ifndef DOCX_SERIALIZER
#define DOCX_SERIALIZER

//todo
#include <atlbase.h>
#include <atlstr.h>

#include <string>

namespace BinDocxRW{
class BinaryFileWriter;
}
namespace Writers{
class FileWriter;
}

namespace BinDocxRW {

	class CDocxSerializer{
	private:
		std::wstring m_sFontDir;
		std::wstring m_sEmbeddedFontsDir;
		bool m_bIsNoBase64Save;
		bool m_bSaveChartAsImg;
		BinDocxRW::BinaryFileWriter* m_oBinaryFileWriter;
		Writers::FileWriter* m_pCurFileWriter;
	public:
		CDocxSerializer();
		bool loadFromFile(std::wstring& sSrcFileName, std::wstring& sDstPath, std::wstring& sXMLOptions, std::wstring& sThemePath, std::wstring& sMediaPath);
		bool saveToFile(std::wstring& sSrcFileName, std::wstring& sDstPath, std::wstring& sXMLOptions);
		bool GetXmlContent(unsigned char* pBinaryObj, long lSize, long lStart, long lLength, std::wstring& sOutputXml);
		bool GetBinaryContent(std::wstring& bsTxContent, unsigned char** ppBinary, long& lDataSize);
		void setFontDir(std::wstring& sFontDir);
		void setEmbeddedFontsDir(std::wstring& sEmbeddedFontsDir);
		void setIsNoBase64Save(bool bIsNoBase64Save);
		void setSaveChartAsImg(bool bSaveChartAsImg);

		//todo
#ifdef _WIN32
		IUnknown* m_pInterface;
		void setComInterface(IUnknown* pInterface);
#endif
	};
}
#endif	// #ifndef DOCX_SERIALIZER