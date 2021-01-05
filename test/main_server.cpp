#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <curses.h>
#include <vector>
#include "network.hpp"
using namespace std;
////////////////////////////////////////////////////////////////
//There is still an output misfunction when we start the game
//The actual classes correspond to previous versions of network
////////////////////////////////////////////////////////////////

int main() {
    string password = " ";
    //ask for password to login as server
    while (password != "tetris") {
        cout << "Enter password : ";
        cin >> password;
        if (password != "tetris") {
            cout << "Wrong password! Please try again." << endl;
            cout << endl;
        }
    }

    int actual_nb = 0;
    int total_nb = 2;
    cout << "Server Running" << endl;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    bool done = false;
    vector<sf::TcpSocket*> clients;

    listener.listen(8080);
    selector.add(listener);

    //wait for players 
    while (actual_nb < total_nb) {
        //wait until smthg happens
        if (selector.wait()) {
            //if we are receiving a connection
            if (selector.isReady(listener)) {
                sf::TcpSocket *socket = new sf::TcpSocket;
                listener.accept(*socket);
                sf::Packet packet;
                string id;
                if (socket->receive(packet) == sf::Socket::Done) {
                    packet >> id;
                }

                cout << id << " has connected to the game" << endl;
                clients.push_back(socket);
                selector.add(*socket);
                actual_nb++;
            }
        }
    }
    //game loop
    while (!done) {  
        //visit each client in turn
        for (int i=0; i< clients.size(); i++) {
            string mat = "matrix";
            if (selector.isReady(*clients[i])) {
                sf::Packet statusPacket;
                string text;
                //if there is a client input
                if (clients[i]->receive(statusPacket) == sf::Socket::Done) {
                    statusPacket >> text;
                    cout << text << " from client " << i << endl;  
                }  
                //send matrix to client even if there was no input
                cout << "Sending matrix to client " << i << endl;
                sf::Packet matrixPacket;
                matrixPacket << mat;
                clients[i]->send(matrixPacket);
            } 
            
        } 
    }
    //delete sockets
    for(vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++) {
        delete *it;
    }

    return 0;
}