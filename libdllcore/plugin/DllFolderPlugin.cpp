#include "pch.h"

#include "DllFolderPlugin.h"

inline std::ostream& operator<<(
	std::ostream& out,
	const FILETIME& obj
	)
{
	return out << obj.dwHighDateTime << obj.dwLowDateTime;
}

FILETIME currentFtTime() {
	SYSTEMTIME stNewVersionTime;
	FILETIME ftNewVersionTime;

	GetSystemTime(&stNewVersionTime);
	SystemTimeToFileTime(&stNewVersionTime, &ftNewVersionTime);
	return ftNewVersionTime;
}

DllFolderPlugin::DllFolderPlugin(const std::string& tag, bool keepAlive) :
	DllPlugin(tag),
	mKeepAlive(keepAlive),
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
	std::wstring latestLibrary;
	FILETIME latestTs(lookupLatest(path, latestLibrary));

	dbg << FileLogger::info << "freeing any previous library" << FileLogger::normal << std::endl;
	freeLibrary();

	mFolder = path;
	if (!latestLibrary.empty() && loadLibrary(latestLibrary)) {
		mCurrentVersion = latestTs;
		return true;
	}

	dbg << FileLogger::err << "failed to load library" << FileLogger::normal << std::endl;
	return false;
}


bool DllFolderPlugin::refreshLibrary() {
	FileLogger dbg(mDbg, "refreshLibrary");
	std::wstring latestLibrary;
	FILETIME latestTs = lookupLatest(mFolder, latestLibrary);

	if (!latestLibrary.empty() && CompareFileTime(&latestTs, &mCurrentVersion) > 0) {
		dbg << FileLogger::info << " found new library version " << wstringConvertToString(latestLibrary) << FileLogger::normal << std::endl;
		//dbg << FileLogger::verbose << " previous version: " << mCurrentVersion << FileLogger::normal << std::endl;
		//dbg << FileLogger::verbose << " updated  version: " << latestTs << FileLogger::normal << std::endl;

		if (!loadLibrary(latestLibrary)) {
			mCurrentVersion = { 0,0 };
			dbg << FileLogger::err << " failed updating to new library version of " << wstringConvertToString(latestLibrary) << FileLogger::normal << std::endl;
			return false;
		}

		return true;
	}
	return false;
}

bool DllFolderPlugin::loadLibrary(const std::wstring& dllPath) {
	FileLogger dbg(mDbg, "loadLibrary");
	auto targetDup = dllPath.substr(0, dllPath.find_last_of('.')) + L"_lock.dll";
	bool loaded;

	// copy with suffix before injecting
	if (CopyFile(dllPath.c_str(), targetDup.c_str(), 0)) {
		dbg << FileLogger::info << "duplicated " << wstringConvertToString(dllPath) << " to " << wstringConvertToString(targetDup) << FileLogger::normal << std::endl;
		loaded = DllPlugin::loadLibrary(targetDup);
	}
	else {
		dbg << FileLogger::err << "error duplicating " << wstringConvertToString(dllPath) << " to " << wstringConvertToString(targetDup) << FileLogger::normal << std::endl;
		loaded = DllPlugin::loadLibrary(dllPath);
	}

	if (loaded) {
		mCurrentVersion = currentFtTime();
	}
	return loaded;
}

bool DllFolderPlugin::onD3dRender() {
	FileLogger dbg(mDbg, "onD3dRender");

	dbg << FileLogger::debug << "library timestamp: " << mCurrentVersion << FileLogger::normal << std::endl;

	if (!refreshLibrary()) {
		if (isLibraryLoaded() && 0 != mDllOnD3dRenderFunc()) {
			dbg << FileLogger::warn << "plugin ejection" << FileLogger::normal << std::endl;
			return false;
		}
	}
	else {
		dbg << FileLogger::warn << "library changed" << FileLogger::normal << std::endl;
	}

	return mKeepAlive;
}

SYSTEMTIME systemTimeAdd(SYSTEMTIME s, double seconds) {
	FILETIME f;
	ULARGE_INTEGER u;

	SystemTimeToFileTime(&s, &f);
	memcpy(&u, &f, sizeof(u));

	const double c_dSecondsPer100nsInterval = 100. * 1.E-9;
	u.QuadPart += seconds / c_dSecondsPer100nsInterval;

	memcpy(&f, &u, sizeof(f));

	FileTimeToSystemTime(&f, &s);
	return s;
}

FILETIME DllFolderPlugin::lookupLatest(const std::wstring& path, std::wstring& result) {
	FileLogger dbg(mDbg, "lookupLatest " + wstringConvertToString(path));
	std::wstring search_path = path + L"/*.dll";
	FILETIME latestTs = { 0, 0 };
	WIN32_FIND_DATA fd;

	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		SYSTEMTIME stFewSecondsAgo;
		FILETIME ftFewSecondsAgo;
		double fewSecondsDelta = 2;

		GetSystemTime(&stFewSecondsAgo);
		stFewSecondsAgo = systemTimeAdd(stFewSecondsAgo, -fewSecondsDelta);
		SystemTimeToFileTime(&stFewSecondsAgo, &ftFewSecondsAgo);

		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { // ignore subfolders
				bool isNewer = CompareFileTime(&fd.ftLastWriteTime, &latestTs) > 0; // ftLastWriteTime must be lower than a ftFewSecondsAgo
				bool isOldEnough = CompareFileTime(&ftFewSecondsAgo, &fd.ftLastWriteTime) > 0; // ftLastWriteTime must be lower than a ftFewSecondsAgo

				if (false) {
					dbg << FileLogger::debug << "stFewSecondsAgo " << ftFewSecondsAgo << FileLogger::normal << std::endl;
					dbg << FileLogger::debug << "file " << fd.ftLastWriteTime << FileLogger::normal << std::endl;
					dbg << FileLogger::debug << "isNewer " << isNewer << " isOldEnough " << isOldEnough << FileLogger::normal << std::endl;
				}

				if (isNewer && isOldEnough) {
					dbg << FileLogger::debug << "creationTime " << fd.ftLastWriteTime << " > " << latestTs << FileLogger::normal << std::endl;
					latestTs = fd.ftLastWriteTime;
					result = path + L"\\" + fd.cFileName;
				}

			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);

		if (!result.empty())
			dbg << FileLogger::debug << "latest library created at " << latestTs << ": " << wstringConvertToString(result) << FileLogger::normal << std::endl;
		else
			dbg << FileLogger::warn << "folder is empty, no latest library" << FileLogger::normal << std::endl;
	}
	else {
		dbg << FileLogger::err << wstringConvertToString(path) << ": INVALID_HANDLE_VALUE" << FileLogger::normal << std::endl;

	}

	return latestTs;
}

