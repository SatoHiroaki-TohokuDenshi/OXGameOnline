#pragma once

class Server {
private:
	int listenSock_;
	int clientSock_;

public:
	Server();
	~Server();

	//������
	bool Initialize();
	bool Initialize(unsigned short p);

	//�Q�[���J�n
	void PlayGame();

	//���M
	bool Send(int sock, unsigned int value);
	
	//��M
	bool Recv(int sock, unsigned int* value);

	//���
	bool Release();
};
