#pragma once

#include "DllPlugin.h"

class DllFolderPlugin : public DllPlugin
{
public:
	DllFolderPlugin();
	virtual ~DllFolderPlugin();

	virtual bool loadFolder(const std::wstring& path);
	virtual bool refreshLibrary();
	virtual void freeLibrary() override;

	virtual void onD3dRender() override;

protected:

	std::wstring mFolder;
	FILETIME mCurrentVersion;
};
