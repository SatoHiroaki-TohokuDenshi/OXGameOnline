#include "Server.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "OXGame.h"

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;

Server::Server()
	: listenSock_(0), clientSock_(0) {

}

Server::~Server() {

}

bool Server::Initialize() {
	std::cout << "You are Server Side!\n";

	//成否判定変数
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
	listenSock_ = (int)socket(AF_INET, SOCK_STREAM, 0);	// 0で自動設定
	// リスンソケット作成失敗
	if (listenSock_ < 0)
	{
		// エラーコードを出力
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//bind
	struct sockaddr_in bindAddr;	// bind用のソケットアドレス情報
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenSock_, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		// エラーコードを出力
		std::cout << "Error: bind( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return false;
	}
	std::cout << "Success: bind()" << std::endl;

	// リスン状態に設定	キューのサイズ:1
	if (listen(listenSock_, 1) != 0) {
		// エラーコードを出力
		std::cout << "Error: listen( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return false;
	}
	std::cout << "Success: listen()" << std::endl;

	std::cout << "---wait connect---" << std::endl;

	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	// クライアントからのconnect()を受けて、コネクション確立済みのソケット作成
	clientSock_ = (int)accept(listenSock_, (struct sockaddr*)&clientAddr, &addrlen);
	if (clientSock_ < 0) {
		// エラーコードを出力
		std::cout << "Error: accept( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
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
	listenSock_ = (int)socket(AF_INET, SOCK_STREAM, 0);	// 0で自動設定
	//リスンソケット作成失敗
	if (listenSock_ < 0)
	{
		//エラーコードを出力
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//終了
		return false;
	}
	std::cout << "Success: socket()" << std::endl;

	//bind
	struct sockaddr_in bindAddr;	// bind用のソケットアドレス情報
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(p);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenSock_, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		//エラーコードを出力
		std::cout << "Error: bind( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//終了
		return false;
	}
	std::cout << "Success: bind()" << std::endl;

	//リスン状態に設定	キューのサイズ:1
	if (listen(listenSock_, 1) != 0) {
		//エラーコードを出力
		std::cout << "Error: listen( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//終了
		return false;
	}
	std::cout << "Success: listen()" << std::endl;

	std::cout << "---wait connect---" << std::endl;

	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	//クライアントとの接続ソケットを作成
	clientSock_ = (int)accept(listenSock_, (struct sockaddr*)&clientAddr, &addrlen);
	if (clientSock_ < 0) {
		//エラーコードを出力
		std::cout << "Error: accept( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		//終了
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
		// 相手の入力
		std::system("cls");
		ox.Print();
		IsSuccess = Recv(&value);
		if (!IsSuccess)	break;
		ox.UpdateOtherSide(value);
		if (ox.IsFinish())	break;

		// 自分の入力
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
	int sendValue = htonl(value);	// 送信データ ... ネットワークバイトオーダーに変換後の値を格納
	int ret;						// 成否判定
	//送信
	ret = send(clientSock_, (char*)&sendValue, sizeof(sendValue), 0);
	//失敗
	if (ret != sizeof(sendValue)) {
		return false;
	}

	//成功
	return true;
}

bool Server::Recv(unsigned int* value) {
	int recvValue = 0;	// 受信データの格納領域...ネットワークバイトオーダー状態
	int ret;			// 成否判定

	//受信
	ret = recv(clientSock_, (char*)&recvValue, sizeof(recvValue), 0);
	//失敗
	if (ret != sizeof(recvValue)) {
		return false;
	}

	//成功時は受信データをバイトオーダーに変換
	*value = ntohl(recvValue);
	return true;
}

bool Server::Release() {
	//成否判定変数
	int result = 0;

	//送受信ともに切断
	//shutdown(sock, 0x02);
	if (shutdown(clientSock_, SD_BOTH) != 0) {
		// エラーコードを出力
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;


	//ソケットの破棄
	if (closesocket(clientSock_) != 0) {
		//エラーコードを出力
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	if (closesocket(listenSock_) != 0) {
		//エラーコードを出力
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	//WinSock終了処理
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << result << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;

	return false;
}
