#pragma once

class Server {
private:
	int listenSock_;
	int clientSock_;

public:
	Server();
	~Server();

	//初期化
	bool Initialize();
	bool Initialize(unsigned short p);

	//ゲーム開始
	void PlayGame();

	//送信
	bool Send(unsigned int value);
	
	//受信
	bool Recv(unsigned int* value);

	//解放
	bool Release();
};
