#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include "network.hpp"
using namespace std;

Server::Server() {
    listener.listen(8080);
    listener.accept(socket);
    cout << "You are connected as a Server" << endl;
}
Server::~Server() {
    cout << "You are disconnected" << endl;
}
void Server::send(string text, sf::TcpSocket socket) {
    socket.send(text.c_str(), text.length() + 1);
}
string Server::receive() {
    
    socket.receive(buffer, sizeof(buffer), received);
    return buffer;
}


Client::Client() {
    socket.connect(ip, 8080);
    cout << "You are connected as a Player" << endl;
}
Client::~Client() {
    cout << "You are disconnected" << endl;
}
void Client::send(string text) {
    socket.send(text.c_str(), text.length() + 1);
}
string Client::receive() {
    socket.receive(buffer, sizeof(buffer), received);
    cout << buffer << endl;
    return buffer;
}