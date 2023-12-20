#include <iostream>
#include "px/script/angel_script.hpp"

const char *code =
R"(
void main() {
  Player player;

  player.x = 2435;
  player.hello();
}

PlayerPtr myPlayer;
void test(PlayerPtr player) {
  myPlayer = player;
  player.y = player.y * 3;
  print(player.y);
}
void test() {
  myPlayer.y = myPlayer.y + 20;
  myPlayer.where();
}

void hello(const string &in name) {
  print(makeHello(name));
}

string makeHello(const string &in name) {
  return "Hello " + name + "!";
}

void addHello(string &out toEdit, const string &in name) {
  toEdit += "\nHello " + name + "!";
}
)";

void print(const std::string &str) {
  std::cout << str << std::endl;
}
void printF(float f) {
  std::cout << f << std::endl;
}

struct Entity {
  virtual ~Entity() = default;

  float health {30.0f};

  virtual void where() const = 0;
};
typedef std::shared_ptr<Entity> EntityPtr;

PX_AS_TYPENAME          (Entity, "Entity");
PX_AS_TYPENAME_SMART_PTR(Entity, "EntityPtr");

struct Player : public Entity {
  virtual ~Player() = default;

  void where() const final {
    std::cout << x << " " << y << std::endl;
  }

  const float maxHealth {5.0f};

  float x{};
  float y{};
};
typedef std::shared_ptr<Player> PlayerPtr;

PX_AS_TYPENAME          (Player, "Player");
PX_AS_TYPENAME_SMART_PTR(Player, "PlayerPtr");

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
  as.registerObjectType<Player>("Player")
    .property<&Player::x>("x")
    .property<&Player::y>("y")
    .property<&Player::maxHealth>("maxHealth")

    .method<&Player::where>("where")

    .proxyMethod("hello", &hello)

    // from derived
    .property<&Player::health>("health");

//   base ptr type
  as.registerObjectType<EntityPtr>();

  // smart ptr type
  as.registerObjectType<PlayerPtr>()
    .derived<Entity>()

    .property<&Player::x>("x")
    .property<&Player::y>("y")
    .property<&Player::maxHealth>("maxHealth")

    .method<&Player::where>("where");

//  smart.registerMethod<&Player::where>("where");

  as.registerGlobalFunction("print", &printP); // void print(const PlayerPtr &in)

  // building script
  auto builder = as.createModuleBuilder();
  builder.startNewModule("main");
  builder.addSection(code, "code.as");

  auto module = builder.build();

  auto main = module.getFunction<void>("main");

  auto test =  module.getFunction<void, PlayerPtr>("test"); // overloaded functions
  auto test2 = module.getFunction<void>           ("test");

  auto hello = module.getFunction<void, const std::string&>("hello");

  auto makeHello = module.getFunction<std::string, const std::string &>("makeHello");
  auto addHello = module.getFunction<void, px::out<std::string>, const std::string &>("addHello");

  main();

  {
    PlayerPtr ptr = std::make_shared<Player>();
    ptr->x = 3.14f;
    ptr->y = 2.43f;

    std::cout << "test(ptr):\t"; test(ptr);
    std::cout << "test():\t";    test2();
  }

  hello("world");

  std::string str = makeHello("PyXiion");
  addHello(str, "World");
  std::cout << str << std::endl;
}