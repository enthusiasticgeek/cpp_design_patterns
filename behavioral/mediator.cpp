#include <iostream>
#include <string>
#include <memory>

class Mediator; // Forward declaration

class BaseComponent : public std::enable_shared_from_this<BaseComponent> {
protected:
  std::shared_ptr<Mediator> mediator_;

public:
  void set_mediator(const std::shared_ptr<Mediator>& mediator) {
    mediator_ = mediator;
  }
};

class Component1 : public BaseComponent {
public:
  void DoA();
  void DoB();
};

class Component2 : public BaseComponent {
public:
  void DoC();
  void DoD();
};

class Mediator {
private:
  std::weak_ptr<Component1> component1_;
  std::weak_ptr<Component2> component2_;

public:
  void SetComponents(const std::shared_ptr<Component1>& c1, const std::shared_ptr<Component2>& c2) {
    component1_ = c1;
    component2_ = c2;
  }

  void Notify(const std::shared_ptr<BaseComponent>& sender, const std::string& event) const {
    if (event == "A") {
      std::cout << "Mediator reacts on A and triggers the following operations:\n";
      if (auto c2 = component2_.lock()) {
        c2->DoC();
      }
    }
    if (event == "D") {
      std::cout << "Mediator reacts on D and triggers the following operations:\n";
      if (auto c1 = component1_.lock()) {
        c1->DoB();
      }
      if (auto c2 = component2_.lock()) {
        c2->DoC();
      }
    }
  }
};

void Component1::DoA() {
  std::cout << "Component 1 does A.\n";
  mediator_->Notify(shared_from_this(), "A");
}

void Component1::DoB() {
  std::cout << "Component 1 does B.\n";
  mediator_->Notify(shared_from_this(), "B");
}

void Component2::DoC() {
  std::cout << "Component 2 does C.\n";
  mediator_->Notify(shared_from_this(), "C");
}

void Component2::DoD() {
  std::cout << "Component 2 does D.\n";
  mediator_->Notify(shared_from_this(), "D");
}

void ClientCode() {
  std::shared_ptr<Component1> c1 = std::make_shared<Component1>();
  std::shared_ptr<Component2> c2 = std::make_shared<Component2>();
  std::shared_ptr<Mediator> mediator = std::make_shared<Mediator>();
  mediator->SetComponents(c1, c2);

  c1->set_mediator(mediator);
  c2->set_mediator(mediator);

  std::cout << "Client triggers operation A.\n";
  c1->DoA();
  std::cout << "\n";
  std::cout << "Client triggers operation D.\n";
  c2->DoD();
}

int main() {
  ClientCode();
  return 0;
}
