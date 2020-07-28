#include <iostream>
#include <memory>// Prototype Design Pattern
#include <algorithm>
/**
 * The Target defines the domain-specific interface used by the client code.
 */
class Target {
 public:
  virtual ~Target() = default;

  virtual std::string Request() const {
    return "Target: The default target's behavior.";
  }
};

/**
 * The Adaptee contains some useful behavior, but its interface is incompatible
 * with the existing client code. The Adaptee needs some adaptation before the
 * client code can use it.
 */
class Adaptee {
 public:
  std::string SpecificRequest() const {
    return ".eetpadA eht fo roivaheb laicepS";
  }
};

/**
 * The Adapter makes the Adaptee's interface compatible with the Target's
 * interface.
 */
class Adapter : public Target {
 private:
  std::shared_ptr<Adaptee> adaptee_;

 public:
  Adapter(std::shared_ptr<Adaptee> adaptee) : adaptee_(std::move(adaptee)) {}
  std::string Request() const override {
    std::string to_reverse = this->adaptee_->SpecificRequest();
    std::reverse(to_reverse.begin(), to_reverse.end());
    return "Adapter: (TRANSLATED) " + to_reverse;
  }
};

/**
 * The client code supports all classes that follow the Target interface.
 */
void ClientCode(const std::shared_ptr<Target> target) {
  std::cout << target->Request();
}

int main(int argc, char* argv[]) {
  std::cout << "Client: I can work just fine with the Target objects:\n";
  std::shared_ptr<Target> target = std::make_shared<Target>();
  ClientCode(target);
  std::cout << "\n\n";
  std::shared_ptr<Adaptee> adaptee = std::make_shared<Adaptee>();
  std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
  std::cout << "Adaptee: " << adaptee->SpecificRequest();
  std::cout << "\n\n";
  std::cout << "Client: But I can work with it via the Adapter:\n";
  std::shared_ptr<Adapter> adapter = std::make_shared<Adapter>(adaptee);
  ClientCode(adapter);
  std::cout << "\n";

  return 0;
}
