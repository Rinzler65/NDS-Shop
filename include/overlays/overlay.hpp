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

#ifndef _NDS_SHOP_OVERLAY_HPP
#define _NDS_SHOP_OVERLAY_HPP

#include "common.hpp"
#include <3ds.h>

namespace Overlays {
	void SelectStore();
	void SelectLanguage();
	void ShowCredits();
	std::string SelectDir(const std::string &oldDir, const std::string &msg);
	void SelectTheme();
};

#endif