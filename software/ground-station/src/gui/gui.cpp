#include "gui.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "imgui_wrapper/render_wrapper.h"
#include "src/util/defines.h"
#include "src/util/logger.h"
#include "src/assets/image_loader.h"
#include "src/gui/styles/fonts.h"
#include "src/util/config.h"

GUI::GUI(SDL_Window* window, f32 window_width, f32 window_height) {
	// setting up imgui
	desc = Renderer::alloc_desc();
	set_backend_desc(desc);

	Renderer::init_from_SDL3(window, desc);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= 
		ImGuiConfigFlags_DockingEnable | 
		ImGuiConfigFlags_NavEnableKeyboard |
		ImGuiBackendFlags_RendererHasVtxOffset;

	ImGui::StyleColorsDark();

	// loading fonts
	FontBook::load_font(Config::font_path.c_str());
	ImGui::PushFont(FontBook::get_font(FONT_SIZE_20PX));

	// giving full window parameters
	x_ = 0;
	y_ = 0;
	width_ = window_width;
	height_ = window_height;

	viewport = std::make_unique<Viewport>("viewport");
	navbar = std::make_unique<Navbar>("navbar");
	infobar = std::make_unique<Infobar>("infobar");
	titlebar = std::make_unique<Titlebar>("titlebar");
	statusbar = std::make_unique<Statusbar>("statusbar");
	toolbar = std::make_unique<Toolbar>("toolbar");
}

GUI::~GUI() {
	ImGui::PopFont();
	Renderer::shutdown();
	Renderer::free_desc(desc);
}

void GUI::draw(SDL_Window* window) {
    Renderer::new_frame(window, desc);

	// fullscreen imgui window
	const ImGuiViewport* full_window_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(full_window_viewport->WorkPos);
	ImGui::SetNextWindowSize(full_window_viewport->WorkSize);
	ImGui::SetNextWindowViewport(full_window_viewport->ID);

	// styling to fit entire window
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags window_flags = 
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoBringToFrontOnFocus | 
		ImGuiWindowFlags_NoNavFocus | 
		ImGuiWindowFlags_NoBackground;

	// setting theme colors
	ImGui::Begin("MasterDockSpaceWindow", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

	static bool docker_init = false;
	if (!docker_init) {
		docker_init = true;

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.18f, 0.22f, 0.30f, 1);

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, full_window_viewport->WorkSize);

		// Format:
		//  ┌───────────────────────────────────────────────────────┐
		//  │                       TITLE BAR                       │
		//  ├─────┬──────────┬──────────────────────────┬───────────┤
		//  │     │          │                          │           │
		//  │ NAV │   INFO   │        VIEWPORT          │   Tools   │
		//  │     │          │                          │           │
		//  │     │          │                          │           │
		//  │     │          │                          │           │
		//  │     │          ├──────────────────────────┤           │
		//  │     │          │          STATUS          │           │
		//  │     │          │                          │           │
		//  └─────┴──────────┴──────────────────────────┴───────────┘

		ImGuiID center_id = dockspace_id;

		// Setting space for each panel
		ImGuiID titlebar_id = ImGui::DockBuilderSplitNode(
			center_id, ImGuiDir_Up, 0.08f, nullptr, &center_id
		);

		ImGuiID navbar_id = ImGui::DockBuilderSplitNode(
			center_id, ImGuiDir_Left, 0.05f, nullptr, &center_id
		);

		ImGuiID infobar_id = ImGui::DockBuilderSplitNode(
			center_id, ImGuiDir_Left, 0.15f, nullptr, &center_id
		);

		ImGuiID toolbar_id = ImGui::DockBuilderSplitNode(
			center_id, ImGuiDir_Right, 0.25f, nullptr, &center_id
		);

		ImGuiID statusbar_id = ImGui::DockBuilderSplitNode(
			center_id, ImGuiDir_Down, 0.10f, nullptr, &center_id
		);


		// Saving ids
		viewport->set_id(center_id);
		navbar->set_id(navbar_id);
		infobar->set_id(infobar_id);
		titlebar->set_id(titlebar_id);
		toolbar->set_id(toolbar_id);
		statusbar->set_id(statusbar_id);

		// Dock windows
		ImGui::DockBuilderDockWindow(navbar->get_name(), navbar_id);
		ImGui::DockBuilderDockWindow(infobar->get_name(), infobar_id);
		ImGui::DockBuilderDockWindow(viewport->get_name(), center_id);
		ImGui::DockBuilderDockWindow(titlebar->get_name(), titlebar_id);
		ImGui::DockBuilderDockWindow(toolbar->get_name(), toolbar_id);
		ImGui::DockBuilderDockWindow(statusbar->get_name(), statusbar_id);

		// Configuring options
		viewport->configure();
		navbar->configure();
		infobar->configure();
		titlebar->configure();
		toolbar->configure();
		statusbar->configure();

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);

	// each panel now renders on its own
	navbar->draw();
	viewport->draw();
	infobar->draw();
	titlebar->draw();
	statusbar->draw();
	toolbar->draw();

	ImGui::End();   

	Renderer::render(desc);
}

void GUI::event_handler(SDL_Event* event) {
	Renderer::process_event(event);

	// keybind motions
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event->key.key) {
                case SDLK_W: Log::debug("test"); break;
                default: break;
            }
            break; // missing break in original
        }
        default: break;
    }
}

// // ─────────────────────────────────────────────
// //  Helpers
// // ─────────────────────────────────────────────
//
// static void push_panel_style() {
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,   ImVec2(0, 0));
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//     ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.13f, 0.14f, 1.0f));
// }
//
// static void pop_panel_style() {
//     ImGui::PopStyleVar(2);
//     ImGui::PopStyleColor();
// }
//
// // ─────────────────────────────────────────────
// //  Constructor / Destructor
// // ─────────────────────────────────────────────
//
// GUI::GUI(SDL_Window* window) {
//     Renderer::BackendInitDesc desc;
//     Renderer::InitFromSDL3(window, desc);
//
//     ImGuiIO& io = ImGui::GetIO();
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//     io.IniFilename  = nullptr; // disable imgui.ini — we manage layout ourselves
//
//     ImGui::StyleColorsDark();
//
//     // Tune the dark theme
//     ImGuiStyle& style = ImGui::GetStyle();
//     style.WindowRounding   = 0.0f;
//     style.ChildRounding    = 0.0f;
//     style.FrameRounding    = 3.0f;
//     style.GrabRounding     = 3.0f;
//     style.ScrollbarRounding = 0.0f;
//     style.WindowBorderSize = 0.0f;
//     style.Colors[ImGuiCol_Border]        = ImVec4(0.22f, 0.22f, 0.24f, 1.0f);
//     style.Colors[ImGuiCol_WindowBg]      = ImVec4(0.13f, 0.13f, 0.14f, 1.0f);
//     style.Colors[ImGuiCol_ChildBg]       = ImVec4(0.11f, 0.11f, 0.12f, 1.0f);
//     style.Colors[ImGuiCol_FrameBg]       = ImVec4(0.18f, 0.18f, 0.20f, 1.0f);
//     style.Colors[ImGuiCol_Button]        = ImVec4(0.22f, 0.22f, 0.26f, 1.0f);
//     style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.38f, 1.0f);
//     style.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.38f, 0.38f, 0.52f, 1.0f);
//     style.Colors[ImGuiCol_Header]        = ImVec4(0.22f, 0.22f, 0.30f, 1.0f);
//     style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.28f, 0.40f, 1.0f);
//     style.Colors[ImGuiCol_Tab]           = ImVec4(0.16f, 0.16f, 0.18f, 1.0f);
//     style.Colors[ImGuiCol_TabHovered]    = ImVec4(0.28f, 0.28f, 0.38f, 1.0f);
//     style.Colors[ImGuiCol_TabSelected]   = ImVec4(0.22f, 0.22f, 0.30f, 1.0f);
// }
//
// GUI::~GUI() {
//     Renderer::Shutdown();
// }
//
// // ─────────────────────────────────────────────
// //  Splitters
// // ─────────────────────────────────────────────
//
// // Vertical splitter — adjusts a width value (left or right panel edge)
// void GUI::draw_splitter_v(const char* id, float x, float y, float h,
//                            float& value, float vmin, float vmax) {
//     ImDrawList* dl = ImGui::GetForegroundDrawList();
//
//     // Visible line
//     ImU32 col = IM_COL32(50, 50, 55, 255);
//     dl->AddRectFilled(ImVec2(x, y), ImVec2(x + SPLITTER_W, y + h), col);
//
//     // Hit zone (wider than visual for easy grab)
//     ImGui::SetCursorScreenPos(ImVec2(x - (SPLITTER_HIT - SPLITTER_W) * 0.5f, y));
//     ImGui::InvisibleButton(id, ImVec2(SPLITTER_HIT, h));
//
//     if (ImGui::IsItemActive())
//         value = CLAMP(value + ImGui::GetIO().MouseDelta.x, vmin, vmax);
//
//     if (ImGui::IsItemHovered() || ImGui::IsItemActive())
//         ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
// }
//
// // Horizontal splitter — adjusts a 0..1 fraction for top/bottom split
// void GUI::draw_splitter_h(const char* id, float x, float y, float w, float& frac) {
//     ImDrawList* dl = ImGui::GetForegroundDrawList();
//
//     ImU32 col = IM_COL32(50, 50, 55, 255);
//     dl->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + SPLITTER_W), col);
//
//     ImGui::SetCursorScreenPos(ImVec2(x, y - (SPLITTER_HIT - SPLITTER_W) * 0.5f));
//     ImGui::InvisibleButton(id, ImVec2(w, SPLITTER_HIT));
//
//     if (ImGui::IsItemActive()) {
//         // Convert pixel delta to fraction of available height
//         float avail_h = ImGui::GetIO().DisplaySize.y
//                       - m_toolbar_height - m_statusbar_height;
//         frac = CLAMP(frac + ImGui::GetIO().MouseDelta.y / avail_h,
//                        RSPLIT_MIN, RSPLIT_MAX);
//     }
//
//     if (ImGui::IsItemHovered() || ImGui::IsItemActive())
//         ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
// }
//
// // ─────────────────────────────────────────────
// //  Panel content
// // ─────────────────────────────────────────────
//
// void GUI::draw_toolbar(float x, float y, float w, float h) {
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 6));
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//     ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.11f, 1.0f));
//
//     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
//     ImGui::Begin("##toolbar", nullptr, PANEL_FLAGS | ImGuiWindowFlags_NoScrollWithMouse);
//
//     // Draw bottom border
//     ImDrawList* dl = ImGui::GetWindowDrawList();
//     dl->AddLine(ImVec2(x, y + h - 1), ImVec2(x + w, y + h - 1),
//                 IM_COL32(40, 40, 44, 255), 1.0f);
//
//     ImGui::SetCursorPosY((h - ImGui::GetFrameHeight()) * 0.5f);
//     ImGui::Button("File");    ImGui::SameLine(0, 4);
//     ImGui::Button("Edit");    ImGui::SameLine(0, 4);
//     ImGui::Button("View");    ImGui::SameLine(0, 4);
//     ImGui::Button("Run");
//
//     // Right-aligned items
//     float right_x = w - 80.0f;
//     ImGui::SameLine(right_x);
//     ImGui::Button("Settings");
//
//     ImGui::End();
//     ImGui::PopStyleColor();
//     ImGui::PopStyleVar(2);
// }
//
// void GUI::draw_statusbar(float x, float y, float w, float h) {
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 3));
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//     ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.09f, 0.09f, 0.10f, 1.0f));
//
//     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
//     ImGui::Begin("##statusbar", nullptr, PANEL_FLAGS | ImGuiWindowFlags_NoScrollWithMouse);
//
//     ImDrawList* dl = ImGui::GetWindowDrawList();
//     dl->AddLine(ImVec2(x, y), ImVec2(x + w, y), IM_COL32(40, 40, 44, 255), 1.0f);
//
//     ImGui::TextDisabled("Ready");
//     ImGui::SameLine(w - 120.0f);
//     ImGui::TextDisabled("FPS: %.0f", ImGui::GetIO().Framerate);
//
//     ImGui::End();
//     ImGui::PopStyleColor();
//     ImGui::PopStyleVar(2);
// }
//
// void GUI::draw_left_panel(float x, float y, float w, float h) {
//     push_panel_style();
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
//
//     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
//     ImGui::Begin("##left_panel", nullptr, PANEL_FLAGS);
//
//     ImGui::SeparatorText("Scene");
//     // TODO: scene tree here
//     ImGui::TextDisabled("(empty)");
//
//     ImGui::Spacing();
//     ImGui::SeparatorText("Assets");
//     // TODO: asset browser here
//
//     ImGui::End();
//     ImGui::PopStyleVar();
//     pop_panel_style();
// }
//
// void GUI::draw_viewport(float x, float y, float w, float h) {
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//     ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.06f, 0.07f, 1.0f));
//
//     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
//     ImGui::Begin("##viewport", nullptr, PANEL_FLAGS | ImGuiWindowFlags_NoScrollWithMouse);
//
//     // ── Render target goes here ──────────────────────────────────────────
//     // When you have an SDL_Texture* or OpenGL FBO:
//     //   ImGui::Image((ImTextureID)your_render_texture, ImVec2(w, h));
//     //
//     // For now, draw a placeholder with the background draw list
//     ImDrawList* dl = ImGui::GetWindowDrawList();
//     dl->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), IM_COL32(15, 15, 20, 255));
//
//     // Grid overlay — subtle reference grid for the viewport
//     ImU32 grid_col = IM_COL32(30, 30, 38, 255);
//     float step = 40.0f;
//     for (float gx = x; gx < x + w; gx += step)
//         dl->AddLine(ImVec2(gx, y), ImVec2(gx, y + h), grid_col, 0.5f);
//     for (float gy = y; gy < y + h; gy += step)
//         dl->AddLine(ImVec2(x, gy), ImVec2(x + w, gy), grid_col, 0.5f);
//
//     // Centre crosshair
//     float cx = x + w * 0.5f, cy = y + h * 0.5f;
//     dl->AddLine(ImVec2(cx - 12, cy), ImVec2(cx + 12, cy), IM_COL32(80, 80, 100, 180), 1.0f);
//     dl->AddLine(ImVec2(cx, cy - 12), ImVec2(cx, cy + 12), IM_COL32(80, 80, 100, 180), 1.0f);
//
//     // Overlay label
//     ImGui::SetCursorScreenPos(ImVec2(x + 8, y + 8));
//     ImGui::TextDisabled("Viewport");
//
//     // Mouse input guard — only forward to 3D camera when hovered
//     if (ImGui::IsWindowHovered()) {
//         // TODO: pass ImGui::GetIO().MouseDelta to your camera controller
//     }
//
//     ImGui::End();
//     ImGui::PopStyleColor();
//     ImGui::PopStyleVar(2);
// }
//
// void GUI::draw_right_top(float x, float y, float w, float h) {
//     push_panel_style();
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
//
//     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
//     ImGui::Begin("##inspector", nullptr, PANEL_FLAGS);
//
//     ImGui::SeparatorText("Inspector");
//     // TODO: property grid
//
//     ImGui::End();
//     ImGui::PopStyleVar();
//     pop_panel_style();
// }
//
// void GUI::draw_right_bottom(float x, float y, float w, float h) {
//     push_panel_style();
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
//
//     ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
//     ImGui::Begin("##console", nullptr, PANEL_FLAGS);
//
//     ImGui::SeparatorText("Console");
//     // TODO: log output scroll region
//
//     ImGui::End();
//     ImGui::PopStyleVar();
//     pop_panel_style();
// }
//
// // ─────────────────────────────────────────────
// //  Main draw
// // ─────────────────────────────────────────────
//
// void GUI::draw(void) {
//     Renderer::NewFrame();
//
//     const ImVec2 display = ImGui::GetIO().DisplaySize;
//     const float  W       = display.x;
//     const float  H       = display.y;
//
//     // ── Derived layout (all computed from the few stored floats) ─────────
//     const float content_y = m_toolbar_height;
//     const float content_h = H - m_toolbar_height - m_statusbar_height;
//
//     const float left_x  = 0.0f;
//     const float mid_x   = m_left_width + SPLITTER_W;
//     const float right_x = W - m_right_width;
//     const float mid_w   = right_x - SPLITTER_W - mid_x;
//
//     const float right_top_h    = content_h * m_right_split;
//     const float right_bottom_y = content_y + right_top_h + SPLITTER_W;
//     const float right_bottom_h = content_h - right_top_h - SPLITTER_W;
//
//     // ── Draw panels (order matters — splitters drawn on top) ─────────────
//     draw_toolbar(0, 0, W, m_toolbar_height);
//     draw_statusbar(0, H - m_statusbar_height, W, m_statusbar_height);
//
//     draw_left_panel(left_x, content_y, m_left_width, content_h);
//     draw_viewport(mid_x,   content_y, mid_w, content_h);
//     draw_right_top(right_x, content_y, m_right_width, right_top_h);
//     draw_right_bottom(right_x, right_bottom_y, m_right_width, right_bottom_h);
//
//     // ── Splitters — full-screen transparent overlay ─────────────────────
//     ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
//     ImGui::SetNextWindowSize(display, ImGuiCond_Always);
//     ImGui::SetNextWindowBgAlpha(0.0f);
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//     ImGui::Begin("##splitter_host", nullptr,
//         ImGuiWindowFlags_NoTitleBar              |
//         ImGuiWindowFlags_NoResize                |
//         ImGuiWindowFlags_NoMove                  |
//         ImGuiWindowFlags_NoScrollbar             |
//         ImGuiWindowFlags_NoSavedSettings         |
//         ImGuiWindowFlags_NoBackground            |
//         ImGuiWindowFlags_NoBringToFrontOnFocus);
//     ImGui::PopStyleVar(2);
//
//     draw_splitter_v("##vsplit_left",
//                     m_left_width, content_y, content_h,
//                     m_left_width, LEFT_MIN, LEFT_MAX);
//
//     draw_splitter_v("##vsplit_right",
//                     W - m_right_width - SPLITTER_W, content_y, content_h,
//                     m_right_width, RIGHT_MIN, RIGHT_MAX);
//
//     draw_splitter_h("##hsplit_right",
//                     right_x, content_y + right_top_h, m_right_width,
//                     m_right_split);
//
//     ImGui::End();
//
//     Renderer::Render();
// }
//
// // ─────────────────────────────────────────────
// //  Event handler
// // ─────────────────────────────────────────────
//
// void GUI::event_handler(SDL_Event* event) {
//     ImGui_ImplSDL3_ProcessEvent(event);
//     switch (event->type) {
//         case SDL_EVENT_KEY_DOWN: {
//             switch (event->key.key) {
//                 case SDLK_W: Log::debug("test"); break;
//                 default: break;
//             }
//             break; // missing break in original
//         }
//         default: break;
//     }
// }
