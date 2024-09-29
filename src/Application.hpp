#pragma once

#include <memory>
#include "utils/Common.hpp"

class Application {
public:
    Application();

    void run();

private:
    GLFWwindow *m_pWindow;
    int m_width;
    int m_height;
    std::string m_title;
};
