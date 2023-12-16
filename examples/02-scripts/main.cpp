#include <iostream>
#include "px/script/angel_script.hpp"

const char *code =
R"(
void main() {
  Player player;

  player.x = 2435;
  player.hello();
}
)";

void print(const std::string &str) {
  std::cout << str << std::endl;
}
void printF(float f) {
  std::cout << f << std::endl;
}

struct Player : px::script::AsClassType<"Player"> {
  float x{};
  float y{};

  const float maxHealth {5.0f};

  void where() const {
    std::cout << x << " " << y << std::endl;
  }
};

void hello(Player *self) {
  fmt::print("Hello from X:{} Y:{}!\n", self->x, self->y);
}

int main() {
  px::script::AngelScript as;

  // registering functions
  as.registerGlobalFunction("print", &print); // void print(const string &in)
  as.registerGlobalFunction("print", &printF); // void print(float)

  // registering types
  auto type = as.registerObjectType<Player>("Player");
  type.registerProperty<&Player::x>("x");
  type.registerProperty<&Player::y>("y");
  type.registerProperty<&Player::maxHealth>("maxHealth");

  type.registerMethod<&Player::where>("where");

  type.registerProxyMethod("hello", &hello);

  // building script
  auto builder = as.createModuleBuilder();
  builder.startNewModule("main");
  builder.addSection(code, "code.as");

  auto module = builder.build();

  auto f = module.getFunction<void>("main");
  f();
}