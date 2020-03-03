#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class Deque {
    public:
    Deque() {
        front.resize(0);
        back.resize(0);
    };
    
    bool Empty() const { return front.empty() && back.empty();}
    
    size_t Size() const { return front.size() + back.size();};
    
    T& operator[](size_t index) {return At(index);};

    const T& operator[](size_t index) const {return At(index);};

    T& At(size_t index) { if(Empty() || index >= Size() || index < 0) throw std::out_of_range("invalid index Deque - " + std::to_string(index)); return (index >= front.size()) ? back[index - front.size()] : front[front.size() - index - 1];};
    
    const T& At(size_t index) const { if(Empty() || index >= Size()|| index < 0) throw std::out_of_range("invalid index Deque - " + std::to_string(index)); return (index >= front.size()) ? back[index - front.size()] : front[front.size() - index - 1];};

    T& Front() {return At(0);/*front.back();*/};
    
    const T& Front() const {return At(0);};
    
    T& Back() {return At(Size()-1);};
    
    const T& Back() const {return At(Size()-1);};
    
    void PushFront(const T& value) { front.push_back(value); };
    
    void PushBack(const T& value) { back.push_back(value); };

    private:
    std::vector<T> front;
    std::vector<T> back;
};
