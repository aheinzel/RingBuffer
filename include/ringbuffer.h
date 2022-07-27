#include<assert.h>
#include<cstddef>

template<typename T>
class RingBuffer{
private:
    T * _buffer = 0;
    const size_t _capacity;
    bool _full = false;
    size_t _head = 0;
    size_t _tail = 0;
    
    size_t increment(size_t & i){
        ++i;
        if(i == capacity()){
            i = 0;
        }

        return i;
    }

    size_t decrement(size_t & i){
        i = (i == 0 ? capacity() : i) - 1;
        return i;
    }

protected:
    RingBuffer(T * buffer, const RingBuffer & other)
        : _buffer(buffer), _capacity(other._capacity), _full(other._full), _head(other._head), _tail(other._tail){
        assert(_capacity == other._capacity);
        for(size_t i = 0; i < _capacity; ++i){
            _buffer[i] = other._buffer[i];
        }
    }

public:
    RingBuffer(T * buffer, size_t capacity): _buffer(buffer), _capacity(capacity){
    }

    virtual ~RingBuffer() = 0;

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

    const T & peek(){
        assert(!empty());
        return _buffer[_tail];
    }

    const T & getFront(){
        assert(!empty());
        decrement(_head);
        _full = false;
        return _buffer[_head];
    }

    const T & peekFront(){
        assert(!empty());
        size_t tmp = _head;
        decrement(tmp);
        return _buffer[tmp];
    }

    void reset(){
        _full = false;
        _head = 0;
        _tail = 0;
    }
};

template<typename T>
RingBuffer<T>::~RingBuffer(){}


template<size_t requestedCapacity, typename T>
class StaticRingBuffer : public RingBuffer<T>{
public:
    T _buffer[requestedCapacity];
 
 public:
    StaticRingBuffer(): RingBuffer<T>(_buffer, requestedCapacity){
    }

    StaticRingBuffer(const StaticRingBuffer & other): RingBuffer<T>(_buffer, other){
    }

    virtual ~StaticRingBuffer(){
    }
};


template<typename T>
class DynamicRingBuffer : public RingBuffer<T>{
private:
    T * _buffer;
    bool _deleteBuffer;

    DynamicRingBuffer(T * buffer, const DynamicRingBuffer & other): _buffer(buffer), _deleteBuffer(true), RingBuffer<T>(buffer, other){
    }

 public:
    DynamicRingBuffer(size_t capacity): DynamicRingBuffer<T>(new T [capacity], true, capacity){
    }

    DynamicRingBuffer(T * buffer, bool takeBufferOwnership, size_t capacity): _buffer(buffer), _deleteBuffer(takeBufferOwnership), RingBuffer<T>(buffer, capacity){
    }

    DynamicRingBuffer(const DynamicRingBuffer & other): DynamicRingBuffer<T>(new T [other.capacity()], other){
    }

    virtual ~DynamicRingBuffer(){
        if(_deleteBuffer){
            delete _buffer;
            _buffer = nullptr;
        }
    }
};