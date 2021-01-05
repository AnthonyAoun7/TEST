#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
using namespace std;

class Server {
    public:
    sf::TcpSocket socket;
    sf::TcpListener listener;
    //to change for n player, now 2
    char modes[2];
    char buffer[10000];
    size_t received;

    Server();
    ~Server();

    void send(string text, sf::TcpSocket socket);
    string receive();
};

class Client {
    public:
    sf::TcpSocket socket;
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    char buffer[10000];
    size_t received;

    Client();
    ~Client();

    void send(string text);
    string receive();
};

#endif