//
//  WindowsAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WindowsAssetHandler.hpp"

#include <Windows.h>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <assert.h>

FileData WindowsAssetHandler::loadAsset(const char* filePath)
{
	assert(filePath != NULL);

	wchar_t* name = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, filePath, -1, name, 4096);

	std::ifstream fileStream(name, std::ios::in | std::ios::binary | std::ios::ate);

	if (!fileStream)
	{
		wchar_t moduleName[_MAX_PATH];

		assert(GetModuleFileNameW(NULL, moduleName, _MAX_PATH));

		wchar_t drive[_MAX_DRIVE];
		wchar_t path[_MAX_PATH];

		assert(!_wsplitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, 0, NULL, 0));

		wchar_t filename[_MAX_PATH];
		assert(!_wmakepath_s(filename, _MAX_PATH, drive, path, name, NULL));

		fileStream.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
	}

	assert(fileStream);

	std::streampos len = fileStream.tellg();
	assert(fileStream);

	std::vector<uint8_t>* blob = new std::vector<uint8_t>();
	blob->resize(size_t(len));

	fileStream.seekg(0, std::ios::beg);
	assert(fileStream);

	fileStream.read(reinterpret_cast<char*>(blob->data()), len);
	assert(fileStream);

	fileStream.close();

	delete[] name;

	return FileData(blob->size(), (const char*)blob->data(), (const void*)blob);
}

void WindowsAssetHandler::releaseAsset(const FileData& fileData)
{
	assert(fileData._fileHandle != NULL);

    const std::vector<uint8_t>* handle = static_cast<const std::vector<uint8_t>*>(fileData._fileHandle);
    delete handle;
}

WindowsAssetHandler::WindowsAssetHandler() : AssetHandler()
{
    // Empty
}

WindowsAssetHandler::~WindowsAssetHandler()
{
    // Empty
}
