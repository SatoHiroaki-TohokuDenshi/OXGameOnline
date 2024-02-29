#include "Server.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "OXGame.h"

#pragma comment( lib, "ws2_32.lib" )

// �|�[�g�ԍ�
const unsigned short SERVERPORT = 8888;

Server::Server()
	: listenSock_(0), clientSock_(0) {

}

Server::~Server() {

}

bool Server::Initialize() {
	std::cout << "You are Server Side!\n";

	//���۔���ϐ�
	int result = 0;

	//WinSock
	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << result << " )" << std::endl;
		return false;
	}
	std::cout << "Success: WSAStartup" << std::endl;

	//listen socket
	int listenSock_;
	listenSock_ = (int)socket(AF_INET, SOCK_STREAM, 0);	// 0�Ŏ����ݒ�
	// ���X���\�P�b�g�쐬���s
	if (listenSock_ < 0)
	{
		// �G���[�R�[�h���o��
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//bind
	struct sockaddr_in bindAddr;	// bind�p�̃\�P�b�g�A�h���X���
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenSock_, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: bind( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return false;
	}
	std::cout << "Success: bind()" << std::endl;

	// ���X����Ԃɐݒ�	�L���[�̃T�C�Y:1
	if (listen(listenSock_, 1) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: listen( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return false;
	}
	std::cout << "Success: listen()" << std::endl;

	std::cout << "---wait connect---" << std::endl;

	struct sockaddr_in clientAddr;		// �ڑ��v�������Ă����N���C�A���g�̃\�P�b�g�A�h���X���i�[�̈�
	int addrlen = sizeof(clientAddr);	// clientAddr�̃T�C�Y

	// �N���C�A���g�����connect()���󂯂āA�R�l�N�V�����m���ς݂̃\�P�b�g�쐬
	clientSock_ = (int)accept(listenSock_, (struct sockaddr*)&clientAddr, &addrlen);
	if (clientSock_ < 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: accept( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return false;
	}
	std::cout << "Success: accept() " << std::endl;

	return true;
}

bool Server::Initialize(unsigned short p) {
	std::cout << "You are Server Side!\n";

	//WinSock
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << ret << " )" << std::endl;
		return false;
	}
	std::cout << "Success: WSAStartup" << std::endl;

	//listen socket
	int listenSock_;
	listenSock_ = (int)socket(AF_INET, SOCK_STREAM, 0);	// 0�Ŏ����ݒ�
	//���X���\�P�b�g�쐬���s
	if (listenSock_ < 0)
	{
		//�G���[�R�[�h���o��
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//�I��
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//bind
	struct sockaddr_in bindAddr;	// bind�p�̃\�P�b�g�A�h���X���
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(p);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenSock_, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: bind( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//�I��
		return false;
	}
	std::cout << "Success: bind()" << std::endl;

	//���X����Ԃɐݒ�	�L���[�̃T�C�Y:1
	if (listen(listenSock_, 1) != 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: listen( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//�I��
		return false;
	}
	std::cout << "Success: listen()" << std::endl;

	std::cout << "---wait connect---" << std::endl;

	struct sockaddr_in clientAddr;		// �ڑ��v�������Ă����N���C�A���g�̃\�P�b�g�A�h���X���i�[�̈�
	int addrlen = sizeof(clientAddr);	// clientAddr�̃T�C�Y

	//�N���C�A���g�Ƃ̐ڑ��\�P�b�g���쐬
	clientSock_ = (int)accept(listenSock_, (struct sockaddr*)&clientAddr, &addrlen);
	if (clientSock_ < 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: accept( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//�I��
		return false;
	}
	std::cout << "Success: accept() " << std::endl;

	return true;
}

void Server::PlayGame() {
	OXGame ox(CELLTYPE::CELL_X);
	bool IsSuccess = true;
	unsigned int value;

	while (true) {
		// ����̓���
		std::system("cls");
		ox.Print();
		IsSuccess = Recv(&value);
		if (!IsSuccess)	break;
		ox.UpdateOtherSide(value);
		if (ox.IsFinish())	break;

		// �����̓���
		std::system("cls");
		ox.Print();
		value = ox.Input();
		ox.UpdateMySide(value);
		IsSuccess = Send(value);
		if (!IsSuccess)	break;
		if (ox.IsFinish())	break;
	}

	if (IsSuccess) {
		std::system("cls");
		ox.Print();
		if (ox.IsWin())			std::cout << "You Win!" << std::endl;
		else if (ox.IsLose())	std::cout << "You Lose!" << std::endl;
		else					std::cout << "Draw!" << std::endl;
	}
	else {
		std::cout << "Missing the Connection" << std::endl;
	}
}

bool Server::Send(unsigned int value) {
	int sendValue = htonl(value);	// ���M�f�[�^ ... �l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	int ret;						// ���۔���
	//���M
	ret = send(clientSock_, (char*)&sendValue, sizeof(sendValue), 0);
	//���s
	if (ret != sizeof(sendValue)) {
		return false;
	}

	//����
	return true;
}

bool Server::Recv(unsigned int* value) {
	int recvValue = 0;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���
	int ret;			// ���۔���

	//��M
	ret = recv(clientSock_, (char*)&recvValue, sizeof(recvValue), 0);
	//���s
	if (ret != sizeof(recvValue)) {
		return false;
	}

	//�������͎�M�f�[�^���o�C�g�I�[�_�[�ɕϊ�
	*value = ntohl(recvValue);
	return true;
}

bool Server::Release() {
	//���۔���ϐ�
	int result = 0;

	//����M�Ƃ��ɐؒf
	//shutdown(sock, 0x02);
	if (shutdown(clientSock_, SD_BOTH) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;


	//�\�P�b�g�̔j��
	if (closesocket(clientSock_) != 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	if (closesocket(listenSock_) != 0) {
		//�G���[�R�[�h���o��
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	//WinSock�I������
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << result << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;

	return false;
}
