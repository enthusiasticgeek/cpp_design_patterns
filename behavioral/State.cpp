#include <iostream>
#include <memory>
#include <typeinfo>

class State;  // Forward declaration

class Context : public std::enable_shared_from_this<Context> {
private:
    std::shared_ptr<State> state_;

public:
    Context();

    void TransitionTo(const std::shared_ptr<State>& state);

    void Request1();
    void Request2();
};

class State {
protected:
    std::weak_ptr<Context> context_;

public:
    virtual ~State() {}

    void set_context(const std::shared_ptr<Context>& context) {
        context_ = context;
    }

    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
};

class ConcreteStateA : public State {
public:
    void Handle1() override;
    void Handle2() override;
};

class ConcreteStateB : public State {
public:
    void Handle1() override;
    void Handle2() override;
};

Context::Context() : state_(nullptr) {}

void Context::TransitionTo(const std::shared_ptr<State>& state) {
    std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
    state_ = state;
    state_->set_context(shared_from_this());
}

void Context::Request1() {
    state_->Handle1();
}

void Context::Request2() {
    state_->Handle2();
}

void ConcreteStateA::Handle1() {
    std::cout << "ConcreteStateA handles request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the context.\n";

    if (auto context = context_.lock()) {
        context->TransitionTo(std::make_shared<ConcreteStateB>());
    } else {
        std::cout << "Invalid weak pointer to context.\n";
    }
}

void ConcreteStateA::Handle2() {
    std::cout << "ConcreteStateA handles request2.\n";
}

void ConcreteStateB::Handle1() {
    std::cout << "ConcreteStateB handles request1.\n";
}

void ConcreteStateB::Handle2() {
    std::cout << "ConcreteStateB handles request2.\n";
    std::cout << "ConcreteStateB wants to change the state of the context.\n";

    if (auto context = context_.lock()) {
        context->TransitionTo(std::make_shared<ConcreteStateA>());
    } else {
        std::cout << "Invalid weak pointer to context.\n";
    }
}

void ClientCode() {
    auto context = std::make_shared<Context>();
    auto stateA = std::make_shared<ConcreteStateA>();
    context->TransitionTo(stateA);

    context->Request1();
    context->Request2();
}

int main() {
    ClientCode();
    return 0;
}
