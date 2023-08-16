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
            Timer(){
                reset();
            }

            int getElapsedTimeSeconds(){
                auto endpointTime = std::chrono::high_resolution_clock::now();

                auto start = std::chrono::time_point_cast<std::chrono::seconds>(m_Start).time_since_epoch().count();
                auto stop = std::chrono::time_point_cast<std::chrono::seconds>(endpointTime).time_since_epoch().count();

                return stop-start;
            }

            void reset(){
                 m_Start = std::chrono::high_resolution_clock::now();
            }

            ~Timer(){}
        
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };
} 
