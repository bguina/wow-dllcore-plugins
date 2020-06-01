#pragma once

#include "DllPlugin.h"

class DllFolderPlugin : public DllPlugin
{
public:
	DllFolderPlugin(const std::string& tag, bool keepAlive);
	virtual std::string getTag() const;
	virtual ~DllFolderPlugin();

	virtual std::string getLibraryFolder() const;
	virtual bool loadFolder(const std::wstring& path);
	virtual bool refreshLibrary();

	virtual void freeLibrary() override;
	virtual bool onD3dRender() override;

protected:
	virtual bool loadLibrary(const std::wstring& dllPath) override;
	FILETIME lookupLatest(const std::wstring& path, std::wstring& result);

	bool mKeepAlive;
	std::wstring mFolder;
	FILETIME mCurrentVersion;
};
