#include <iostream>
#include "px/script/angel_script.hpp"

const char *code =
R"(
void main() {
  print("Hello world");

  print2("Custom function declaration");
}
)";

void print(const std::string &str) {
  std::cout << str << std::endl;
}

int main() {
  px::script::AngelScript as;

  as.registerGlobalFunction("print", &print); // void print(const string &in)
  as.registerGlobalFunctionWithDecl("void print2(const string &in)", &print);

  auto builder = as.createModuleBuilder();
  builder.startNewModule("main");
  builder.addSection(code, "code.as");

  auto module = builder.build();

  auto f = module.getFunction<void>("main");
  f();
}