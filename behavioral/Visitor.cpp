#include <iostream>
#include <algorithm>
#include <array>
#include <memory>

/**
 * The Visitor Interface declares a set of visiting methods that correspond to
 * component classes. The signature of a visiting method allows the visitor to
 * identify the exact class of the component that it's dealing with.
 */
class ConcreteComponentA;
class ConcreteComponentB;

class Visitor {
 public:
  virtual void VisitConcreteComponentA( std::shared_ptr<ConcreteComponentA> element)  = 0;
  virtual void VisitConcreteComponentB( std::shared_ptr<ConcreteComponentB> element)  = 0;
};

/**
 * The Component interface declares an `accept` method that should take the base
 * visitor interface as an argument.
 */

class Component {
 public:
  virtual ~Component() {}
  virtual void Accept(std::shared_ptr<Visitor> visitor)  = 0;
};

/**
 * Each Concrete Component must implement the `Accept` method in such a way that
 * it calls the visitor's method corresponding to the component's class.
 */
class ConcreteComponentA : public Component , public std::enable_shared_from_this<ConcreteComponentA> {
  /**
   * Note that we're calling `visitConcreteComponentA`, which matches the
   * current class name. This way we let the visitor know the class of the
   * component it works with.
   */
 public:
  void Accept(std::shared_ptr<Visitor> visitor)  override {
    visitor->VisitConcreteComponentA(shared_from_this());
  }
  /**
   * Concrete Components may have special methods that don't exist in their base
   * class or interface. The Visitor is still able to use these methods since
   * it's aware of the component's concrete class.
   */
  std::string ExclusiveMethodOfConcreteComponentA()  {
    return "A";
  }
};

class ConcreteComponentB : public Component  , public std::enable_shared_from_this<ConcreteComponentB> {
  /**
   * Same here: visitConcreteComponentB => ConcreteComponentB
   */
 public:
  void Accept(std::shared_ptr<Visitor> visitor)  override {
    visitor->VisitConcreteComponentB(shared_from_this());
  }
  std::string SpecialMethodOfConcreteComponentB()  {
    return "B";
  }
};

/**
 * Concrete Visitors implement several versions of the same algorithm, which can
 * work with all concrete component classes.
 *
 * You can experience the biggest benefit of the Visitor pattern when using it
 * with a complex object structure, such as a Composite tree. In this case, it
 * might be helpful to store some intermediate state of the algorithm while
 * executing visitor's methods over various objects of the structure.
 */
class ConcreteVisitor1 : public Visitor {
 public:
  void VisitConcreteComponentA( std::shared_ptr<ConcreteComponentA> element)  override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
  }

  void VisitConcreteComponentB( std::shared_ptr<ConcreteComponentB> element)  override {
    std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
  }
};

class ConcreteVisitor2 : public Visitor {
 public:
  void VisitConcreteComponentA( std::shared_ptr<ConcreteComponentA> element)  override {
    std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
  }
  void VisitConcreteComponentB( std::shared_ptr<ConcreteComponentB> element)  override {
    std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
  }
};
/**
 * The client code can run visitor operations over any set of elements without
 * figuring out their concrete classes. The accept operation directs a call to
 * the appropriate operation in the visitor object.
 */
void ClientCode(std::array< std::shared_ptr<Component> , 2> components, std::shared_ptr<Visitor> visitor) {
  // ...
  for ( std::shared_ptr<Component> comp : components) {
    comp->Accept(visitor);
  }
  // ...
}

int main(int argc, char* argv[]) {
  std::array< std::shared_ptr<Component> , 2> components = {std::make_shared<ConcreteComponentA>(), std::make_shared<ConcreteComponentB>()};
  std::cout << "The client code works with all visitors via the base Visitor interface:\n";
  std::shared_ptr<ConcreteVisitor1> visitor1 = std::make_shared<ConcreteVisitor1>();
  ClientCode(components, visitor1);
  std::cout << "\n";
  std::cout << "It allows the same client code to work with different types of visitors:\n";
  std::shared_ptr<ConcreteVisitor2> visitor2 = std::make_shared<ConcreteVisitor2>();
  ClientCode(components, visitor2);
  return 0;
}
