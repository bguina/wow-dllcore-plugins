#include "pch.h"

#include "DllFolderPlugin.h"

FILETIME lookupLatest(const std::wstring& path, std::wstring& result) {
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
					result = path + L"\\" + fd.cFileName;
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

DllFolderPlugin::~DllFolderPlugin() {
}

std::string DllFolderPlugin::getTag() const {
	return "DllFolderPlugin[" + getLibraryPath() + ":" + std::to_string(getLibraryVersion()) + "]";
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

	dbg << "call" << std::endl;
	if (isLibraryLoaded()) {
		dbg << FileLogger::info << "freeing previous library" << FileLogger::normal << std::endl;
		freeLibrary();
	}

	if (loadLibrary(latestFile)) {
		mFolder = path;
		mCurrentVersion = latestTs;
		return true;
	}

	dbg << FileLogger::err << "failed to load library" << FileLogger::normal << std::endl;
	return false;
}

bool DllFolderPlugin::refreshLibrary() {
	FileLogger dbg(mDbg, "refreshLibrary");
	std::wstring latestLibrary;
	auto latestTs = lookupLatest(mFolder, latestLibrary);

	if (!latestLibrary.empty() && CompareFileTime(&latestTs, &mCurrentVersion) > 0) {
		dbg << FileLogger::info << " found new library version " << wstringConvertToString(latestLibrary) << FileLogger::normal << std::endl;

		if (!loadLibrary(latestLibrary)) {
			dbg << FileLogger::err << " failed updating to new library version of " << wstringConvertToString(latestLibrary) << FileLogger::normal << std::endl;

			mCurrentVersion = { 0,0 };
			return false;
		}

		mCurrentVersion = latestTs;
		return true;
	}

	return false;
}

bool DllFolderPlugin::onD3dRender() {
	FileLogger dbg(mDbg, "onD3dRender");
	if (isLibraryLoaded()) {

	/*	if (!refreshLibrary()) {*/

			if (0 != mDllOnD3dRenderFunc()) {
				dbg << FileLogger::warn << "plugin ejection" << FileLogger::normal << std::endl;
				return false;
			}
		//}
		//else
			//dbg << FileLogger::warn << "library changed" << FileLogger::normal << std::endl;

		return true;
	} else 
		dbg << FileLogger::warn << "no library loaded" << FileLogger::normal << std::endl;

	return false;
}
