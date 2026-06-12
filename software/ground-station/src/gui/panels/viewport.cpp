#include "viewport.h"
#include "src/gui/widgets/widget.h"
#include "src/util/logger.h"

#include <random>

Viewport::Viewport(const std::string name) {
	Log::debug("viewport initialized");

	name_ = name;
	id_ = 0;
	textures.push_back(
		load_imgui_texture("res/img/widgets/question.png")
	);
}

Viewport::~Viewport(void) {
	unload_texture(textures[0]);
}

void Viewport::draw(void) {
	ImGui::Begin(name_.c_str());
	ImGui::Text("Test");

	// static bool checked = false;
	// ImGui::Checkbox("checkbox", &checked);
	// if (checked) {
	// 	ImGui::Text("checkbox is on");
	// }

	// static bool pressed = false;	
	// Widget::icon_button(textures[0].id, "Info", ImVec2(50,50), pressed);

	srand(0);
    static f32 xn[50], yn[50], zn[50];
    for (int i = 0; i < 50; i++) {
        xn[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
        yn[i] = 0.50f + 0.2f * ((float)rand() / (float)RAND_MAX);
        zn[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
    }
	Widget::plot_point_cloud(xn, yn, zn, 50);

	ImGui::End();

	
}

void Viewport::configure() {
	if (!id_) {
		Log::debug("viewport configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Viewport)", id_);
}
