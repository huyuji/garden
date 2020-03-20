#include <boost/thread.hpp>
#include <Windows.h>
#include <boost/lexical_cast.hpp>
#include <string>

namespace
{
    static void test()
    {
        std::cout << "test in testThread enters\n";
        Sleep(3000);
        std::cout << "test in testThread exits\n";
    }

    void test1()
    {
        boost::thread t;
        t.join();
        t.join();

        t = boost::thread(test);
        t.join();
        t.join();
    }

    boost::thread_group threads;

    void test2()
    {
        threads.join_all();
        std::cout << "watch complete\n";
    }

    void test3()
    {
        std::cout << boost::lexical_cast<std::string>(boost::this_thread::get_id());
    }

    void test4()
    {
        threads.create_thread(test);
        boost::thread t1(test2);
        boost::thread t2(test2);
        t1.join();
        t2.join();
    }
}

void testThread()
{
    test3();
}