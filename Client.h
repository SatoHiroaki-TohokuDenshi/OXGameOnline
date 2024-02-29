#pragma once

class Client {
private:
	int socket_;

public:
	Client();
	~Client();

	//初期化
	bool Initialize();
	bool Initialize(unsigned short p);

	//ゲーム開始
	void PlayGame();

	//解放
	bool Release();

private:
	//送信
	bool Send(unsigned int value);

	//受信
	bool Recv(unsigned int* value);
};
