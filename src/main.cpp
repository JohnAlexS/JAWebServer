#include <iostream>
#include <ServerManager.h>


int main(){
    try{
        ServerManager sm;
        std::string command;

        std::thread listener([&]{
            sm.client_listener();
        });

        while(true){
            std::cout << "JAWServer>> ";
            std::cin >> command;

            if(command == "stop"){
                std::cout << "Shutting down threads" << std::endl;
                break;
            }
        }

        sm.shutdown();
        listener.join();
        
        return 0;
    }

    catch(std::runtime_error e){
        std::cout << "Unrecoverable error, exiting: " << e.what() << std::endl;
        return 1;
    }
}