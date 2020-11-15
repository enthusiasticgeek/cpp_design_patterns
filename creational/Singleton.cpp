#include <iostream>
#include <memory>
#include <thread>
#include <mutex>          // std::mutex, std::unique_lock

/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class Singleton
{

    /**
     * The Singleton's constructor should always be private to prevent direct
     * construction calls with the `new` operator.
     */

protected:
    std::string value_;
    
    explicit Singleton(const std::string value): value_(value)
    {
    }

    static std::shared_ptr<Singleton> singleton_;
    static std::mutex mutex_;

    /**
     * Singletons should not be cloneable.
     */
    Singleton(Singleton &other) = delete;
    
    /**
     * Singletons should not be moveable.
     */
	Singleton(Singleton&&) = delete;
    /**
     * Singletons should not be assignable.
     */
    //void operator=(const Singleton &) = delete;
    
    Singleton& operator = (const Singleton&) = delete;
    
    
	Singleton& operator = (Singleton&&) = delete;
	
	//~Singleton() = default;
	
	//Exposing non-default constructor
    template<typename ...Arg> std::shared_ptr<Singleton> static Create(Arg&&...arg) {
      struct EnableMakeShared : public Singleton {
         EnableMakeShared(Arg&&...arg) : Singleton(std::forward<Arg>(arg)...) {}
      };
      return std::make_shared<EnableMakeShared>(std::forward<Arg>(arg)...);
    }

public:

    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static std::shared_ptr<Singleton> GetInstance(const std::string& value);
    /**
     * Finally, any singleton should define some business logic, which can be
     * executed on its instance.
     */
    void SomeBusinessLogic()
    {
        // ...
    }

    std::string value() const{
        return value_;
    } 
    
};

std::shared_ptr<Singleton> Singleton::singleton_(nullptr);
std::mutex Singleton::mutex_;

/**
 * Static methods should be defined outside the class.
 */
std::shared_ptr<Singleton> Singleton::GetInstance(const std::string& value)
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangerous in case two instance threads wants to access at the same time
     */
    std::lock_guard<std::mutex> lock(mutex_);
    if(singleton_==nullptr){
        singleton_ = Create(value);
    }
    return singleton_;
}


void ThreadFoo(){
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::shared_ptr<Singleton> singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
}

void ThreadBar(){
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::shared_ptr<Singleton> singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
}


int main()
{
    std::cout <<"If you see the same value, then singleton was reused (yay!\n" <<
                "If you see different values, then 2 singletons were created (booo!!)\n\n" <<
                "RESULT:\n";   
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();

    return 0;
}
