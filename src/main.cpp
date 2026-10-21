#include "UserInterface.h"
#include <iostream>
#include <exception>

int main() {
    try {
        UserInterface ui;
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        return 1;
    }
    return 0;
}
