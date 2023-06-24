#include <iostream>
#include <list>
#include <string>
#include <memory>

class IObserver {
public:
  virtual ~IObserver() {}
  virtual void Update(const std::string& message_from_subject) = 0;
};

class ISubject {
public:
  virtual ~ISubject() {}
  virtual void Attach(std::shared_ptr<IObserver> observer) = 0;
  virtual void Detach(std::shared_ptr<IObserver> observer) = 0;
  virtual void Notify() = 0;
};

class Subject : public ISubject, public std::enable_shared_from_this<Subject> {
public:
  virtual ~Subject() {
    std::cout << "Goodbye, I was the Subject.\n";
  }

  void Attach(std::shared_ptr<IObserver> observer) override {
    list_observer_.push_back(observer);
  }

  void Detach(std::shared_ptr<IObserver> observer) override {
    list_observer_.remove(observer);
  }

  void Notify() override {
    std::list<std::shared_ptr<IObserver>>::iterator iterator = list_observer_.begin();
    HowManyObserver();
    while (iterator != list_observer_.end()) {
      (*iterator)->Update(message_);
      ++iterator;
    }
  }

  void CreateMessage(std::string message = "Empty") {
    this->message_ = message;
    Notify();
  }

  void HowManyObserver() {
    std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
  }

  void SomeBusinessLogic() {
    this->message_ = "change message message";
    Notify();
    std::cout << "I'm about to do something important.\n";
  }

private:
  std::list<std::shared_ptr<IObserver>> list_observer_;
  std::string message_;
};

class Observer : public IObserver, public std::enable_shared_from_this<Observer> {
public:
  Observer(std::shared_ptr<Subject> subject) : subject_(subject) {
    std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\".\n";
    this->number_ = Observer::static_number_;
  }

  virtual ~Observer() {
    std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
  }

  void Update(const std::string& message_from_subject) override {
    message_from_subject_ = message_from_subject;
    PrintInfo();
  }

  void AttachToSubject() {
    subject_->Attach(shared_from_this());
  }

  void RemoveMeFromTheList() {
    subject_->Detach(shared_from_this());
    std::cout << "Observer \"" << number_ << "\" removed from the list.\n";
  }

  void PrintInfo() {
    std::cout << "Observer \"" << this->number_ << "\": a new message is available --> " << this->message_from_subject_ << "\n";
  }

private:
  std::string message_from_subject_;
  std::shared_ptr<Subject> subject_;
  static int static_number_;
  int number_;
};

int Observer::static_number_ = 0;

void ClientCode() {
  std::shared_ptr<Subject> subject = std::make_shared<Subject>();

  subject->CreateMessage("Welcome! :D");
  std::shared_ptr<Observer> observer1 = std::make_shared<Observer>(subject);
  std::shared_ptr<Observer> observer2 = std::make_shared<Observer>(subject);
  std::shared_ptr<Observer> observer3 = std::make_shared<Observer>(subject);

  observer1->AttachToSubject();
  observer2->AttachToSubject();
  observer3->AttachToSubject();

  observer1->RemoveMeFromTheList();

  subject->CreateMessage("Hello there!");

  observer2->RemoveMeFromTheList();

  subject->SomeBusinessLogic();
}

int main() {
  ClientCode();
  return 0;
}
