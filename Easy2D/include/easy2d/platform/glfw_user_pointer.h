#pragma once

struct GLFWwindow;

namespace easy2d {

class Window;
class Input;

struct GlfwUserPointer {
    Window* window = nullptr;
    Input* input = nullptr;
};

} // namespace easy2d

