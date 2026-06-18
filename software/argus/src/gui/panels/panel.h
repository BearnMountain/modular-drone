#pragma once

#include <string>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "src/util/defines.h"
#include "src/util/logger.h"

class Panel {
public:
    virtual ~Panel() = default;

    // Core interface
    // virtual void update(float dt) {}
    virtual void draw() = 0;

    // Lifecycle
    virtual void on_attach() {}
    virtual void on_detach() {}

    // State
    bool is_open() const { return open; }
    void set_open(bool value) { open = value; }

    const char* get_name() const { return name_.c_str(); }

	// imgui
	void set_id(ImGuiID id) { id_ = id; }
	ImGuiID get_id(void) const { return id_; }

	virtual void configure(void) = 0; // sets panel style

protected:
    std::string name_;
    bool open = true;

	f32 x_, y_, width_, height_;
	ImGuiID id_;
};
