#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <typeinfo>
#include <memory>

// The Memento interface provides a way to retrieve the memento's metadata,
// such as creation date or name. However, it doesn't expose the
// Originator's state.
class IMemento {
public:
    virtual std::string GetName() const = 0;
    virtual std::string GetState() const = 0;
    virtual std::chrono::system_clock::time_point GetDate() const = 0;
    virtual ~IMemento() {}
};

// The ConcreteMemento contains the infrastructure for storing the
// Originator's state.
class ConcreteMemento : public IMemento {
private:
    std::string state_;
    std::chrono::system_clock::time_point date_;

public:
    ConcreteMemento(const std::string& state)
        : state_(state), date_(std::chrono::system_clock::now()) {
    }

    // The Originator uses this method when restoring its state.
    std::string GetState() const override {
        return state_;
    }

    // The rest of the methods are used by the Caretaker to display
    // metadata.
    std::string GetName() const override {
        return std::to_string(date_.time_since_epoch().count()) + " / (" + state_.substr(0, 9) + ")...";
    }

    std::chrono::system_clock::time_point GetDate() const override {
        return date_;
    }
};

// The Originator holds some important state that may change over time. It
// also defines a method for saving the state inside a memento and another
// method for restoring the state from it.
class Originator {
private:
    std::string state_;

public:
    Originator(const std::string& state) : state_(state) {
        std::cout << "Originator: My initial state is: " << state_ << std::endl;
    }

    // The Originator's business logic may affect its internal state.
    // Therefore, the client should backup the state before launching
    // methods of the business logic via the Save() method.
    void DoSomething() {
        std::cout << "Originator: I'm doing something important." << std::endl;
        state_ = GenerateRandomString(30);
        std::cout << "Originator: and my state has changed to: " << state_ << std::endl;
    }

    std::string GenerateRandomString(int length = 10) {
        const std::string allowedSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string result;

        while (length > 0) {
            result += allowedSymbols[rand() % allowedSymbols.length()];
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
            length--;
        }

        return result;
    }

    // Saves the current state inside a memento.
    std::unique_ptr<IMemento> Save() {
        return std::make_unique<ConcreteMemento>(state_);
    }

    // Restores the Originator's state from a memento object.
    void Restore(const IMemento* memento) {
        const ConcreteMemento* concreteMemento = dynamic_cast<const ConcreteMemento*>(memento);
        if (concreteMemento == nullptr) {
            throw std::runtime_error("Unknown memento class " + std::string(typeid(*memento).name()));
        }

        state_ = concreteMemento->GetState();
        std::cout << "Originator: My state has changed to: " << state_ << std::endl;
    }
};

// The Caretaker doesn't depend on the Concrete Memento class. Therefore, it
// doesn't have access to the originator's state, stored inside the memento.
// It works with all mementos via the base Memento interface.
class Caretaker {
private:
    std::vector<std::unique_ptr<IMemento>> mementos_;
    Originator* originator_;

public:
    Caretaker(Originator* originator) : originator_(originator) {
    }

    ~Caretaker() = default;

    void Backup() {
        std::cout << "\nCaretaker: Saving Originator's state..." << std::endl;
        mementos_.push_back(originator_->Save());
    }

    void Undo() {
        if (mementos_.empty()) {
            return;
        }

        std::unique_ptr<IMemento>& memento = mementos_.back();
        mementos_.pop_back();

        std::cout << "Caretaker: Restoring state to: " << memento->GetName() << std::endl;

        try {
            originator_->Restore(memento.get());
        } catch (const std::runtime_error&) {
            Undo();
        }
    }

    void ShowHistory() const {
        std::cout << "Caretaker: Here's the list of mementos:" << std::endl;
        for (const std::unique_ptr<IMemento>& memento : mementos_) {
            std::cout << memento->GetName() << std::endl;
        }
    }
};

int main() {
    // Client code.
    Originator* originator = new Originator("Super-duper-super-puper-super.");
    Caretaker* caretaker = new Caretaker(originator);

    caretaker->Backup();
    originator->DoSomething();

    caretaker->Backup();
    originator->DoSomething();

    caretaker->Backup();
    originator->DoSomething();

    std::cout << std::endl;
    caretaker->ShowHistory();

    std::cout << "\nClient: Now, let's rollback!\n" << std::endl;
    caretaker->Undo();

    std::cout << "\n\nClient: Once more!\n" << std::endl;
    caretaker->Undo();

    std::cout << std::endl;

    delete caretaker;
    delete originator;

    return 0;
}
