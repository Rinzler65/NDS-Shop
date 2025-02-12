/*
*   This file is part of NDS-Shop Project
*   Copyright (C) 2024-2025 NDS-Shop Team
*   
*   This program is a free open-source software that allows users
*	to browse and download digital products.  It is based on the
*	code of the Universal-Updater project from Universal-Team.
*   
*   It's distributed under the terms of the GNU General Public
*	License and it's completely free to use and modify.
*   
*   This program comes with no warranty, but we are constantly
*	working on improving its functionality and user experience.
*   
*   If you have any suggestions or find any bugs, please let us know!
*   
*   NDS-Shop Team reserves the right to update the license terms
*	at any time without prior notice.
*   Any changes to the code must be clearly marked as such to avoid confusion.
*/

#include "common.hpp"
#include "fileBrowse.hpp"
#include "meta.hpp"
#include <unistd.h>

/*
	The Constructor of the Meta.

	Includes MetaData file creation, if non existent.
*/
Meta::Meta() {
	if (access(_META_PATH, F_OK) != 0) {
		FILE *temp = fopen(_META_PATH, "w");
		char tmp[2] = { '{', '}' };
		fwrite(tmp, sizeof(tmp), 1, temp);
		fclose(temp);
	}

	FILE *temp = fopen(_META_PATH, "rt");
	if (temp) {
		this->metadataJson = nlohmann::json::parse(temp, nullptr, false);
		fclose(temp);
	}
	if (this->metadataJson.is_discarded())
		this->metadataJson = { };

	if (config->metadata()) this->ImportMetadata();
}

/*
	Import the old Metadata of the 'updates.json' file.
*/
void Meta::ImportMetadata() {
	if (access("sdmc:/3ds/Universal-Updater/updates.json", F_OK) != 0) {
		config->metadata(false);
		return; // Not found.
	}

	Msg::DisplayMsg(Lang::get("FETCHING_METADATA"));

	nlohmann::json oldJson;
	FILE *old = fopen("sdmc:/3ds/Universal-Updater/updates.json", "rt");
	if (old) {
		oldJson = nlohmann::json::parse(old, nullptr, false);
		fclose(old);
	}
	if (oldJson.is_discarded())
		oldJson = { };

	std::vector<UniStoreInfo> info = GetUniStoreInfo(_STORE_PATH); // Fetch UniStores.

	for (int i = 0; i < (int)info.size(); i++) {
		if (info[i].Title != "" && oldJson.contains(info[i].FileName)) {
			for(auto it = oldJson[info[i].FileName].begin(); it != oldJson[info[i].FileName].end(); ++it) {
				this->SetUpdated(info[i].Title, it.key().c_str(), it.value().get<std::string>());
			}
		}
	}

	config->metadata(false);
}

/*
	Get Last Updated.

	const std::string &unistoreName: The UniStore name.
	const std::string &entry: The Entry name.
*/
std::string Meta::GetUpdated(const std::string &unistoreName, const std::string &entry) const {
	if (!this->metadataJson.contains(unistoreName)) return ""; // UniStore Name does not exist.

	if (!this->metadataJson[unistoreName].contains(entry)) return ""; // Entry does not exist.

	if (!this->metadataJson[unistoreName][entry].contains("updated")) return ""; // updated does not exist.

	if (this->metadataJson[unistoreName][entry]["updated"].is_string()) return this->metadataJson[unistoreName][entry]["updated"];
	return "";
}

/*
	Get the marks.

	const std::string &unistoreName: The UniStore name.
	const std::string &entry: The Entry name.
*/
int Meta::GetMarks(const std::string &unistoreName, const std::string &entry) const {
	int temp = 0;

	if (!this->metadataJson.contains(unistoreName)) return temp; // UniStore Name does not exist.

	if (!this->metadataJson[unistoreName].contains(entry)) return temp; // Entry does not exist.

	if (!this->metadataJson[unistoreName][entry].contains("marks")) return temp; // marks does not exist.

	if (this->metadataJson[unistoreName][entry]["marks"].is_number()) return this->metadataJson[unistoreName][entry]["marks"];
	return temp;
}

/*
	Return, if update available.

	const std::string &unistoreName: The UniStore name.
	const std::string &entry: The Entry name.
	const std::string &updated: Compare for the update.
*/
bool Meta::UpdateAvailable(const std::string &unistoreName, const std::string &entry, const std::string &updated) const {
	if (this->GetUpdated(unistoreName, entry) != "" && updated != "") {
		return strcasecmp(updated.c_str(), this->GetUpdated(unistoreName, entry).c_str()) > 0;
	}

	return false;
}

/*
	Get the marks.

	const std::string &unistoreName: The UniStore name.
	const std::string &entry: The Entry name.
*/
std::vector<std::string> Meta::GetInstalled(const std::string &unistoreName, const std::string &entry) const {
	if (!this->metadataJson.contains(unistoreName)) return { }; // UniStore Name does not exist.

	if (!this->metadataJson[unistoreName].contains(entry)) return { }; // Entry does not exist.

	if (!this->metadataJson[unistoreName][entry].contains("installed")) return { }; // marks does not exist.

	if (this->metadataJson[unistoreName][entry]["installed"].is_array()) return this->metadataJson[unistoreName][entry]["installed"];
	return { };
}

/*
	The save call.

	Write to file.. called on destructor.
*/
void Meta::SaveCall() {
	FILE *file = fopen(_META_PATH, "wb");
	const std::string dump = this->metadataJson.dump(1, '\t');
	fwrite(dump.c_str(), 1, dump.size(), file);
	fclose(file);
}