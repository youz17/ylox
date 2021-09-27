#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "scanner.h"

void run_prompt() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        auto result = scan(input);
        std::ranges::for_each(
            result, [](Token t) { std::cout << std::format("{}\n", t); });
    }
}

void run_file(std::string_view file_name) {}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        run_prompt();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        std::cerr << "use like ./ylox <script>" << std::endl;
    }
    return 0;
}