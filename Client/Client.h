#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include<thread>
#include<mutex>
#include<list>
#include<Windows.h>
#include<fstream>
#include<map>
#include<vector>
#include<string>
#include<algorithm>
#include <functional>
using namespace std;
#pragma warning(disable: 4996)
class Client {
private:
	void handler_error();
	void send_int(int number);//отослать число
	void send_message(string& msg);//отослать сообщение
	int take_int();//получение числа
	string take_message();//получение сообщени€
	SOCKET connection; //соккет
	const int save_message_size = 1024; //размер строки, которую мы можем отослать без потерь
	map<string, function<void(string&)>> fun_body{ //абстракци€ соответсвий "строка - функци€"
		{"SendFile",[&](string& F) {send_file(F); }},
		{"GetFile",[&](string& F) {get_file(F); }},
		{"ReadFile",[&](string& F) {read_file(F); }}
	};
public:
	Client() = default;
	Client(string adr, u_short port, int family = AF_INET);
	void send_file(string & file_name);//отослать файл на сервер
	void get_file(string & file_name);//получить файл с сервера и создать такой же в директории, подразумеваетс€, что это файл формата JSON
	void read_file(string & file_name);//прочитать файл в директории
	void head_function();//основан€ функци€, позвол€юща€ пользователю написать и вызывать функции
};