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

#include "theme.hpp"
#include <regex>
#include <unistd.h>

/**
 * @brief Creates a 8 byte RGBA color
 * @param r red component of the color
 * @param g green component of the color
 * @param b blue component of the color
 * @param a alpha component of the color
 */
#define RGBA8(r, g, b, a) ((((r) & 0xFF) << 0) | (((g) & 0xFF) << 8) | (((b) & 0xFF) << 16) | (((a) & 0xFF) << 24))


Theme::Theme(const std::string &ThemeJSON) {
	FILE *file = fopen(ThemeJSON.c_str(), "rt");
	if (file) {
		this->json = nlohmann::json::parse(file, nullptr, false);
		fclose(file);
	}

	if (!file || this->json.is_discarded()) this->json = this->InitWithDefaultColors();
	this->Loaded = true;
}

nlohmann::json Theme::InitWithDefaultColors(const std::string &ThemePath) {
	nlohmann::json JS = {
		{ "Default", {
			{ "BarColor", "#00C9FF" },
			{ "BGColor", "#4B4B4B" },
			{ "BarOutline", "#FFFFFF" },
			{ "TextColor", "#FFFFFF" },
			{ "EntryBar", "#00C9FF" },
			{ "EntryOutline", "#FFFFFF" },
			{ "BoxInside", "#4B4B4B" },
			{ "BoxSelected", "#00C9FF" },
			{ "BoxUnselected", "#4B4B4B" },
			{ "ProgressbarOut", "#000000" },
			{ "ProgressbarIn", "#00C9FF" },
			{ "SearchBar", "#00C9FF" },
			{ "SearchBarOutline", "#FFFFFF" },
			{ "SideBarSelected", "#00C9FF" },
			{ "SideBarUnselected", "#4B4B4B" },
			{ "MarkSelected", "#00C9F0" },
			{ "MarkUnselected", "#4B4B4B" },
			{ "DownListPrev", "#4B4B4B" },
			{ "SideBarIconColor", "#FFFFFF" },
			{ "Description", "NDS-Shop Themes\n\nBy: Rinzler" }
		}}
	};

	FILE *out = fopen(ThemePath.c_str(), "w");
	const std::string dump = JS.dump(1, '\t');
	fwrite(dump.c_str(), 1, JS.dump(1, '\t').size(), out);
	fclose(out);

	return JS;
}


void Theme::LoadTheme(const std::string &ThemeName) {
	this->vBarColor	         = this->GetThemeColor(ThemeName, "BarColor", C2D_Color32(50, 73, 98, 255));
	this->vBGColor	         = this->GetThemeColor(ThemeName, "BGColor", C2D_Color32(38, 44, 77, 255));
	this->vBarOutline        = this->GetThemeColor(ThemeName, "BarOutline", C2D_Color32(25, 30, 53, 255));
	this->vTextColor         = this->GetThemeColor(ThemeName, "TextColor", C2D_Color32(255, 255, 255, 255));
	this->vEntryBar	         = this->GetThemeColor(ThemeName, "EntryBar", C2D_Color32(50, 73, 98, 255));
	this->vEntryOutline      = this->GetThemeColor(ThemeName, "EntryOutline", C2D_Color32(25, 30, 53, 255));
	this->vBoxInside	     = this->GetThemeColor(ThemeName, "BoxInside", C2D_Color32(28, 33, 58, 255));
	this->vBoxSelected	     = this->GetThemeColor(ThemeName, "BoxSelected", C2D_Color32(108, 130, 155, 255));
	this->vBoxUnselected     = this->GetThemeColor(ThemeName, "BoxUnselected", C2D_Color32(0, 0, 0, 255));
	this->vProgressbarOut    = this->GetThemeColor(ThemeName, "ProgressbarOut", C2D_Color32(28, 33, 58, 255));
	this->vProgressbarIn     = this->GetThemeColor(ThemeName, "ProgressbarIn", C2D_Color32(77, 101, 128, 255));
	this->vSearchBar	     = this->GetThemeColor(ThemeName, "SearchBar", C2D_Color32(51, 75, 102, 255));
	this->vSearchBarOutline  = this->GetThemeColor(ThemeName, "SearchBarOutline", C2D_Color32(25, 30, 53, 255));
	this->vSideBarSelected   = this->GetThemeColor(ThemeName, "SideBarSelected", C2D_Color32(108, 130, 155, 255));
	this->vSideBarUnselected = this->GetThemeColor(ThemeName, "SideBarUnselected", C2D_Color32(77, 101, 128, 255));
	this->vMarkSelected		 = this->GetThemeColor(ThemeName, "MarkSelected", C2D_Color32(77, 101, 128, 255));
	this->vMarkUnselected	 = this->GetThemeColor(ThemeName, "MarkUnselected", C2D_Color32(28, 33, 58, 255));
	this->vDownListPrev		 = this->GetThemeColor(ThemeName, "DownListPrev", C2D_Color32(28, 33, 58, 255));
	this->vSideBarIconColor  = this->GetThemeColor(ThemeName, "SideBarIconColor", C2D_Color32(173, 204, 239, 255));
}

std::vector<std::pair<std::string, std::string>> Theme::ThemeNames() {
	std::vector<std::pair<std::string, std::string>> Temp = { };

	if (this->Loaded) {
		for(auto it = this->json.begin(); it != this->json.end(); ++it) {
			if (this->json[it.key()].contains("Description") && this->json[it.key()]["Description"].is_string()) {
				Temp.push_back(std::make_pair(it.key(), this->json[it.key()]["Description"]));

			} else {
				Temp.push_back(std::make_pair(it.key(), it.key()));
			}
		}
	}

	return Temp;
}


uint32_t Theme::GetThemeColor(const std::string &ThemeName, const std::string &Key, const uint32_t DefaultColor) {
	if(this->json.contains(ThemeName) && this->json[ThemeName].is_object() && this->json[ThemeName].contains(Key) && this->json[ThemeName][Key].is_string()) {
		const std::string &colorString = this->json[ThemeName][Key].get_ref<const std::string &>();
		if (colorString.length() < 7 || std::regex_search(colorString.substr(1), std::regex("[^0-9A-Fa-f]"))) { // invalid color.
			return DefaultColor;
		}

		int r = std::stoi(colorString.substr(1, 2), nullptr, 16);
		int g = std::stoi(colorString.substr(3, 2), nullptr, 16);
		int b = std::stoi(colorString.substr(5, 2), nullptr, 16);
		return RGBA8(r, g, b, 0xFF);

	}

	return DefaultColor;
}