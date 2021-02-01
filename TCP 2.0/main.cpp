#include <iostream>
#include <SFML/Network.hpp>
#include <string>

const unsigned short PORT = 8080;
//const std::string IPADDRESS("192.168.0.100");
sf::IpAddress IPADDRESS = sf::IpAddress::getLocalAddress();

std::string msgClient = "ping";
std::string msgServer;

//for the client
sf::TcpSocket socket;
//for the server
std::vector<sf::TcpSocket *> clients;
std::vector<std::string> ids;
int actual_nb = 0;
//specify clients number (>=1)
int total_nb = 2;

sf::Mutex globalMutex;
bool quit = false;

std::string SocketToID(sf::TcpSocket *s)
{
	int ind;
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i] == s)
			ind = i;
	}
	return ids[ind];
}

void DoStuffC(void)
{
	//static std::string oldMsg;
	while (!quit)
	{
		//if there is an input collected by the other thread, send it to server
		//if not, send a ping
		sf::Packet packetSend;

		///////////////////////////////////
		//WE SHOULD PING WHEN NOT SENDING//
		///////////////////////////////////
		/*globalMutex.lock();
		packetSend << "ping";
		globalMutex.unlock();*/

		globalMutex.lock();
		packetSend << msgClient;
		globalMutex.unlock();

		socket.send(packetSend);

		//receive updated matrix
		std::string msg;
		sf::Packet packetReceive;

		socket.receive(packetReceive);
		//if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
		if ((packetReceive >> msg) && !msg.empty())
		{
			std::cout << msg << std::endl;
			//oldMsg = msg;
		}
	}
}

void DoStuffS(void)
{
	//static std::string oldMsg;
	while (!quit)
	{
		//go through all clients
		for (int i = 0; i < clients.size(); i++)
		{
			//check if we receive input from client
			std::string msg;
			sf::Packet packetReceive;

			clients[i]->receive(packetReceive);

			//if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
			if ((packetReceive >> msg) && !msg.empty())
			{
				//print client input if available
				std::cout << SocketToID(clients[i]) << " input : " << msg << std::endl;
				//oldMsg = msg;
			}
			sf::Packet packetSend;
			msgServer = "Receiving : updated matrix...";
			//globalMutex.lock();
			packetSend << msgServer;
			//globalMutex.unlock();

			clients[i]->send(packetSend);
		}
	}
}

void Server(void)
{
	sf::TcpListener listener;
	listener.listen(PORT);

	while (actual_nb < total_nb)
	{
		sf::TcpSocket *socket = new sf::TcpSocket;
		listener.accept(*socket);

		sf::Packet id_packet;
		std::string id;
		if (socket->receive(id_packet) == sf::Socket::Done)
		{
			id_packet >> id;
		}
		std::cout << "Player " << id << " is connected (" << actual_nb + 1 << "/" << total_nb << ") - IP : " << (*socket).getRemoteAddress() << std::endl;

		clients.push_back(socket);
		ids.push_back(id);
		actual_nb++;
	}
}

bool Client(void)
{
	std::string id;
	std::cout << "Enter online id: ";
	std::cin >> id;
	if (socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
	{
		sf::Packet id_packet;
		id_packet << id;
		socket.send(id_packet);
		std::cout << "Connected\n";
		return true;
	}
	return false;
}

void GetInputC(void)
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit")
		quit = true;
	globalMutex.lock();
	msgClient = s;
	globalMutex.unlock();
}
void GetInputS(void)
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit: ";
	getline(std::cin, s);
	if (s == "exit")
		quit = true;
}

int main(int argc, char *argv[])
{
	sf::Thread *thread = 0;

	char who;
	std::cout << "Do you want to be a server (s) or a client (c) ? ";
	std::cin >> who;

	if (who == 's')
	{
		std::string password = " ";
		//ask for password to login as server
		while (password != "tetris")
		{
			std::cout << "Enter password : ";
			std::cin >> password;
			if (password != "tetris")
			{
				std::cout << "Wrong password! Please try again.\n"
						  << std::endl;
			}
		}
		std::cout << "Server running...\n";
		std::cout << "Waiting for the " << total_nb << " players\n";
		Server();
		std::cout << "The Game started!\n"
				  << std::endl;
	}
	else
		Client();

	if (who == 's')
		thread = new sf::Thread(&DoStuffS);
	else
		thread = new sf::Thread(&DoStuffC);
	thread->launch();

	while (!quit)
	{
		if (who == 's')
			GetInputS();
		else
			GetInputC();
	}

	if (thread)
	{
		thread->wait();
		delete thread;
	}
	return 0;
}