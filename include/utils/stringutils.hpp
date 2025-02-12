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

#ifndef _NDS_SHOP_STRING_UTILS_HPP
#define _NDS_SHOP_STRING_UTILS_HPP

#include "meta.hpp"
#include <string>
#include <vector>

namespace StringUtils {
	std::string lower_case(const std::string &str);
	std::string FetchStringsFromVector(const std::vector<std::string> &fetch);
	std::string formatBytes(u64 bytes);
	std::string GetMarkString(int marks);
	std::vector<std::string> GetMarks(int marks);
	std::string format(const char *fmt_str, ...);
};

#endif