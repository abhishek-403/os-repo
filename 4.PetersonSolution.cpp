#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

const int COUNT = 10; // Adjust the number of iterations for testing

atomic<bool> flag[2];
atomic<int> turn;

void criticalSection(int id)
{
    for (int i = 0; i < COUNT; ++i)
    {
        flag[id] = true;
        turn = 1 - id;
        // Wait until it's your turn
        while (flag[1 - id] && turn == 1 - id)
            ; // Busy wait
        // Critical Section
        cout << "Thread " << id << " is in critical section (Iteration: " << i + 1 << ")" << endl;
        // Simulate work in the critical section
        this_thread::sleep_for(chrono::milliseconds(100));
        flag[id] = false;
        // Non-Critical Section
        cout << "Thread " << id << " is in non-critical section (Iteration: " << i + 1 << ")" << endl;
        // Simulate work outside the critical section
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void threadFunction(int id)
{
    criticalSection(id);
}

int main()
{
    flag[0] = flag[1] = false;
    turn = 0;

    // Create two threads
    thread t1(threadFunction, 0);
    thread t2(threadFunction, 1);

    // Join the threads with the main thread
    t1.join();
    t2.join();

    return 0;
}
