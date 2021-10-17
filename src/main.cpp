#include "stdafx.h"

#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include "literal.h"

constexpr auto PROMPT = "ylox> ";

void RunPrompt() {
	std::string input;
	while (true)
	{
		std::cout << PROMPT;
		std::getline(std::cin, input);
		if (input.empty())
		{
			std::cout << "please input something" << std::endl;
			continue;
		}
		try
		{
			auto stmts = Parse(Scan(input));
			Interpreter::Eval(stmts);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void RunFile(std::string_view file_name) {}

int main(int argc, char* argv[]) {
	if (argc == 1)
	{
		RunPrompt();
	}
	else if (argc == 2)
	{
		RunFile(argv[1]);
	}
	else
	{
		std::cerr << "use like ./ylox <script>" << std::endl;
	}
	return 0;
}