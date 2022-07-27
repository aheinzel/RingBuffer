#include <gtest/gtest.h>
#include "ringbuffer.h"

//TODO replace "factories" with cleaner version using traits
struct StaticRingBufferFactory{
  static RingBuffer<int> * create2(){
    return new StaticRingBuffer<2, int>();
  }

  static RingBuffer<int> * create3(){
    return new StaticRingBuffer<3, int>();
  }

  static RingBuffer<int> * create5(){
    return new StaticRingBuffer<5, int>();
  }
};


struct DynamicRingBufferFactory{
  static RingBuffer<int> * create2(){
    return create(2);
  }

  static RingBuffer<int> * create3(){
    return create(3);
  }

  static RingBuffer<int> * create5(){
    return create(5);
  }

  static RingBuffer<int> * create(const size_t capacity){
    return new DynamicRingBuffer<int>(capacity);
  }
};


template <typename T>
class RingBufferTest : public testing::Test {
 public:
  RingBuffer<int> * rb2 = T::create2();
  RingBuffer<int> * rb3 = T::create3();
  RingBuffer<int> * rb5 = T::create5();

  virtual ~ RingBufferTest(){
    delete rb2;
    rb2 = nullptr;
    delete rb3;
    rb3 = nullptr;
    delete rb5;
    rb5 = nullptr;
  }
};


using RingBufferTypes = ::testing::Types<StaticRingBufferFactory, DynamicRingBufferFactory>;
TYPED_TEST_SUITE(RingBufferTest, RingBufferTypes);


TYPED_TEST(RingBufferTest, capacity) {
  EXPECT_EQ(2, this -> rb2 -> capacity());
  EXPECT_EQ(5, this -> rb5 -> capacity());
}


TYPED_TEST(RingBufferTest, empty) {
  EXPECT_TRUE(this -> rb2 -> empty());
  this -> rb2 -> put(1);
  EXPECT_FALSE(this -> rb2 -> empty());
}


TYPED_TEST(RingBufferTest, full) {
  EXPECT_FALSE(this -> rb2 -> full());
  this -> rb2 -> put(1);
  EXPECT_FALSE(this -> rb2 -> full());
  this -> rb2 -> put(2);
  EXPECT_TRUE(this -> rb2 -> full());
}


TYPED_TEST(RingBufferTest, size) {
  EXPECT_EQ(0, this -> rb3 -> size());
  this -> rb3 -> put(9);
  EXPECT_EQ(1, this -> rb3 -> size());
  this -> rb3 -> put(8);
  EXPECT_EQ(2, this -> rb3 -> size());
  this -> rb3 -> put(7);
  EXPECT_EQ(3, this -> rb3 -> size());
  this -> rb3 -> get();
  EXPECT_EQ(2, this -> rb3 -> size());
}


TYPED_TEST(RingBufferTest, put) {
  this -> rb3 -> put(1);
  this -> rb3 -> put(2);
  this -> rb3 -> put(3);

  EXPECT_EQ(1, this -> rb3 -> get());
  EXPECT_EQ(2, this -> rb3 -> get());
  EXPECT_EQ(3, this -> rb3 -> get());
}


TYPED_TEST(RingBufferTest, putWrap) {
  this -> rb3 -> put(1);
  this -> rb3 -> put(2);
  this -> rb3 -> put(3);
  this -> rb3 -> put(4);
  this -> rb3 -> put(5);

  EXPECT_EQ(3, this -> rb3 -> get());
  EXPECT_EQ(4, this -> rb3 -> get());
  EXPECT_EQ(5, this -> rb3 -> get());
}


TYPED_TEST(RingBufferTest, putIntermedGetWrap) {
  EXPECT_TRUE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(0, this -> rb3 -> size());
  this -> rb3 -> put(1);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(1, this -> rb3 -> size());
  
  this -> rb3 -> put(2);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(2, this -> rb3 -> size());
  
  EXPECT_EQ(1, this -> rb3 -> get());
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(1, this -> rb3 -> size());
  
  this -> rb3 -> put(3);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(2, this -> rb3 -> size());

  EXPECT_EQ(2, this -> rb3 -> get());
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(1, this -> rb3 -> size());

  EXPECT_EQ(3, this -> rb3 -> get());
  EXPECT_TRUE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(0, this -> rb3 -> size());
  
  this -> rb3 -> put(4);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(1, this -> rb3 -> size());

  this -> rb3 -> put(5);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_FALSE(this -> rb3 -> full());
  EXPECT_EQ(2, this -> rb3 -> size());

  this -> rb3 -> put(6);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_TRUE(this -> rb3 -> full());
  EXPECT_EQ(3, this -> rb3 -> size());

  this -> rb3 -> put(7);
  EXPECT_FALSE(this -> rb3 -> empty());
  EXPECT_TRUE(this -> rb3 -> full());
  EXPECT_EQ(3, this -> rb3 -> size());

  EXPECT_EQ(5, this -> rb3 -> get());
  EXPECT_EQ(6, this -> rb3 -> get());
  EXPECT_EQ(7, this -> rb3 -> get());
}


TYPED_TEST(RingBufferTest, getFront) {
  this -> rb3 -> put(9);
  EXPECT_EQ(9, this -> rb3 -> getFront());
  this -> rb3 -> put(8);
  this -> rb3 -> put(7);
  EXPECT_EQ(7, this -> rb3 -> getFront());
  EXPECT_EQ(8, this -> rb3 -> get());
  this -> rb3 -> put(6);
  this -> rb3 -> put(5);
  EXPECT_EQ(5, this -> rb3 -> getFront());
  EXPECT_EQ(6, this -> rb3 -> getFront());
}


TYPED_TEST(RingBufferTest, peek) {
  this -> rb3 -> put(9);
  EXPECT_EQ(9, this -> rb3 -> peek());
  EXPECT_EQ(9, this -> rb3 -> peek());
  this -> rb3 -> put(8);
  EXPECT_EQ(9, this -> rb3 -> peek());
  EXPECT_EQ(8, this -> rb3 -> getFront());
  EXPECT_EQ(9, this -> rb3 -> peek());
  this -> rb3 -> put(7);
  EXPECT_EQ(9, this -> rb3 -> peek());
  EXPECT_EQ(9, this -> rb3 -> get());
  EXPECT_EQ(7, this -> rb3 -> peek());
}


TYPED_TEST(RingBufferTest, peekFront) {
  this -> rb3 -> put(9);
  this -> rb3 -> put(8);
  EXPECT_EQ(8, this -> rb3 -> peekFront());
  EXPECT_EQ(8, this -> rb3 -> peekFront());
  this -> rb3 -> put(7);
  EXPECT_EQ(7, this -> rb3 -> peekFront());
  EXPECT_EQ(9, this -> rb3 -> get());
  EXPECT_EQ(7, this -> rb3 -> peekFront());
  this -> rb3 -> put(6);
  this -> rb3 -> put(5);
  EXPECT_EQ(5, this -> rb3 -> peekFront());
  EXPECT_EQ(5, this -> rb3 -> getFront());
  EXPECT_EQ(6, this -> rb3 -> peekFront());
}


TYPED_TEST(RingBufferTest, copyCtor) {
  FAIL() << "not implemented";
}


TEST(DynamicRingBuffer, copyCtor){
  DynamicRingBuffer<int> rb3(3);
  rb3.put(9);
  rb3.put(8);
  rb3.put(7);
  rb3.put(6);
  rb3.put(5);
  rb3.get();
  rb3.getFront();

  DynamicRingBuffer<int> rbNew(rb3);
  EXPECT_EQ(3, rbNew.capacity());
  EXPECT_EQ(1, rbNew.size());
  rb3.put(4);
  rb3.put(3);
  rb3.get();
  EXPECT_EQ(3, rbNew.capacity());
  EXPECT_EQ(1, rbNew.size());
  EXPECT_EQ(6, rbNew.get());
  EXPECT_EQ(0, rbNew.size());
  rbNew.put(99);
  EXPECT_EQ(1, rbNew.size());
  EXPECT_EQ(2, rb3.size());
  EXPECT_EQ(4, rb3.get());
  EXPECT_EQ(3, rb3.getFront());  
}


TEST(StaticRingBuffer, copyCtor){
  StaticRingBuffer<3, int> rb3;
  rb3.put(9);
  rb3.put(8);
  rb3.put(7);
  rb3.put(6);
  rb3.put(5);
  rb3.get();
  rb3.getFront();

  StaticRingBuffer<3, int> rbNew(rb3);
  EXPECT_EQ(3, rbNew.capacity());
  EXPECT_EQ(1, rbNew.size());
  rb3.put(4);
  rb3.put(3);
  rb3.get();
  EXPECT_EQ(3, rbNew.capacity());
  EXPECT_EQ(1, rbNew.size());
  EXPECT_EQ(6, rbNew.get());
  EXPECT_EQ(0, rbNew.size());
  rbNew.put(99);
  EXPECT_EQ(1, rbNew.size());
  EXPECT_EQ(2, rb3.size());
  EXPECT_EQ(4, rb3.get());
  EXPECT_EQ(3, rb3.getFront());  
}