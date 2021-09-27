#define __cpp_lib_format

#include <format>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

static const map<string, vector<tuple<string, string>>> asts = {
    {"Unary", {{"TokenType", "op"}, {"std::unique_ptr<Expr>", "value"}}},
    {"String", {{"std::string", "value"}}},
    {"Number", {{"double", "value"}}},
    {"Bool", {{"bool", "value"}}},
    {"Binary", {{"TokenType", "op"}, {"std::unique_ptr<Expr>", "left"}, {"std::unique_ptr<Expr>", "right"}}},
    {"Group", {{"std::unique_ptr<Expr>", "expr"}}}};

void gen_visitor_class() {
    std::cout << "struct AstVisitor<R> {\n";

    for (const auto &[name, _] : asts) {
        std::cout << std::format("  virtual R visit{}({}&) = 0;\n", name,
                                 name);
    }

    std::cout << "};\n\n";
}

int main() {
    std::cout << "#include <string>\n";
    std::cout << "#include \"scanner.h\"\n\n";

    gen_visitor_class();

    std::cout << "struct Expr {\n"
                 "    virtual void accept(AstVisitor<void> visitor) = 0;\n"
                 "};\n\n";

    for (const auto &[type, fields] : asts) {
        std::cout << std::format("struct {}: public Expr {{\n", type);

        for (const auto &[field_type, field_name] : fields) {
            std::cout << std::format("  {} {};\n", field_type, field_name);
        }
        std::cout
            << std::format("\n  auto operator<=>(const {}&) const = default;\n",
                           type)
            << std::format("  void accept(AstVisitor<void> visitor) override {{\n")
            << std::format("     visitor.visit{}(*this);\n", type) << "  }\n"
            << "};\n\n";
    }
    return 0;
}