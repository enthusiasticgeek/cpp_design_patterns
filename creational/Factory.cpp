//Adopted from Alexander Shvets "Dive Into Design Patterns"
//Copyright (c) 2020 Pratik M Tambe <enthusiasticgeek@gmail.com>
//Author: enthusiasticgeek
//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./Factory
#include <iostream>
#include <memory>

/**
 * The WritingInstrument interface declares the operations that all concrete products must
 * implement.
 */

class WritingInstrument {
 public:
  virtual ~WritingInstrument() {}
  virtual std::string Operation() const = 0;
};

/**
 * Concrete WritingInstruments provide various implementations of the WritingInstrument interface.
 */
class ConcretePen : public WritingInstrument {
 public:
  std::string Operation() const override {
    return "{Pen}";
  }
};

class ConcretePencil : public WritingInstrument {
 public:
  std::string Operation() const override {
    return "{Pencil}";
  }
};

class ConcreteStylus : public WritingInstrument {
 public:
  std::string Operation() const override {
    return "{Stylus}";
  }
};

/**
 * The WritingInstrumentCreator class declares the factory method that is supposed to return an
 * object of a WritingInstrument class. The WritingInstrumentCreator's subclasses usually provide the
 * implementation of this method.
 */

class WritingInstrumentCreator {
  /**
   * Note that the WritingInstrumentCreator may also provide some default implementation of the
   * factory method.
   */
 public:
  virtual ~WritingInstrumentCreator(){};
  //virtual WritingInstrument* FactoryMethod() const = 0;
  virtual std::unique_ptr<WritingInstrument> FactoryMethod() const = 0;
  /**
   * Also note that, despite its name, the WritingInstrumentCreator's primary responsibility is
   * not creating products. Usually, it contains some core business logic that
   * relies on WritingInstrument objects, returned by the factory method. Subclasses can
   * indirectly change that business logic by overriding the factory method and
   * returning a different type of product from it.
   */

  std::string Scribble() const {
    // Call the factory method to create a WritingInstrument object.
    //WritingInstrument* product = this->FactoryMethod();
    std::unique_ptr<WritingInstrument> product = this->FactoryMethod();
    // Now, use the product.
    std::string result = "WritingInstrumentCreator: Scribbling with " + product->Operation();
    return result;
  }
};

/**
 * Concrete WritingInstrumentCreators override the factory method in order to change the
 * resulting product's type.
 */
class ConcreteWritingInstrumentCreator1 : public WritingInstrumentCreator {
  /**
   * Note that the signature of the method still uses the abstract product type,
   * even though the concrete product is actually returned from the method. This
   * way the WritingInstrumentCreator can stay independent of concrete product classes.
   */
 public:
  //WritingInstrument* FactoryMethod() const override {
  std::unique_ptr<WritingInstrument> FactoryMethod() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcretePen>(ConcretePen());
  }
};

class ConcreteWritingInstrumentCreator2 : public WritingInstrumentCreator {
 public:
  //WritingInstrument* FactoryMethod() const override {
  std::unique_ptr<WritingInstrument> FactoryMethod() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcretePencil>(ConcretePencil());
  }
};

class ConcreteWritingInstrumentCreator3 : public WritingInstrumentCreator {
 public:
  //WritingInstrument* FactoryMethod() const override {
  std::unique_ptr<WritingInstrument> FactoryMethod() const override {
    //no need to use std::move as Return Value Optimization (RVO) by compiler would take place.	  
    //Assigning memory to the heap
    return std::make_unique<ConcreteStylus>(ConcreteStylus());
  }
};


/**
 * The WritingWritingClient code works with an instance of a concrete creator, albeit through
 * its base interface. As long as the WritingWritingClient keeps working with the creator via
 * the base interface, you can pass it any creator's subclass.
 */
void WritingClientCode(const std::unique_ptr<WritingInstrumentCreator> creator) {
  // ...
  std::cout << "WritingClient: I'm oblivious to the creator's class, but it still works.\n"
            << creator->Scribble() << std::endl;
  // ...
}


/**
 * The Application picks a creator's type depending on the configuration or
 * environment.
 */

int main(int argc, char* argv[]) {
  std::cout << "App: Launched with the ConcreteWritingInstrumentCreator1.\n";
  std::unique_ptr<WritingInstrumentCreator> creator1 = std::make_unique<ConcreteWritingInstrumentCreator1>(ConcreteWritingInstrumentCreator1());
  WritingClientCode(std::move(creator1));
  std::cout << std::endl;
  std::cout << "App: Launched with the ConcreteWritingInstrumentCreator2.\n";
  std::unique_ptr<WritingInstrumentCreator> creator2 = std::make_unique<ConcreteWritingInstrumentCreator2>(ConcreteWritingInstrumentCreator2());
  WritingClientCode(std::move(creator2));
  std::cout << std::endl;
  std::cout << "App: Launched with the ConcreteWritingInstrumentCreator3.\n";
  std::unique_ptr<WritingInstrumentCreator> creator3 = std::make_unique<ConcreteWritingInstrumentCreator3>(ConcreteWritingInstrumentCreator3());
  WritingClientCode(std::move(creator3));
  return 0;
}

