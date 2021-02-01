#include <iostream>
#include <string>

int main() {

    

    while(1) {
        std::string tmp;
        std::string s;
        s = "ping";
        if (std::cin >> tmp) {
            s = "MY INPUT!!";
        }
        std::cout << s << std::endl;
        
    }
	
    return 0;
}