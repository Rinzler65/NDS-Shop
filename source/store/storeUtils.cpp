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
#include "queueSystem.hpp"
#include "storeUtils.hpp"

std::unique_ptr<Meta> StoreUtils::meta = nullptr;
std::unique_ptr<Store> StoreUtils::store = nullptr;
std::vector<std::unique_ptr<StoreEntry>> StoreUtils::entries;

/*
	Compare Title.

	const std::unique_ptr<StoreEntry> &a: Const Reference to Entry A.
	const std::unique_ptr<StoreEntry> &b: Const Reference to Entry B.
*/
bool StoreUtils::compareTitleDescending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(a->GetTitle()).c_str(), StringUtils::lower_case(b->GetTitle()).c_str()) > 0;

	return true;
}
bool StoreUtils::compareTitleAscending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(b->GetTitle()).c_str(), StringUtils::lower_case(a->GetTitle()).c_str()) > 0;

	return true;
}

/*
	Compare Author.

	const std::unique_ptr<StoreEntry> &a: Const Reference to Entry A.
	const std::unique_ptr<StoreEntry> &b: Const Reference to Entry B.
*/
bool StoreUtils::compareAuthorDescending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(a->GetAuthor()).c_str(), StringUtils::lower_case(b->GetAuthor()).c_str()) > 0;

	return true;
}
bool StoreUtils::compareAuthorAscending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(b->GetAuthor()).c_str(), StringUtils::lower_case(a->GetAuthor()).c_str()) > 0;

	return true;
}

/*
	Compare Last Updated.

	const std::unique_ptr<StoreEntry> &a: Const Reference to Entry A.
	const std::unique_ptr<StoreEntry> &b: Const Reference to Entry B.
*/
bool StoreUtils::compareUpdateDescending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(a->GetLastUpdated()).c_str(), StringUtils::lower_case(b->GetLastUpdated()).c_str()) > 0;

	return true;
}
bool StoreUtils::compareUpdateAscending(const std::unique_ptr<StoreEntry> &a, const std::unique_ptr<StoreEntry> &b) {
	if (a && b) return strcasecmp(StringUtils::lower_case(b->GetLastUpdated()).c_str(), StringUtils::lower_case(a->GetLastUpdated()).c_str()) > 0;

	return true;
}

/*
	Sort the entries.

	bool Ascending: If Ascending.
	SortType sorttype: The sort type.
*/
void StoreUtils::SortEntries(bool Ascending, SortType sorttype) {
	switch(sorttype) {
		case SortType::TITLE:
			Ascending ? std::sort(StoreUtils::entries.begin(), StoreUtils::entries.end(), StoreUtils::compareTitleAscending) : std::sort(StoreUtils::entries.begin(), StoreUtils::entries.end(), StoreUtils::compareTitleDescending);
			break;

		case SortType::AUTHOR:
			Ascending ? std::sort(StoreUtils::entries.begin(), StoreUtils::entries.end(), StoreUtils::compareAuthorAscending) : std::sort(StoreUtils::entries.begin(), StoreUtils::entries.end(), StoreUtils::compareAuthorDescending);
			break;

		case SortType::LAST_UPDATED:
			Ascending ? std::sort(StoreUtils::entries.begin(), StoreUtils::entries.end(), StoreUtils::compareUpdateAscending) : std::sort(StoreUtils::entries.begin(), StoreUtils::entries.end(), StoreUtils::compareUpdateDescending);
			break;
	}
}

/*
	Find a query from a vector.

	const std::vector<std::string> &items: Const Reference to the vector strings / items.
	const std::string &query: Const Reference to the query.
*/
static bool findInVector(const std::vector<std::string> &items, const std::string &query) {
	for(const std::string &item : items) {
		if (StringUtils::lower_case(item).find(query) != std::string::npos) return true;
	}

	return false;
}

/*
	Search for stuff of the store.

	const std::string &query: Const Reference to the query.
	bool title: if titles should be included.
	bool author: if authors should be included.
	bool category: if categories should be included.
	bool console: if consoles should be included.
	int selectedMarks: The selected mark flags.
	bool updateAvl: if available updates should be an included flag.
	bool isAND: if using AND or OR mode.
*/
void StoreUtils::search(const std::string &query, bool title, bool author, bool category, bool console, int selectedMarks, bool updateAvl, bool isAND) {
	if (isAND) {
		for (auto it = StoreUtils::entries.begin(); it != StoreUtils::entries.end(); ++it) {
			if (!(((title && StringUtils::lower_case((*it)->GetTitle()).find(StringUtils::lower_case(query)) != std::string::npos)
			|| (author && StringUtils::lower_case((*it)->GetAuthor()).find(StringUtils::lower_case(query)) != std::string::npos)
			|| (category && findInVector((*it)->GetCategoryFull(), StringUtils::lower_case(query)))
			|| (console && findInVector((*it)->GetConsoleFull(), StringUtils::lower_case(query)))
			|| (!title && !author && !category && !console))
			&& ((selectedMarks == 0 && !updateAvl) || ((((*it)->GetMarks() & selectedMarks) == selectedMarks) && (!updateAvl || (*it)->GetUpdateAvl()))))) {
				it = StoreUtils::entries.erase(it);
				--it;
			}
		}

	} else {
		for (auto it = StoreUtils::entries.begin(); it != StoreUtils::entries.end(); ++it) {
			if (!(((title && StringUtils::lower_case((*it)->GetTitle()).find(StringUtils::lower_case(query)) != std::string::npos)
			|| (author && StringUtils::lower_case((*it)->GetAuthor()).find(StringUtils::lower_case(query)) != std::string::npos)
			|| (category && findInVector((*it)->GetCategoryFull(), StringUtils::lower_case(query)))
			|| (console && findInVector((*it)->GetConsoleFull(), StringUtils::lower_case(query)))
			|| (!title && !author && !category && !console))
			&& ((selectedMarks == 0 && !updateAvl) || (*it)->GetMarks() & selectedMarks || (updateAvl && (*it)->GetUpdateAvl())))) {
				it = StoreUtils::entries.erase(it);
				--it;
			}
		}
	}
}

/* Reset everything of the store and clear + fetch the entries again. */
void StoreUtils::ResetAll() {
	if (StoreUtils::store) {
		StoreUtils::entries.clear();

		if (StoreUtils::store->GetValid()) {
			for (int i = 0; i < StoreUtils::store->GetStoreSize(); i++) {
				StoreUtils::entries.push_back( std::make_unique<StoreEntry>(StoreUtils::store, StoreUtils::meta, i) );
			}

			StoreUtils::store->SetBox(0);
			StoreUtils::store->SetEntry(0);
			StoreUtils::store->SetScreenIndx(0);
		}
	}
}

/* Refresh the available update displays from all Entries. */
void StoreUtils::RefreshUpdateAVL() {
	for (int i = 0; i < (int)StoreUtils::entries.size(); i++) {
		if (StoreUtils::entries[i]) {
			StoreUtils::entries[i]->SetUpdateAvl(StoreUtils::meta->UpdateAvailable(StoreUtils::store->GetUniStoreTitle(), StoreUtils::entries[i]->GetTitle(), StoreUtils::entries[i]->GetLastUpdated()));
		}
	}
}

void StoreUtils::AddToQueue(int index, const std::string &entry, const std::string &entryName, const std::string &lUpdated) {
	if (!StoreUtils::store || !StoreUtils::store->GetValid()) return;

	/* Check first for proper JSON. */
	if (!StoreUtils::store->GetJson().contains("storeContent")) return;
	if ((int)StoreUtils::store->GetJson()["storeContent"].size() < index) return;
	if (!StoreUtils::store->GetJson()["storeContent"][index].contains(entry)) return;

	nlohmann::json Script = nullptr;

	/* Detect if array or new object thing. Else return Syntax error. :P */
	if (StoreUtils::store->GetJson()["storeContent"][index][entry].type() == nlohmann::json::value_t::array) {
		Script = StoreUtils::store->GetJson()["storeContent"][index][entry];

	} else if (StoreUtils::store->GetJson()["storeContent"][index][entry].type() == nlohmann::json::value_t::object) {
		if (StoreUtils::store->GetJson()["storeContent"][index][entry].contains("script") && StoreUtils::store->GetJson()["storeContent"][index][entry]["script"].is_array()) {
			Script = StoreUtils::store->GetJson()["storeContent"][index][entry]["script"];

		} else {
			return;
		}
	}

	QueueSystem::AddToQueue(Script, StoreUtils::store->GetIconEntry(index), entry, StoreUtils::store->GetUniStoreTitle(), entryName, lUpdated); // Here we add this to the Queue at the end.
}

/*
	Add all update-able entries to the queue.
*/
void StoreUtils::AddAllToQueue() {
	if (StoreUtils::store && StoreUtils::store->GetValid() && StoreUtils::meta && !StoreUtils::entries.empty()) { // Ensure all is valid.
		for (int storeEntry = 0; storeEntry < (int)StoreUtils::entries.size(); storeEntry++) {
			if (StoreUtils::entries[storeEntry]) { // Ensure pointer is valid.

				const std::vector<std::string> entryNames = StoreUtils::store->GetDownloadList(StoreUtils::entries[storeEntry]->GetEntryIndex()); // Return a vector of all Download Entries.
				const std::vector<std::string> installedNames = StoreUtils::meta->GetInstalled(StoreUtils::store->GetUniStoreTitle(), StoreUtils::entries[storeEntry]->GetTitle()); // Return a vector from all installed entries.

				if (!entryNames.empty() && !installedNames.empty()) { // Ensure both aren't empty.
					for (int i = 0; i < (int)entryNames.size(); i++) {
						for (int i2 = 0; i2 < (int)installedNames.size(); i2++) {
							if (entryNames[i] == installedNames[i2]) { // If name matches with installed title, add to queue.
								/* Add to Queue. */
								StoreUtils::AddToQueue(entries[storeEntry]->GetEntryIndex(), entryNames[i2], entries[storeEntry]->GetTitle(), entries[storeEntry]->GetLastUpdated());
							}
						}
					}
				}
			}
		}
	}
}