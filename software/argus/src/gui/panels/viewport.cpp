#include "viewport.h"
// #include "implot/implot.h"
#include "src/util/logger.h"


Viewport::Viewport(const std::string name, Renderer::BackendInitDesc* desc) {
	Log::debug("viewport initialized");

	name_ = name;
	desc_ = desc;
	id_ = 0;
	// map_ = std::make_unique<Map>(
	// 	desc_,
	// 	ImVec2(0.0f, 0.0f),
	// 	ImVec2(10.0f, 10.0f)
	// );
}

Viewport::~Viewport(void) {
}

void Viewport::draw(void) {
	ImGui::Begin(name_.c_str());

	// static bool checked = false;
	// ImGui::Checkbox("checkbox", &checked);
	// if (checked) {
	// 	ImGui::Text("checkbox is on");
	// }

	// static bool pressed = false;	
	// Widget::icon_button(textures[0].id, "Info", ImVec2(50,50), pressed);

	// LATER
	// srand(0);
	//    static f32 xn[50], yn[50], zn[50];
	//    for (int i = 0; i < 50; i++) {
	//        xn[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
	//        yn[i] = 0.50f + 0.2f * ((float)rand() / (float)RAND_MAX);
	//        zn[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
	//    }
	// Widget::plot_point_cloud(xn, yn, zn, 50);

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
