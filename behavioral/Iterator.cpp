/**
 *  * Iterator Design Pattern
 *   *
 *    * Intent: Lets you traverse elements of a collection without exposing its
 *     * underlying representation (list, stack, tree, etc.).
 *      */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

/**
 *  * C++ has its own implementation of iterator that works with a different
 *   * generics containers defined by the standard library.
 *    */

template <typename T, typename U>
class Iterator {
public:
    typedef typename std::vector<T>::iterator iter_type;
    Iterator(std::shared_ptr<U> p_data, bool reverse = false) : m_p_data_(p_data) {
        m_it_ = getPtr()->m_data_.begin();
    }

    void First() {
        m_it_ = getPtr()->m_data_.begin();
    }

    void Next() {
        m_it_++;
    }

    bool IsDone() {
        return (m_it_ == getPtr()->m_data_.end());
    }

    iter_type Current() {
        return m_it_;
    }

    std::shared_ptr<U> getPtr(){
    std::shared_ptr<U> sp( this->m_p_data_ );
    return sp;
    }

private:
    std::weak_ptr<U> m_p_data_;
    iter_type m_it_;
};

/**
 *  * Generic Collections/Containers provides one or several methods for retrieving
 *   * fresh iterator instances, compatible with the collection class.
 *    */

template <class T>
class Container : public std::enable_shared_from_this<Container<T>>  {
//class Container  {
    friend class Iterator<T, Container>;

public:
    void Add(T a) {
        m_data_.push_back(a);
    }

    std::shared_ptr<Iterator<T, Container>> CreateIterator() {
        //return std::make_shared<Iterator<T, Container>>(this);
        return std::make_shared<Iterator<T, Container>>(this->shared_from_this());
        //return std::make_shared<Iterator<T, Container>>(std::enable_shared_from_this<Container<T>>::shared_from_this());
    }

private:
    std::vector<T> m_data_;
};

class Data {
public:
    Data(int a = 0) : m_data_(a) {}

    void set_data(int a) {
        m_data_ = a;
    }

    int data() {
        return m_data_;
    }

private:
    int m_data_;
};

/**
 *  * The client code may or may not know about the Concrete Iterator or Collection
 *   * classes, for this implementation the container is generic so you can used
 *    * with an int or with a custom class.
 *     */
void ClientCode() {
    std::cout << "________________Iterator with int______________________________________" << std::endl;
    std::shared_ptr<Container<int>> cont = std::make_shared<Container<int>>();

    for (int i = 0; i < 10; i++) {
        cont->Add(i);
    }

    std::shared_ptr<Iterator<int, Container<int>>> it = cont->CreateIterator();
    for (it->First(); !it->IsDone(); it->Next()) {

        std::cout << it->Current()[0] << std::endl;
        int a = it->Current()[0];
    }

    //Container<Data> cont2;
    std::shared_ptr<Container<Data>> cont2 = std::make_shared<Container<Data>>();
    Data a(100), b(1000), c(10000);
    cont2->Add(a);
    cont2->Add(b);
    cont2->Add(c);

    std::cout << "________________Iterator with custom Class______________________________" << std::endl;
    std::shared_ptr<Iterator<Data, Container<Data>>> it2 = cont2->CreateIterator();
    for (it2->First(); !it2->IsDone(); it2->Next()) {
        std::cout << it2->Current()->data() << std::endl;
    }
    //delete it;
    //delete it2;

}

int main() {
    ClientCode();
    return 0;
}
