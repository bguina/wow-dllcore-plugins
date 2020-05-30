#pragma once

#include "DllPlugin.h"

class DllFolderPlugin : public DllPlugin
{
public:
	DllFolderPlugin();
	DllFolderPlugin(const std::string& watched);
	virtual std::string getTag() const;
	virtual ~DllFolderPlugin();

	virtual std::string getLibraryFolder() const;
	virtual bool loadFolder(const std::wstring& path);
	virtual bool refreshLibrary();

	virtual void freeLibrary() override;
	virtual bool onD3dRender() override;

protected:

	std::wstring mFolder;
	FILETIME mCurrentVersion;
};
