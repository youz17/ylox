#include "stdafx.h"

#include "scanner.h"
#include "parser.h"
#include "ast_print.h"

void RunPrompt() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        auto expr = Parse(Scan(input));
        AstPrinter::Print(*expr);
    }
}

void RunFile(std::string_view file_name) {}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        RunPrompt();
    } else if (argc == 2) {
        RunFile(argv[1]);
    } else {
        std::cerr << "use like ./ylox <script>" << std::endl;
    }
    return 0;
}