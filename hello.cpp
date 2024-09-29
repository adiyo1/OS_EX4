/*
 * File: hello.cpp
 */
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string string_so_far;

    for (int i = 0; i < argc; i++) {
        if (!string_so_far.empty()) {
            string_so_far += " ";
        }
        string_so_far += argv[i];
    }

    std::cout << "You entered: " << string_so_far << std::endl;
    return 0;
}
