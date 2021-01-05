#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <curses.h>
#include "network.hpp"
using namespace std;

int main() {
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	bool done = false;
	string id;
	string text = "";

	cout << "Enter online id: ";
	std::cin >> id;

	socket.connect(ip, 8080);

	sf::Packet packet;
	packet << id;
	socket.send(packet);
	socket.setBlocking(false);

	while (!done) {
		sf::Packet statusPacket, matrixPacket;
		string mat, tmp, text;
		text = "I got an input : ";
		//if input is given
		if (cin >> tmp) {
			text += tmp;
			statusPacket << text;
			socket.send(statusPacket);
		}
		//receive matrix even if not inpu was sent
		socket.receive(matrixPacket);
		matrixPacket >> mat;
		//output matrix
		cout << mat << endl;
	

	}

	return 0;
}