#include "Functions.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

#include "OXGame.h"

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;

int Client() {
	std::cout << "Send Int Client" << std::endl;

	// WinSock初期化
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << ret << " )" << std::endl;
		return 1;	// おーわり！
	}
	std::cout << "Success: WSAStartup" << std::endl;

	// ソケットの作成
	int sock;
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		// エラーコードを出力
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return 1;
	}
	std::cout << "Success: socket()" << std::endl;

	// 接続先サーバのソケットアドレス情報設定
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	std::string SA;
	std::cout << "Input Server Address : ";
	std::cin >> SA;
	inet_pton(AF_INET, SA.c_str(), &serverAddr.sin_addr.s_addr);	// ほんとはよくない。せめて127.0.0.1を定数化

	// 接続要求
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		// エラーコードを出力
		std::cout << "Error: connect( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return 1;
	}
	std::cout << "Success: connect()" << std::endl;

	PlayClient(sock);

	// 送受信ともに切断
	// shutdown(sock, 0x02);
	if (shutdown(sock, SD_BOTH) != 0) {
		// エラーコードを出力
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;

	// ソケットの破棄
	if (closesocket(sock) != 0) {
		// エラーコードを出力
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	// WinSock終了処理
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << ret << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;
	return 0;
}

int Server() {
	std::cout << "Send Int Server" << std::endl;

	// WinSock初期化
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		std::cout << "Error: WSAStartup ( ErrorCode:" << ret << " )" << std::endl;
		return 1;
	}
	std::cout << "Success: WSAStartup" << std::endl;

	// リスンソケットの作成
	int listenSock;
	listenSock = (int)socket(AF_INET, SOCK_STREAM, 0);	// 0で自動設定
	// リスンソケット作成失敗
	if (listenSock < 0)
	{
		// エラーコードを出力
		std::cout << "Error: socket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return 1;
	}
	std::cout << "Success: socket()" << std::endl;

	// bind
	struct sockaddr_in bindAddr;	// bind用のソケットアドレス情報
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ソケットアドレス情報設定	※固定のポート番号設定
	if (bind(listenSock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0) {
		// エラーコードを出力
		std::cout << "Error: bind( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return 1;
	}
	std::cout << "Success: bind()" << std::endl;

	// リスン状態に設定	キューのサイズ:1
	if (listen(listenSock, 1) != 0) {
		// エラーコードを出力
		std::cout << "Error: listen( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return 1;
	}
	std::cout << "Success: listen()" << std::endl;

	std::cout << "---wait connect---" << std::endl;

	// 通信用ソケット ( クライアントのソケットとこのソケット間にコネクションが確立 )
	int sock;


	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	// クライアントからのconnect()を受けて、コネクション確立済みのソケット作成
	sock = (int)accept(listenSock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0) {
		// エラーコードを出力
		std::cout << "Error: accept( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
		// 終了
		return 1;
	}
	std::cout << "Success: accept() " << std::endl;


	// 送受信部
	PlayServer(sock);

	// 送受信ともに切断
	// shutdown(sock, 0x02);
	if (shutdown(sock, SD_BOTH) != 0) {
		// エラーコードを出力
		std::cout << "Error: shutdown( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: shutdown() " << std::endl;


	// ソケットの破棄
	if (closesocket(sock) != 0) {
		// エラーコードを出力
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	if (closesocket(listenSock) != 0) {
		// エラーコードを出力
		std::cout << "Error: closesocket( ErrorCode: " << WSAGetLastError() << ")" << std::endl;
	}
	std::cout << "Success: closesocket() " << std::endl;

	// WinSock終了処理
	if (WSACleanup() != 0) {
		std::cout << "Error: WSACleanup ( ErrorCode:" << ret << " )" << std::endl;
	}
	std::cout << "Success: WSACleanup" << std::endl;

	return 0;
}

/// <summary>
/// intのデータをバイトオーダーを変換してから送信
/// </summary>
/// <param name="sock">送信に使用するソケットのソケットディスクリプタ</param>
/// <param name="value">送信データ</param>
/// <returns>成功 : true  失敗 : false</returns>
bool Send(int sock, unsigned int value) {
	int sendValue = htonl(value);	// 送信データ ... ネットワークバイトオーダーに変換後の値を格納
	int ret;		// 成否の判定用
	// 送信
	ret = send(sock, (char*)&sendValue, sizeof(sendValue), 0);
	// 失敗
	if (ret != sizeof(sendValue)) {
		return false;
	}

	// 成功
	return true;
}

/// <summary>
/// unsigned intデータを受信、バイトオーダーを変換
/// </summary>
/// <param name="sock">受信に使用するソケットのソケットディスクリプタ</param>
/// <param name="value">受信データの格納領域の先頭アドレス</param>
/// <returns>成功 : true 失敗 : false</returns>
bool Recv(int sock, unsigned int* value)
{
	int recvValue;	// 受信データの格納領域...ネットワークバイトオーダー状態
	int ret;		// 成否の判定用
	// 受信
	ret = recv(sock, (char*)&recvValue, sizeof(recvValue), 0);
	// 失敗
	if (ret != sizeof(recvValue)) {
		return false;
	}

	// 成功時は受信データをバイトオーダーに変換
	*value = ntohl(recvValue);
	return true;
}

void PlayClient(int& sock) {
	OXGame ox(CELLTYPE::CELL_O);
	bool IsSuccess = true;
	unsigned int value;

	while (true) {
		// 自分の入力
		std::system("cls");
		ox.Print();
		value = ox.Input();
		ox.UpdateMySide(value);
		IsSuccess = Send(sock, value);
		if (!IsSuccess)	break;
		if (ox.IsFinish())	break;

		// 相手の入力
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
		// 相手の入力
		std::system("cls");
		ox.Print();
		IsSuccess = Recv(sock, &value);
		if (!IsSuccess)	break;
		ox.UpdateOtherSide(value);
		if (ox.IsFinish())	break;

		// 自分の入力
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
