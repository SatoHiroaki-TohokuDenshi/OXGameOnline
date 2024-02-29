#include "Client.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include "OXGame.h"

#pragma comment( lib, "ws2_32.lib" )

// �|�[�g�ԍ�
const unsigned short SERVERPORT = 8888;

Client::Client()
	: socket_(0) {

}

Client::~Client() {

}

bool Client::Initialize() {
	std::cout << "You are Server Side!\n";

	//���۔���ϐ�
	int result = 0;

	//WinSock������
	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << result << " )" << std::endl;
		return false;	// ���[���I
	}
	std::cout << "Success: WSAStartup" << std::endl;

	//�\�P�b�g�̍쐬
	int sock;
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//�I��
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//�ڑ���T�[�o�̃\�P�b�g�A�h���X���ݒ�
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);	// �ق�Ƃ͂悭�Ȃ��B���߂�127.0.0.1��萔��

	//�ڑ��v��
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: connect( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return false;
	}
	std::cout << "Success: connect()" << std::endl;

	return true;
}

bool Client::Initialize(unsigned short p) {
	std::cout << "You are Server Side!\n";

	//���۔���ϐ�
	int result = 0;

	//WinSock������
	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << result << " )" << std::endl;
		return false;	// ���[���I
	}
	std::cout << "Success: WSAStartup" << std::endl;

	//�\�P�b�g�̍쐬
	int sock;
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//�I��
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//�ڑ���T�[�o�̃\�P�b�g�A�h���X���ݒ�
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(p);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);	// �ق�Ƃ͂悭�Ȃ��B���߂�127.0.0.1��萔��

	//�ڑ��v��
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: connect( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return false;
	}
	std::cout << "Success: connect()" << std::endl;

	return true;
}

void Client::PlayGame() {
	OXGame ox(CELLTYPE::CELL_O);
	bool IsSuccess = true;
	unsigned int value;

	while (true) {
		// �����̓���
		std::system("cls");
		ox.Print();
		value = ox.Input();
		ox.UpdateMySide(value);
		IsSuccess = Send(value);
		if (!IsSuccess)	break;
		if (ox.IsFinish())	break;

		// ����̓���
		std::system("cls");
		ox.Print();
		IsSuccess = Recv(&value);
		if (!IsSuccess)	break;
		ox.UpdateOtherSide(value);
		if (ox.IsFinish())	break;
	}

	if (IsSuccess) {
		std::system("cls");
		ox.Print();
		if (ox.IsWin())			std::cout << "You Win!" << std::endl;
		else if (ox.IsLose())	std::cout << "You Lose!" << std::endl;
		else					std::cout << "Draw!" << std::endl;
		std::cout << "push enter to next...";
		std::getchar();
	}
	else {
		std::cout << "Missing the Connection" << std::endl;
		std::cout << "push enter to next...";
		std::getchar();
	}
}

bool Client::Send(unsigned int value) {
	int sendValue = htonl(value);	// ���M�f�[�^ ... �l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	int ret;						// ���ۂ̔���p
	//���M
	ret = send(socket_, (char*)&sendValue, sizeof(sendValue), 0);
	//���s
	if (ret != sizeof(sendValue)) {
		return false;
	}

	//����
	return true;
}

bool Client::Recv(unsigned int* value) {
	int recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���
	int ret;		// ���ۂ̔���p
	//��M
	ret = recv(socket_, (char*)&recvValue, sizeof(recvValue), 0);
	//���s
	if (ret != sizeof(recvValue)) {
		return false;
	}

	//�������͎�M�f�[�^���o�C�g�I�[�_�[�ɕϊ�
	*value = ntohl(recvValue);
	return true;
}

bool Client::Release() {
	//���۔���ϐ�
	int result = 0;

	//����M�Ƃ��ɐؒf
	//shutdown(sock, 0x02);
	if (shutdown(socket_, SD_BOTH) != 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;

	//�\�P�b�g�̔j��
	if (closesocket(socket_) != 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	//WinSock�I������
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << result << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;

	return true;
}
