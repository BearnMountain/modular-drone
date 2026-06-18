#pragma once

#include "src/util/defines.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "image_loader.h"

enum IconAsset {
	ICON_ASSET_BELL,
	ICON_ASSET_DASHBOARD,
	ICON_ASSET_DRONE,
	ICON_ASSET_LOG,
	ICON_ASSET_MAP,
	ICON_ASSET_PLAN,
	ICON_ASSET_QUESTION,
	ICON_ASSET_SATELLITE,
	ICON_ASSET_SETTING,
	ICON_ASSET_SIGNAL_TOWER,
	ICON_ASSET_STORAGE,
	ICON_ASSET_USER,
	ICON_ASSET_WIFI,
	ICON_ASSET_COUNT
};

namespace AssetManager {
	void init();
	void uninit();

	// textures
	void load_darkmode_icons(void);
	void unload_darkmode_icons(void);
	// fonts

	// getting textures
	Texture get_icon(IconAsset reference);
};
