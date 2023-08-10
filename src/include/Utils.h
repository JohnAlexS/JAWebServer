#pragma once
#include <chrono>
#include <string>

namespace utils
{
    constexpr unsigned int hash(const char* s, int off = 0){
        return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off]; 
    }

    class Timer{
        public: 
            Timer(std::string name) : name(name){
                m_Start = std::chrono::high_resolution_clock::now();
            }

            ~Timer(){
                auto endpointTime = std::chrono::high_resolution_clock::now();

                auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
                auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(endpointTime).time_since_epoch().count();

                auto duration  = stop-start;
                double ms = duration*0.001;

                std::cout << "Timer " << name << " took " << ms << " milliseconds to execute.";
            }
        
        private:
            std::string name;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };
} 
