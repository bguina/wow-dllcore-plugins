#include "pch.h"

#include "DllFolderPlugin.h"

FILETIME lookupLatest(const std::wstring& path, std::wstring& latest) {
	std::wstring search_path = path + L"/*.dll";
	WIN32_FIND_DATA fd;
	FILETIME latestTs = { 0,0 };

	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				if (CompareFileTime(&fd.ftLastWriteTime, &latestTs) > 0) {
					latestTs = fd.ftCreationTime;
					latest = path + L"\\" + fd.cFileName;
				}
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return latestTs;
}

DllFolderPlugin::DllFolderPlugin() :
	DllPlugin(),
	mCurrentVersion({ 0,0 })
{
}

DllFolderPlugin::DllFolderPlugin(const std::string& watched) :
	DllFolderPlugin()
{
	loadFolder(stringConvertToWstring(watched));
}

DllFolderPlugin::~DllFolderPlugin() {

}

std::string DllFolderPlugin::getTag() const {
	return "DllFolderPlugin[" + getLibraryFolder() + "/" + getLibraryName() + ":" + std::to_string(getLibraryVersion()) + "]";
}

std::string DllFolderPlugin::getLibraryFolder() const {
	return wstringConvertToString(mFolder);
}

void DllFolderPlugin::freeLibrary() {
	DllPlugin::freeLibrary();
	mCurrentVersion = { 0,0 };
}

bool DllFolderPlugin::loadFolder(const std::wstring& path) {
	FileLogger dbg(mDbg, "loadFolder");
	std::wstring latestFile;
	FILETIME latestTs(lookupLatest(path, latestFile));

	dbg << "loadFolder" << std::endl;
	if (isLibraryLoaded())
		freeLibrary();

	if (!loadLibrary(latestFile)) {
		mFolder = path;
		mCurrentVersion = latestTs;
		return true;
	}
	mFolder = std::wstring();
	return false;
}

bool DllFolderPlugin::refreshLibrary() {
	std::wstring latestFile;
	auto latestTs = lookupLatest(mFolder, latestFile);

	if (!latestFile.empty() && CompareFileTime(&latestTs, &mCurrentVersion) > 0) {
		mDbg << FileLogger::info << " found new library version " << FileLogger::normal << std::endl;
		mDbg.flush();

		mCurrentVersion = { 0,0 };
		if (!loadLibrary(latestFile)) {
			mDbg << FileLogger::err << " failed updating to new library version " << FileLogger::normal << std::endl;
			return false;
		}

		mCurrentVersion = latestTs;
	}

	return true;
}

bool DllFolderPlugin::onD3dRender() {
	if (!refreshLibrary()) {
		DllPlugin::onD3dRender();
	}

	mDbg.flush();
	return isLibraryLoaded();
}
