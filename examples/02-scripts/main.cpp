#include <iostream>
#include "px/script/angel_script.hpp"

const char *code =
R"(
void main() {
  Player player;

  player.x = 2435;
  player.hello();
}

void test(PlayerPtr player) {
  player.y = player.y * 3;
  print(player.y);
}

void hello(const string &in name) {
  print(makeHello(name));
}

string makeHello(const string &in name) {
  return "Hello " + name + "!";
}
)";

void print(const std::string &str) {
  std::cout << str << std::endl;
}
void printF(float f) {
  std::cout << f << std::endl;
}

struct Player : px::script::AsClassType   <"Player">,
                px::script::AsClassPtrType<"PlayerPtr">{
  float x{};
  float y{};

  const float maxHealth {5.0f};

  void where() const {
    std::cout << x << " " << y << std::endl;
  }
};
typedef std::shared_ptr<Player> PlayerPtr;


void printP(const PlayerPtr &ptr) {
  std::cout << ptr->x << std::endl;
}

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

  // smart ptr type
  auto smart = as.registerObjectType<PlayerPtr>();
  smart.registerProperty<&Player::x>("x");
  smart.registerProperty<&Player::y>("y");
  smart.registerProperty<&Player::maxHealth>("maxHealth");

//  smart.registerMethod<&Player::where>("where");

  as.registerGlobalFunction("print", &printP); // void print(const PlayerPtr &in)

  // building script
  auto builder = as.createModuleBuilder();
  builder.startNewModule("main");
  builder.addSection(code, "code.as");

  auto module = builder.build();

  auto main = module.getFunction<void>("main");
  main();

  PlayerPtr ptr = std::make_shared<Player>();
  ptr->x = 3.14f;
  ptr->y = 2.43f;

  auto test = module.getFunction<void, PlayerPtr>("test");
  test(ptr);

  auto hello = module.getFunction<void, const std::string &>("hello");
  hello("world");

  auto makeHello = module.getFunction<std::string, const std::string &>("makeHello");
  std::cout << makeHello("PyXiion") << std::endl;
}