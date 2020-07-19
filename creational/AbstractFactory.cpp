#include <iostream>
#include <memory>

/**
 * Each distinct product of a product family should have a base interface. All
 * variants of the product must implement this interface.
 */
class AbstractCompact {
 public:
  virtual ~AbstractCompact(){};
  virtual std::string UsefulFunctionA() const = 0;
};

/**
 * Concrete Products are created by corresponding Concrete Factories.
 */
class ConcreteCorolla : public AbstractCompact {
 public:
  std::string UsefulFunctionA() const override {
    return "The result of the product : Toyota Corolla.";
  }
};

class ConcreteCivic : public AbstractCompact {
  std::string UsefulFunctionA() const override {
    return "The result of the product : Honda Civic.";
  }
};

/**
 * Here's the the base interface of another product. All products can interact
 * with each other, but proper interaction is possible only between products of
 * the same concrete variant.
 */
class AbstractMidSize {
  /**
   * Product B is able to do its own thing...
   */
 public:
  virtual ~AbstractMidSize(){};
  virtual std::string UsefulFunctionB() const = 0;
  /**
   * ...but it also can collaborate with the Compact.
   *
   * The Abstract Factory makes sure that all products it creates are of the
   * same variant and thus, compatible.
   */
  virtual std::string AnotherUsefulFunctionB(const std::unique_ptr<AbstractCompact> collaborator) const = 0;
};

/**
 * Concrete Products are created by corresponding Concrete Factories.
 */
class ConcreteCamry : public AbstractMidSize {
 public:
  std::string UsefulFunctionB() const override {
    return "The result of the product Toyota Camry.";
  }
  /**
   * The variant, Product B1, is only able to work correctly with the variant,
   * Product A1. Nevertheless, it accepts any instance of AbstractCompact as an
   * argument.
   */
    std::string AnotherUsefulFunctionB(const std::unique_ptr<AbstractCompact> collaborator) const override {
    const std::string result = collaborator->UsefulFunctionA();
    return "The result of the B1 collaborating with ( " + result + " )";
  }

};

class ConcreteAccord : public AbstractMidSize {
 public:
  std::string UsefulFunctionB() const override {
    return "The result of the product Honda Accord.";
  }
  /**
   * The variant, Product B2, is only able to work correctly with the variant,
   * Product A2. Nevertheless, it accepts any instance of AbstractCompact as an
   * argument.
   */
  std::string AnotherUsefulFunctionB(const std::unique_ptr<AbstractCompact> collaborator) const override {
    const std::string result = collaborator->UsefulFunctionA();
    return "The result of the B2 collaborating with ( " + result + " )";
  }
};

/**
 * The Abstract Factory interface declares a set of methods that return
 * different abstract products. These products are called a family and are
 * related by a high-level theme or concept. Products of one family are usually
 * able to collaborate among themselves. A family of products may have several
 * variants, but the products of one variant are incompatible with products of
 * another.
 */
class AbstractFactory {
 public:
  //virtual AbstractCompact *CreateCompact() const = 0;
  //virtual AbstractMidSize *CreateMidSize() const = 0;
  virtual std::unique_ptr<AbstractCompact> CreateCompact() const = 0;
  virtual std::unique_ptr<AbstractMidSize> CreateMidSize() const = 0;
};

/**
 * Concrete Factories produce a family of products that belong to a single
 * variant. The factory guarantees that resulting products are compatible. Note
 * that signatures of the Concrete Factory's methods return an abstract product,
 * while inside the method a concrete product is instantiated.
 */
class ConcreteToyota : public AbstractFactory {
 public:
  std::unique_ptr<AbstractCompact> CreateCompact() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcreteCorolla>(ConcreteCorolla());
  }
  std::unique_ptr<AbstractMidSize> CreateMidSize() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcreteCamry>(ConcreteCamry());
  }

};

/**
 * Each Concrete Factory has a corresponding product variant.
 */
class ConcreteHonda : public AbstractFactory {
 public:
  std::unique_ptr<AbstractCompact> CreateCompact() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcreteCivic>(ConcreteCivic());
  }
  std::unique_ptr<AbstractMidSize> CreateMidSize() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcreteAccord>(ConcreteAccord());
  }
};

/**
 * The client code works with factories and products only through abstract
 * types: AbstractFactory and AbstractProduct. This lets you pass any factory or
 * product subclass to the client code without breaking it.
 */
void ClientCode(const std::unique_ptr<AbstractFactory> factory) {
  std::unique_ptr<AbstractCompact> compact = factory->CreateCompact();
  const std::unique_ptr<AbstractMidSize> midsize = factory->CreateMidSize();
  std::cout << midsize->UsefulFunctionB() << "\n";
  std::cout << midsize->AnotherUsefulFunctionB(std::move(compact)) << "\n";

}


int main() {
  std::cout << "Client: Testing client code with the first factory type:\n";
  std::unique_ptr<ConcreteToyota> toyota = std::make_unique<ConcreteToyota>(ConcreteToyota());
  ClientCode(std::move(toyota));
  std::cout << std::endl;
  std::cout << "Client: Testing the same client code with the second factory type:\n";
  std::unique_ptr<ConcreteHonda> honda = std::make_unique<ConcreteHonda>(ConcreteHonda());
  ClientCode(std::move(honda));
  return 0;
}
