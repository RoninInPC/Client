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
	void send_int(int number);//�������� �����
	void send_message(string& msg);//�������� ���������
	int take_int();//��������� �����
	string take_message();//��������� ���������
	SOCKET connection; //������
	const int save_message_size = 1024; //������ ������, ������� �� ����� �������� ��� ������
	map<string, function<void(string&)>> fun_body{ //���������� ����������� "������ - �������"
		{"SendFile",[&](string& F) {send_file(F); }},
		{"GetFile",[&](string& F) {get_file(F); }},
		{"ReadFile",[&](string& F) {read_file(F); }}
	};
public:
	Client() = default;
	Client(string adr, u_short port, int family = AF_INET);
	void send_file(string & file_name);//�������� ���� �� ������
	void get_file(string & file_name);//�������� ���� � ������� � ������� ����� �� � ����������, ���������������, ��� ��� ���� ������� JSON
	void read_file(string & file_name);//��������� ���� � ����������
	void head_function();//�������� �������, ����������� ������������ �������� � �������� �������
};