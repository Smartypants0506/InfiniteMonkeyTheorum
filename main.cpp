#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>

void simulateKeystroke(char c) {
    SHORT key = VkKeyScan(c);
    BYTE virtualKey = LOBYTE(key);
    BYTE shiftState = HIBYTE(key);

    if (shiftState & 1) {
        keybd_event(VK_SHIFT, 0, 0, 0); // Press Shift
    }

    keybd_event(virtualKey, 0, 0, 0); // Press key
    keybd_event(virtualKey, 0, KEYEVENTF_KEYUP, 0); // Release key

    if (shiftState & 1) {
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0); // Release Shift
    }
}

void simulateBackspace() {
    keybd_event(VK_BACK, 0, 0, 0); // Press Backspace
    keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0); // Release Backspace
    Sleep(123);
}

void typeText(const std::string &text) {
    std::srand(static_cast<unsigned>(std::time(0)));
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];

        if (std::rand() % 100 < 15) { // 5% chance to simulate a typo
            char typo = c + (std::rand() % 3 - 1); // Generate a nearby character
            simulateKeystroke(typo);
            Sleep(50);
            simulateBackspace();
        }

        simulateKeystroke(c);

        if (std::rand() % 100 < 12) { // 3% chance to delete a correct character
            simulateBackspace();
            simulateKeystroke(c);
            Sleep(50);
        }

        Sleep(50 + (std::rand() % 100)*2); // Random delay between keystrokes
    }
}

int main(int argc, char** argv) {
    if (argc < 1) {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }
    if (argc > 2) {
        std::cerr << "Too many arguments" << std::endl;
        return 1;
    }
    
    
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
        return 1;
    }

    std::string line;
    std::string content;
    while (std::getline(file, line)) {
        content += line + '\n';
    }
    file.close();

    std::cout << "Switch to Google Docs within 5 seconds..." << std::endl;
    Sleep(5000); // Give time to switch to Google Docs

    typeText(content);

    std::cout << "Done typing!" << std::endl;
    return 0;
}
