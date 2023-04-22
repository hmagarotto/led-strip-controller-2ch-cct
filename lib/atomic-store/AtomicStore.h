#ifndef ATOMICSTORE_H
#define ATOMICSTORE_H

#pragma once

#include <cstdint>
#include <array>
#include <interrupts.h>

template<class T, uint8_t _size>
class AtomicStore
{
public:
    AtomicStore() {}
    ~AtomicStore() {}

    void set(uint8_t index, T item) {
        esp8266::InterruptLock lock;
        _items[index] = item;
    }

    auto get(uint8_t index, T item) const {
        esp8266::InterruptLock lock;
        return _items[index];
    }

private:
    std::array<T, _size> _items;

};

#endif
