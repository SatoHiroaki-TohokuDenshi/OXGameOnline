#include "Functions.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

#include "OXGame.h"

#pragma comment( lib, "ws2_32.lib" )

// �|�[�g�ԍ�
const unsigned short SERVERPORT = 8888;

int Client() {
	std::cout << "Send Int Client" << std::endl;

	// WinSock������
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << ret << " )" << std::endl;
		return 1;	// ���[���I
	}
	std::cout << "Success: WSAStartup" << std::endl;

	// �\�P�b�g�̍쐬
	int sock;
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return 1;
	}
	std::cout << "Success: socket()" << std::endl;

	// �ڑ���T�[�o�̃\�P�b�g�A�h���X���ݒ�
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	std::string SA;
	std::cout << "Input Server Address : ";
	std::cin >> SA;
	inet_pton(AF_INET, SA.c_str(), &serverAddr.sin_addr.s_addr);	// �ق�Ƃ͂悭�Ȃ��B���߂�127.0.0.1��萔��

	// �ڑ��v��
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: connect( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return 1;
	}
	std::cout << "Success: connect()" << std::endl;

	PlayClient(sock);

	// ����M�Ƃ��ɐؒf
	// shutdown(sock, 0x02);
	if (shutdown(sock, SD_BOTH) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;

	// �\�P�b�g�̔j��
	if (closesocket(sock) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	// WinSock�I������
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << ret << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;
	return 0;
}

int Server() {
	std::cout << "Send Int Server" << std::endl;

	// WinSock������
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << ret << " )" << std::endl;
		return 1;
	}
	std::cout << "Success: WSAStartup" << std::endl;

	// ���X���\�P�b�g�̍쐬
	int listenSock;
	listenSock = (int)socket(AF_INET, SOCK_STREAM, 0);	// 0�Ŏ����ݒ�
	// ���X���\�P�b�g�쐬���s
	if (listenSock < 0)
	{
		// �G���[�R�[�h���o��
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return 1;
	}
	std::cout << "Success: socket()" << std::endl;

	// bind
	struct sockaddr_in bindAddr;	// bind�p�̃\�P�b�g�A�h���X���
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �\�P�b�g�A�h���X���ݒ�	���Œ�̃|�[�g�ԍ��ݒ�
	if (bind(listenSock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: bind( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return 1;
	}
	std::cout << "Success: bind()" << std::endl;

	// ���X����Ԃɐݒ�	�L���[�̃T�C�Y:1
	if (listen(listenSock, 1) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: listen( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return 1;
	}
	std::cout << "Success: listen()" << std::endl;

	std::cout << "---wait connect---" << std::endl;

	// �ʐM�p�\�P�b�g ( �N���C�A���g�̃\�P�b�g�Ƃ��̃\�P�b�g�ԂɃR�l�N�V�������m�� )
	int sock;


	struct sockaddr_in clientAddr;		// �ڑ��v�������Ă����N���C�A���g�̃\�P�b�g�A�h���X���i�[�̈�
	int addrlen = sizeof(clientAddr);	// clientAddr�̃T�C�Y

	// �N���C�A���g�����connect()���󂯂āA�R�l�N�V�����m���ς݂̃\�P�b�g�쐬
	sock = (int)accept(listenSock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: accept( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// �I��
		return 1;
	}
	std::cout << "Success: accept() " << std::endl;


	// ����M��
	PlayServer(sock);

	// ����M�Ƃ��ɐؒf
	// shutdown(sock, 0x02);
	if (shutdown(sock, SD_BOTH) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;


	// �\�P�b�g�̔j��
	if (closesocket(sock) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	if (closesocket(listenSock) != 0) {
		// �G���[�R�[�h���o��
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	// WinSock�I������
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << ret << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;

	return 0;
}

/// <summary>
/// int�̃f�[�^���o�C�g�I�[�_�[��ϊ����Ă��瑗�M
/// </summary>
/// <param name="sock">���M�Ɏg�p����\�P�b�g�̃\�P�b�g�f�B�X�N���v�^</param>
/// <param name="value">���M�f�[�^</param>
/// <returns>���� : true  ���s : false</returns>
bool Send(int sock, unsigned int value) {
	int sendValue = htonl(value);	// ���M�f�[�^ ... �l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	int ret;		// ���ۂ̔���p
	// ���M
	ret = send(sock, (char*)&sendValue, sizeof(sendValue), 0);
	// ���s
	if (ret != sizeof(sendValue)) {
		return false;
	}

	// ����
	return true;
}

/// <summary>
/// unsigned int�f�[�^����M�A�o�C�g�I�[�_�[��ϊ�
/// </summary>
/// <param name="sock">��M�Ɏg�p����\�P�b�g�̃\�P�b�g�f�B�X�N���v�^</param>
/// <param name="value">��M�f�[�^�̊i�[�̈�̐擪�A�h���X</param>
/// <returns>���� : true ���s : false</returns>
bool Recv(int sock, unsigned int* value)
{
	int recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���
	int ret;		// ���ۂ̔���p
	// ��M
	ret = recv(sock, (char*)&recvValue, sizeof(recvValue), 0);
	// ���s
	if (ret != sizeof(recvValue)) {
		return false;
	}

	// �������͎�M�f�[�^���o�C�g�I�[�_�[�ɕϊ�
	*value = ntohl(recvValue);
	return true;
}

void PlayClient(int& sock) {
	OXGame ox(CELLTYPE::CELL_O);
	bool IsSuccess = true;
	unsigned int value;

	while (true) {
		// �����̓���
		std::system("cls");
		ox.Print();
		value = ox.Input();
		ox.UpdateMySide(value);
		IsSuccess = Send(sock, value);
		if (!IsSuccess)	break;
		if (ox.IsFinish())	break;

		// ����̓���
		std::system("cls");
		ox.Print();
		IsSuccess = Recv(sock, &value);
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
	}
	else {
		std::cout << "Missing the Connection" << std::endl;
	}
}

void PlayServer(int& sock) {
	OXGame ox(CELLTYPE::CELL_X);
	bool IsSuccess = true;
	unsigned int value;

	while (true) {
		// ����̓���
		std::system("cls");
		ox.Print();
		IsSuccess = Recv(sock, &value);
		if (!IsSuccess)	break;
		ox.UpdateOtherSide(value);
		if (ox.IsFinish())	break;

		// �����̓���
		std::system("cls");
		ox.Print();
		value = ox.Input();
		ox.UpdateMySide(value);
		IsSuccess = Send(sock, value);
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
