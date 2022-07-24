#pragma once

#include<assert.h>

template<size_t requestedCapacity, typename T>
class RingBuffer{
private:
    T _buffer[requestedCapacity];
    bool _full = false;
    size_t _head = 0;
    size_t _tail = 0;
    const size_t _capacity = requestedCapacity;
    
    size_t increment(size_t & i){
        ++i;
        if(i == capacity()){
            i = 0;
        }

        return i;
    }

public:
    size_t capacity() const{
        return _capacity;
    }

    size_t size() const{
        if(full()){
            return capacity();
        }else{
            return _head >= _tail ? _head - _tail : capacity() + _head - _tail;
        }
    }

    bool full() const{
        return _full;
    }

    bool empty() const{
        return (!full()) && _head == _tail;
    }

    const T & get(){
        assert(!empty());
        T & v = _buffer[_tail];
        increment(_tail);
        _full = false;
        return v;
    }

    void put(const T & v){
        _buffer[_head] = v;
        if(full()){
            increment(_tail);
        }

        increment(_head);
        _full = _head == _tail;
    }

    void reset(){
        _full = false;
        _head = 0;
        _tail = 0;
    }
};