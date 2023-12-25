#include <iostream>
#include "px/script/angel_script.hpp"
#include "some_manager.hpp"
#include "player.hpp"

const char *code =
R"(
void main() {
  Player player;

  manager.speed = 25;
  player.x = manager.speed * 3;
  manager.reset();

  player.hello();
}

EntityPtr myEntity;
void test(PlayerPtr player) {
  myEntity = PlayerPtr(player); // create a copy, because otherwise player will be nullptr (moving semantics)
  player.y = player.y * 3 + manager.speed;
  print(player.y);
}

void test() {
  myEntity.where();
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

class Enemy {
  Enemy() {
    print("Enemy created!");
  }

  void kill() {
    if (isDead) {
      print(someImportantString);
    } else {
      print("I'm dead now. " + someImportantString);
      someImportantStringMutable += " I'm already dead.";
      isDead = true;
    }
  }

  float ususedVar;
  bool isDead = false;
}
)";

void print(const std::string &str) {
  std::cout << str << std::endl;
}
void printF(float f) {
  std::cout << f << std::endl;
}

void printP(const PlayerPtr &ptr) {
  std::cout << ptr->x << std::endl;
}

void hello(Player *self) {
  fmt::print("Hello from X:{} Y:{}!\n", self->x, self->y);
}

static std::string someImportantString;
static SomeManager manager;

void registerAS(px::script::AngelScript &as) {
  // registering functions
  as.registerGlobalFunction("print", &print); // void print(const string &in)
  as.registerGlobalFunction("print", &printF); // void print(float)

  // global variables
  someImportantString = "You won!";
  as.registerGlobalVariable<const std::string>("someImportantString", &someImportantString);
  as.registerGlobalVariable("someImportantStringMutable", &someImportantString);

  // static object
  as.registerObjectType<SomeManager*>("SomeManager")
      .method<&SomeManager::reset>("reset")

      .property<&SomeManager::speed>("speed");

  as.registerGlobalVariable("manager", &manager);

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
  as.registerObjectType<EntityPtr>()
      .method<&Entity::where>("where");

  // smart ptr type
  as.registerObjectType<PlayerPtr>()
      .derived<Entity>()

      .property<&Player::x>("x")
      .property<&Player::y>("y")
      .property<&Player::maxHealth>("maxHealth")

      .method<&Player::where>("where");

//  smart.registerMethod<&Player::where>("where");

  as.registerGlobalFunction("print", &printP); // void print(const PlayerPtr &in)
}

int main(int argc, char *argv[]) {
  START_EASYLOGGINGPP(argc, argv);
  px::script::AngelScript as;

  registerAS(as);

  // building script
  auto builder = as.createModuleBuilder();
  builder.startNewModule("main");
  builder.addSection(code, "code.as");

  auto module = builder.build();

  // accessing
  auto main = module.getFunction<void>("main");

  auto test =  module.getFunction<void, PlayerPtr>("test"); // overloaded functions
  auto test2 = module.getFunction<void>           ("test");

  auto hello = module.getFunction<void, const std::string&>("hello");

  auto makeHello = module.getFunction<std::string, const std::string &>("makeHello");
  auto addHello = module.getFunction<void, px::out<std::string>, const std::string &>("addHello");

  // calling
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

  // Script classes
  auto type = module.getType("Enemy");
  auto factory = type.getFactory();
  auto killEnemy = type.getMethod<void>("kill");
  auto isDead = type.getProperty<bool>("isDead");

  auto object = *factory();

  std::cout << "isDead: " << std::boolalpha << isDead(object) << std::endl;
  killEnemy(object);

  std::cout << "isDead: " << std::boolalpha << isDead(object) << std::endl;
  killEnemy(object);
}