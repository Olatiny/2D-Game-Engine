#include "../src/timeline.hpp"

void test1() {
    Timeline t(0.5);

    Timeline l(&t, 1);

    std::cout << "start (should be ~0): " << l.getTime() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "before pause (should be ~2): " << l.getTime() << std::endl;

    t.pause();

    std::this_thread::sleep_for(std::chrono::seconds(4));

    t.unpause();

    std::cout << "after unpause (should be ~2): " << l.getTime() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "4 seconds later (should be ~4): " << l.getTime() << std::endl;

}

void test2() {
    Timeline t(1);

    std::this_thread::sleep_for(std::chrono::seconds(4));

    Timeline l(&t, 1);

    std::cout << "local when initiated (should be ~0): " << l.getTime() << std::endl;

    t.pause();

    std::this_thread::sleep_for(std::chrono::seconds(4));

    t.unpause();

    std::cout << "local after global paused for 4 seconds (should be ~0): " << l.getTime() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "local after running for 4 seconds (should be ~4): " << l.getTime() << std::endl;

    t.pause();

    std::this_thread::sleep_for(std::chrono::seconds(4));

    t.unpause();

    std::cout << "local after global paused for 4 seconds (should be ~4): " << l.getTime() << std::endl;

    std::cout << "global after execution (should be 8): " << t.getTime() << std::endl;
}

void test3() {
    Timeline t(1);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    Timeline l(&t, 1);

    t.changeTic(0.5);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "should be 3: " << t.getTime() << std::endl;

    std::cout << "should be 1: " << l.getTime() << std::endl;

    t.changeTic(2);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "should be 8: " << t.getTime() << std::endl;

    std::cout << "should be 6: " << l.getTime() << std::endl;
}

// void test4() {
//     Timeline t(1);

//     Timeline l(&t, 1);

//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     t.reset();

//     std::cout << "should be 0: " << l.getTime() << '\n';

//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     std::cout << "should be 2: " << l.getTime() << '\n';
// }

class TimeThread {
    Timeline* t;
    std::mutex* m;
    std::condition_variable* cv;
    int i;
    bool myTurn = false;

    public:
        TimeThread(int i, Timeline *t, std::mutex* m, std::condition_variable* cv) {
            this->i = i;
            this->t = t;
            this->m = m;
            this->cv = cv;
        }

        void run() {
            if (i == 0) {
                // global

                // t->reset();
                std::this_thread::sleep_for(std::chrono::seconds(1));

                {
                    std::unique_lock<std::mutex> lock(*m);
                    // t->pause();
                    cv->notify_all();
                    cv->wait(lock);

                    t->changeTic(2);

                    cv->notify_all();
                    cv->wait(lock);

                    t->changeTic(1);

                    cv->notify_all();
                }

            } else {
                // local
                {
                    std::unique_lock<std::mutex> lock(*m);
                    cv->wait(lock);

                    // printf("hi\n");
                    float temp = t->getTime();

                    std::cout << "local time (1): " << temp << std::endl;

                    // t->reset();

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    temp = t->getTime();

                    std::cout << "local time (1): " << temp << std::endl;

                    cv->notify_all();
                    cv->wait(lock);

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    temp = t->getTime();

                    std::cout << "local time (3): " << temp << std::endl;

                    cv->notify_all();
                    cv->wait(lock);

                    temp = t->getTime();

                    std::cout << "local time (3): " << temp << std::endl;

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    temp = t->getTime();

                    std::cout << "local time (4): " << temp << std::endl;
                }

            }
        }
};

void wrapper(TimeThread* t) {
    t->run();
}

void test5() {
    Timeline global(1);
    Timeline local(&global, 1);
    std::mutex m;
    std::condition_variable cv;

    TimeThread g(0, &global, &m, &cv);
    TimeThread l(1, &local, &m, &cv);

    std::thread gthread(wrapper, &g);
    std::thread lthread(wrapper, &l);

    lthread.join();
    gthread.join();
}

// To compile: g++ timelineTest.cpp ../src/timeline.cpp -o timelineTest -lsfml-system -pthread

int main() {
    // test1();

    // test2();

    test3();

    // test4();

    // test5();

    return 0;
}