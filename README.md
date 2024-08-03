# Bita Queue Library

## Overview

The Bita Queue Library is a collection of thread-safe, lock-free queue implementations in C++. This project includes:

- `LfQueue`: A lock-free queue implementation using a single-producer, single-consumer (SPSC) approach.
- `TfQueue`: A (to be implemented) lock-free queue with different characteristics (e.g., multi-producer, multi-consumer).
- `Bita`: A master module that aggregates various queue implementations into a single namespace for easy use.

## Features

- **Lock-Free Queues**: Both implementations provide lock-free operations, ensuring high performance in concurrent environments.
- **Thread-Safe**: Designed to handle multiple threads without requiring locks, avoiding contention and improving scalability.
- **Namespace Organization**: All queue implementations are contained within the `Bita` namespace for clarity and convenience.

## Directory Structure

- `lfqueue.hpp`: Header file for the `LfQueue` implementation.
- `lfqueue_impl.hpp`: Implementation file for the `LfQueue`.
- `tfqueue.hpp`: Header file for the `TfQueue` implementation.
- `tfqueue_impl.hpp`: Implementation file for the `TfQueue`.
- `bita.hpp`: Master header file that includes `LfQueue` and `TfQueue`, exposing them under the `Bita` namespace.

## Usage

### Example

```cpp
#include "bita.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

const int NUM_PRODUCERS = 2;
const int NUM_CONSUMERS = 2;
const int NUM_ITEMS = 10;

// Global lock-free queue
Bita::LfQueue<int> queue;

void producer(int id) {
    for (int i = 0; i < NUM_ITEMS; ++i) {
        int value = id * NUM_ITEMS + i;
        queue.push(value);
        std::cout << "Producer " << id << " produced " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void consumer(int id) {
    for (int i = 0; i < 1000000; ++i) {
        auto value = queue.pop();
        if (value.has_value()) {
            std::cout << "Consumer " << id << " consumed " << value.value() << std::endl;
        } else {
            std::cout << "Consumer " << id << " found queue empty" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

int main() {
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Launch producers
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.emplace_back(producer, i);
    }

    // Launch consumers
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.emplace_back(consumer, i);
    }
    
    // Wait for all threads to finish
    for (auto& t : producers) {
        t.join();
    }

    for (auto& t : consumers) {
        t.join();
    }

    return 0;
}
```


### Explanation

- **Overview**: Briefly describes what the library does.
- **Features**: Highlights key features of the library.
- **Directory Structure**: Lists and explains the purpose of each file in the project.
- **Usage**: Provides a simple example of how to use the `LfQueue` and `TfQueue` classes.
- **Files**: Details what each file contains.
- **Building**: Instructions for building the project.
- **License**: Licensing information.
- **Contributing**: Information on how others can contribute.
- **Contact**: Contact information for further questions or comments.

Feel free to adjust the content as needed for your specific project.
