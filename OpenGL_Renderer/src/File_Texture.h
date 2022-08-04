#pragma once
#include "Texture.h"

class File_Texture : public Texture 
{
private:
	unsigned char* m_DataChar = nullptr;
	float* m_DataFloat = nullptr;
public:
	File_Texture(const std::string& filePath, bool gammaCorrected);
	File_Texture(std::string hdrPath);
	~File_Texture();

};

