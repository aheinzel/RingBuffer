#include <gtest/gtest.h>
#include "ringbuffer.h"


TEST(RingBuffer, capacity) {
  RingBuffer<2, int> rb2;
  RingBuffer<5, int> rb5;
  EXPECT_EQ(2, rb2.capacity());
  EXPECT_EQ(5, rb5.capacity());
}

TEST(RingBuffer, empty) {
  RingBuffer<2, int> rb;
  EXPECT_TRUE(rb.empty());
  rb.put(1);
  EXPECT_FALSE(rb.empty());
}

TEST(RingBuffer, full) {
  RingBuffer<2, int> rb;
  EXPECT_FALSE(rb.full());
  rb.put(1);
  EXPECT_FALSE(rb.full());
  rb.put(2);
  EXPECT_TRUE(rb.full());
}

TEST(RingBuffer, size) {
  RingBuffer<3, int> rb;
  EXPECT_EQ(0, rb.size());
  rb.put(9);
  EXPECT_EQ(1, rb.size());
  rb.put(8);
  EXPECT_EQ(2, rb.size());
  rb.put(7);
  EXPECT_EQ(3, rb.size());
  rb.get();
  EXPECT_EQ(2, rb.size());
}

TEST(RingBuffer, put) {
  RingBuffer<3, int> rb;
  rb.put(1);
  rb.put(2);
  rb.put(3);

  EXPECT_EQ(1, rb.get());
  EXPECT_EQ(2, rb.get());
  EXPECT_EQ(3, rb.get());
}

TEST(RingBuffer, putWrap) {
  RingBuffer<3, int> rb;
  rb.put(1);
  rb.put(2);
  rb.put(3);
  rb.put(4);
  rb.put(5);

  EXPECT_EQ(3, rb.get());
  EXPECT_EQ(4, rb.get());
  EXPECT_EQ(5, rb.get());
}

TEST(RingBuffer, putIntermedGetWrap) {
  RingBuffer<3, int> rb;
  EXPECT_TRUE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(0, rb.size());
  rb.put(1);
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(1, rb.size());
  
  rb.put(2);
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(2, rb.size());
  
  EXPECT_EQ(1, rb.get());
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(1, rb.size());
  
  rb.put(3);
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(2, rb.size());

  EXPECT_EQ(2, rb.get());
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(1, rb.size());

  EXPECT_EQ(3, rb.get());
  EXPECT_TRUE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(0, rb.size());
  
  rb.put(4);
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(1, rb.size());

  rb.put(5);
  EXPECT_FALSE(rb.empty());
  EXPECT_FALSE(rb.full());
  EXPECT_EQ(2, rb.size());

  rb.put(6);
  EXPECT_FALSE(rb.empty());
  EXPECT_TRUE(rb.full());
  EXPECT_EQ(3, rb.size());

  rb.put(7);
  EXPECT_FALSE(rb.empty());
  EXPECT_TRUE(rb.full());
  EXPECT_EQ(3, rb.size());

  EXPECT_EQ(5, rb.get());
  EXPECT_EQ(6, rb.get());
  EXPECT_EQ(7, rb.get());
}