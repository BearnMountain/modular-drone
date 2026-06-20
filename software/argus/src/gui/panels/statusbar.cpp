#include "statusbar.h"
#include "src/gui/styles/fonts.h"

Statusbar::Statusbar(const std::string name) {
	Log::debug("statusbar initialized");

	name_ = name;
}

void Statusbar::draw_chevron(ImDrawList* dl, ImVec2 pos, float w, float h) {
    float tipX   = pos.x + w + h * 0.4f; // how far the point sticks out
    float midY   = pos.y + h * 0.5f;

    // 6 points: rectangle left side + chevron point on right
    ImVec2 pts[] = {
        { pos.x,          pos.y      },  // top-left
        { pos.x + w,      pos.y      },  // top-right (before point)
        { tipX,           midY       },  // RIGHT POINT
        { pos.x + w,      pos.y + h  },  // bottom-right
        { pos.x,          pos.y + h  },  // bottom-left
    };

    ImU32 fillColor   = IM_COL32(10, 30, 20, 240);
    ImU32 borderColor = IM_COL32(0, 200, 80, 255);

    dl->AddConvexPolyFilled(pts, 5, fillColor);
    dl->AddPolyline(pts, 5, borderColor, ImDrawFlags_Closed, 2.0f);
}

void Statusbar::draw(void) {

	ImGui::Begin(name_.c_str());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
	ImGui::BeginChild("test");

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();

	draw_list->AddRect(
		pos,
		ImVec2(pos.x + size.x, pos.y + size.y),
		IM_COL32(255, 255, 255, 255),
		8.0f
	);

	const f32 header_portion = 0.3f; // percentage of statusbar that is the header + padding
	const f32 padding_portion = 0.05f;
	const f32 padding = size.y * padding_portion;
	
	f32 target_font_size = size.y * header_portion - padding*2;
	FontSizes header_text_font = FontBook::get_target_font(target_font_size);
	
	// header
	draw_list->AddText(
		FontBook::get_font(header_text_font), target_font_size, 
		ImVec2(pos.x + padding, pos.y + padding),
		IM_COL32(255, 255, 255, 255), 
		"MISSION TIMELINE"
	);
	
	// status timeline
	f32 chevron_size = (1.0f - header_portion) * size.y - 4*padding;
	f32 chevron_y = pos.y + size.y * header_portion + padding;
	draw_chevron(draw_list, ImVec2(pos.x + 30.0f, chevron_y), chevron_size, chevron_size);



	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	ImGui::End();
}

void Statusbar::add_timestep(const std::string& label) {
	timeline_.emplace_back(
		Timestep{
			label,
			Clock::now()
		}
	);
}

void Statusbar::configure() {
	if (!id_) {
		Log::debug("statusbar configuration cant be applied to unitialized ImGuiID");
		return;
	}

	if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(id_))
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	else 
		Log::error("ImGuiID '{}' doesn't exist(Statusbar)", id_);
}
