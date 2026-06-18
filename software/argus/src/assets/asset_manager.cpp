#include "asset_manager.h"
#include "src/util/logger.h"

Texture icons_[ICON_ASSET_COUNT];

void AssetManager::init() {
	// loads icon images

}

void AssetManager::uninit() {

}

void AssetManager::load_darkmode_icons(void) {
	icons_[ 0] = load_imgui_texture("res/img/widgets/darkmode/bell.png");
	icons_[ 1] = load_imgui_texture("res/img/widgets/darkmode/dashboard.png");
	icons_[ 2] = load_imgui_texture("res/img/widgets/darkmode/drone.png");
	icons_[ 3] = load_imgui_texture("res/img/widgets/darkmode/log.png");
	icons_[ 4] = load_imgui_texture("res/img/widgets/darkmode/map.png");
	icons_[ 5] = load_imgui_texture("res/img/widgets/darkmode/plan.png");
	icons_[ 6] = load_imgui_texture("res/img/widgets/darkmode/question.png");
	icons_[ 7] = load_imgui_texture("res/img/widgets/darkmode/satellite.png");
	icons_[ 8] = load_imgui_texture("res/img/widgets/darkmode/setting.png");
	icons_[ 9] = load_imgui_texture("res/img/widgets/darkmode/signal_tower.png");
	icons_[10] = load_imgui_texture("res/img/widgets/darkmode/storage.png");
	icons_[11] = load_imgui_texture("res/img/widgets/darkmode/user.png");
	icons_[12] = load_imgui_texture("res/img/widgets/darkmode/wifi.png");
}

void AssetManager::unload_darkmode_icons(void) {
	for (u32 i = 0; i < ICON_ASSET_COUNT; i++)
		unload_texture(icons_[i]);
}

Texture AssetManager::get_icon(IconAsset reference) {
	if (reference == ICON_ASSET_COUNT) {
		Log::debug("must be a valid IconAsset enum");
		return icons_[0];
	}
	return icons_[static_cast<i32>(reference)];
}
