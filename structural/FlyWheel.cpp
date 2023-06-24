#include <iostream>
#include <cstring>
#include <unordered_map>
#include <memory>

/**
 * Flyweight Design Pattern
 *
 * Intent: Lets you fit more objects into the available amount of RAM by sharing
 * common parts of state between multiple objects, instead of keeping all of the
 * data in each object.
 */

struct SharedState
{
    std::string brand_;
    std::string model_;
    std::string color_;

    SharedState(const std::string& brand, const std::string& model, const std::string& color)
        : brand_(brand), model_(model), color_(color)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const SharedState& ss)
    {
        return os << "[ " << ss.brand_ << " , " << ss.model_ << " , " << ss.color_ << " ]";
    }
};

struct UniqueState
{
    std::string owner_;
    std::string plates_;

    UniqueState(const std::string& owner, const std::string& plates)
        : owner_(owner), plates_(plates)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const UniqueState& us)
    {
        return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
    }
};

/**
 * The Flyweight stores a common portion of the state (also called intrinsic
 * state) that belongs to multiple real business entities. The Flyweight accepts
 * the rest of the state (extrinsic state, unique for each entity) via its
 * method parameters.
 */
class Flyweight
{
private:
    std::shared_ptr<SharedState> shared_state_;

public:
    Flyweight(const std::shared_ptr<SharedState>& shared_state)
        : shared_state_(shared_state)
    {
    }

    void Operation(const UniqueState& unique_state) const
    {
        std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n";
    }
};

/**
 * The Flyweight Factory creates and manages the Flyweight objects. It ensures
 * that flyweights are shared correctly. When the client requests a flyweight,
 * the factory either returns an existing instance or creates a new one if it
 * doesn't exist yet.
 */
class FlyweightFactory
{
private:
    std::unordered_map<std::string, std::shared_ptr<Flyweight>> flyweights_;

    std::string GetKey(const SharedState& ss) const
    {
        return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
    }

public:
    FlyweightFactory(std::initializer_list<SharedState> shared_states)
    {
        for (const SharedState& ss : shared_states)
        {
            std::string key = GetKey(ss);
            flyweights_[key] = std::make_shared<Flyweight>(std::make_shared<SharedState>(ss));
        }
    }

    std::shared_ptr<Flyweight> GetFlyweight(const SharedState& shared_state)
    {
        std::string key = GetKey(shared_state);

        if (flyweights_.find(key) == flyweights_.end())
        {
            std::cout << "FlyweightFactory: Can't find a flyweight, creating a new one.\n";
            flyweights_[key] = std::make_shared<Flyweight>(std::make_shared<SharedState>(shared_state));
        }
        else
        {
            std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }

        return flyweights_.at(key);
    }

    void ListFlyweights() const
    {
        size_t count = flyweights_.size();
        std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
        for (const auto& pair : flyweights_)
        {
            std::cout << pair.first << "\n";
        }
    }
};

void AddCarToPoliceDatabase(
    const std::shared_ptr<FlyweightFactory>& ff, const std::string& plates, const std::string& owner,
    const std::string& brand, const std::string& model, const std::string& color)
{
    std::cout << "\nClient: Adding a car to the database.\n";
    const std::shared_ptr<Flyweight>& flyweight = ff->GetFlyweight({ brand, model, color });
    flyweight->Operation({ owner, plates });
}

int main()
{
    std::shared_ptr<FlyweightFactory> factory = std::make_shared<FlyweightFactory>(std::initializer_list<SharedState>{
        { "Chevrolet", "Camaro2018", "pink" },
        { "Mercedes Benz", "C300", "black" },
        { "Mercedes Benz", "C500", "red" },
        { "BMW", "M5", "red" },
        { "BMW", "X6", "white" }
    });

    factory->ListFlyweights();

    AddCarToPoliceDatabase(factory,
        "CL234IR",
        "James Doe",
        "BMW",
        "M5",
        "red");

    AddCarToPoliceDatabase(factory,
        "CL234IR",
        "James Doe",
        "BMW",
        "X1",
        "red");

    factory->ListFlyweights();

    return 0;
}
