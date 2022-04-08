#include"Client.h"
#include"fstream"
void Client::handler_error()
{
	cout << "ERROR|LOST CONNSECTION!" << endl;
	closesocket(connection);
	system("pause");
	exit(1);
}

void Client::send_int(int number)
{
	auto num = send(connection, (char*)&number, sizeof(int), NULL);
	if (num == -1 || num == 0) {
		handler_error();
	}
}

void Client::send_message(string& msg)
{
	send_int(msg.size());
	auto num = send(connection, msg.c_str(), msg.size(), NULL);
	if (num == -1 || num == 0) handler_error();
}
int Client::take_int()
{
	int number;
	int num = recv(connection, (char*)&number, sizeof(int), NULL);
	if (num == -1 || num == 0) handler_error();
	return number;
}

string Client::take_message()
{
	int size_str = take_int();
	char* msg = new char[size_str + 1];
	auto num = recv(connection, msg, size_str, NULL);
	if (num == -1 || num == 0) handler_error();
	msg[size_str] = '\0';
	return string(msg);
}


Client::Client(string adr, u_short port, int family)
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		handler_error();
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(adr.c_str());
	addr.sin_port = htons(port);
	addr.sin_family = family;

	connection = socket(family, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		handler_error();
	}
	cout << "You get connection\n";
}

void Client::send_file(string& file_name)
{
	string err("ERROR|NOFILE!");
	if (!ifstream(file_name)) { 
		send_message(err);
		return;
	}
	send_message(file_name);
	ifstream F(file_name);
	char* str = new char[save_message_size *4];
	while (!F.eof()) {
		F.getline(str, save_message_size * sizeof(char));
		string send(str);
		if (send.size() != 0) {
			send_int(1);
			send_message(send);
		}
	}
	send_int(0);
	F.close();
}

void Client::get_file(string& file_name)
{
	send_message(file_name);
	string file_n = take_message();
	if (file_n == "ERROR|NOFILE!")return;
	ofstream F(file_n);
	while (take_int()!=0) {
		F << take_message()<<endl;
	}
	F.close();
}

void Client::read_file(string& file_name)
{
	ifstream F(file_name);
	char* str = new char[save_message_size * 4];
	while (!F.eof()) {
		F.getline(str, save_message_size * sizeof(char));
		string send(str);
		cout << str;
	}
	cout << endl;
	F.close();
}

void Client::head_function()
{
	string command,file_name;
	while (true) {
		cout << "Write command\n";
		cin >> command;
		send_message(command);
		if (command == "Exit") {
			break;
		}
		cout << "Write file's name\n";
		cin >> file_name;
		if (fun_body.find(command) != fun_body.end()) {
			fun_body[command](file_name);
		}
		else {
			cout << "ERROR|WRONG COMMAND!";
		}
	}
	closesocket(connection);
}
