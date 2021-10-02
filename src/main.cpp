#include "stdafx.h"

#include "scanner.h"
#include "parser.h"
#include "ast_print.h"

void run_prompt() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        auto expr = Parse(Scan(input));
        AstPrinter::Print(*expr);
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