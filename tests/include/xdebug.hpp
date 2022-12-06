#ifndef XDEBUG_H
#define XDEBUG_H

#include <iostream>
#include <thread>
#include <map>
#include <mutex>
#include <chrono>
#include <utility>
#include <functional>

    #if defined XDEBUG
        /* Set your own std::ostream& XDEBUG_STD_OUT */
        #ifndef XDEBUG_STD_OUT 
            #define XDEBUG_STD_OUT std::cout //defualt std::cout
        #endif

        /* __THREAD__ default is std::this_thread::get_id()*/
        #ifndef __THREAD__
            #define __THREAD__ std::this_thread::get_id()
        #endif
        static std::mutex _mutexPrint;
        static int _thread_id = 1;
        static std::map<std::thread::id, int> _threads{{__THREAD__, 0}};

        /* Each new thread that calls this get a new ID and it return the threads ID */
        #define __THREADID__ [&]{ \
            if(_threads.find(__THREAD__) == _threads.end()){ \
                _threads[__THREAD__] = _thread_id; \
                return _thread_id++; \
            }else {return _threads[__THREAD__];}}

        /* Timing Wrapper function which excutes the function that is passed in*/
        #define EXECUTE(func, ...) [&]{ \
            auto start = std::chrono::high_resolution_clock::now(); \
            auto ret = func(__VA_ARGS__); \
            auto end = std::chrono::high_resolution_clock::now(); \
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
            XDEBUG_STD_OUT << "Duration in mircoseconds : " << duration.count() << std::endl; \
            return ret; \
        }()

        /* debug print just for comments */
        #define DEBUG(...) [](const auto&...x){XDEBUG_STD_OUT << "Debug : ";((XDEBUG_STD_OUT << x),...); XDEBUG_STD_OUT << std::endl; }(__VA_ARGS__);
        /* debug print for threading purposes */
        #define DEBUGT(...) [](const auto&...x){std::lock_guard<std::mutex> guard(_mutexPrint); XDEBUG_STD_OUT << "DebugThread " << __THREADID__() << " : ";((XDEBUG_STD_OUT << x),...); XDEBUG_STD_OUT << std::endl; }(__VA_ARGS__);
        /* trace value outputs __FILE__ __LINE__ and variables names and variable values*/
        #define TRACEV(...) [](const auto&...x){XDEBUG_STD_OUT << __FILE__<<":"<<__LINE__<<" : " << __FUNCTION__ <<" ||"; XDEBUG_STD_OUT <<#__VA_ARGS__ << ", "; XDEBUG_STD_OUT << "= "; ((XDEBUG_STD_OUT << x <<", "),...); XDEBUG_STD_OUT << std::endl; }(__VA_ARGS__);
        /* trace outputs __FILE__ __LINE__ and variable values*/
        #define TRACE(...) [](const auto&...x){ XDEBUG_STD_OUT << __FILE__<<":"<<__LINE__<<" : " << __FUNCTION__ <<" |";(( XDEBUG_STD_OUT <<"|" << x ), ...); XDEBUG_STD_OUT << std::endl; }(__VA_ARGS__);
    #else
        /* off debug statement */
        #define DEBUG(...)
        /* off debug statement */
        #define DEBUGT(...) 
        /* off trace statement */
        #define TRACEV(...)
        /* off trace statement */
        #define TRACE(...)
    #endif

#endif