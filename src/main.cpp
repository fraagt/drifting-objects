#include <iostream>
#include "Application.hpp"

int main() {
    try {
        Application application;

        application.run();

    } catch (const std::exception &exception) {
        std::cout << exception.what() << std::endl;
    }
}
