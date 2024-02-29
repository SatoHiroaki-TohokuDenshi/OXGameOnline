#include "Client.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include "OXGame.h"

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;

Client::Client()
	: socket_(0) {

}

Client::~Client() {

}

bool Client::Initialize() {
	std::cout << "You are Server Side!\n";

	//成否判定変数
	int result = 0;

	//WinSock初期化
	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << result << " )" << std::endl;
		return false;	// おーわり！
	}
	std::cout << "Success: WSAStartup" << std::endl;

	//ソケットの作成
	int sock;
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		//エラーコードを出力
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//終了
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//接続先サーバのソケットアドレス情報設定
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);	// ほんとはよくない。せめて127.0.0.1を定数化

	//接続要求
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		// エラーコードを出力
		std::cout << "Error: connect( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return false;
	}
	std::cout << "Success: connect()" << std::endl;

	return true;
}

bool Client::Initialize(unsigned short p) {
	std::cout << "You are Server Side!\n";

	//成否判定変数
	int result = 0;

	//WinSock初期化
	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << result << " )" << std::endl;
		return false;	// おーわり！
	}
	std::cout << "Success: WSAStartup" << std::endl;

	//ソケットの作成
	int sock;
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		//エラーコードを出力
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//終了
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//接続先サーバのソケットアドレス情報設定
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(p);
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);	// ほんとはよくない。せめて127.0.0.1を定数化

	//接続要求
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		// エラーコードを出力
		std::cout << "Error: connect( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
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
		// 自分の入力
		std::system("cls");
		ox.Print();
		value = ox.Input();
		ox.UpdateMySide(value);
		IsSuccess = Send(value);
		if (!IsSuccess)	break;
		if (ox.IsFinish())	break;

		// 相手の入力
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
	int sendValue = htonl(value);	// 送信データ ... ネットワークバイトオーダーに変換後の値を格納
	int ret;						// 成否の判定用
	//送信
	ret = send(socket_, (char*)&sendValue, sizeof(sendValue), 0);
	//失敗
	if (ret != sizeof(sendValue)) {
		return false;
	}

	//成功
	return true;
}

bool Client::Recv(unsigned int* value) {
	int recvValue;	// 受信データの格納領域...ネットワークバイトオーダー状態
	int ret;		// 成否の判定用
	//受信
	ret = recv(socket_, (char*)&recvValue, sizeof(recvValue), 0);
	//失敗
	if (ret != sizeof(recvValue)) {
		return false;
	}

	//成功時は受信データをバイトオーダーに変換
	*value = ntohl(recvValue);
	return true;
}

bool Client::Release() {
	//成否判定変数
	int result = 0;

	//送受信ともに切断
	//shutdown(sock, 0x02);
	if (shutdown(socket_, SD_BOTH) != 0) {
		//エラーコードを出力
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;

	//ソケットの破棄
	if (closesocket(socket_) != 0) {
		//エラーコードを出力
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	//WinSock終了処理
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << result << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;

	return true;
}
