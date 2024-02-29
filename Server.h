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
	bool Send(unsigned int value);
	
	//��M
	bool Recv(unsigned int* value);

	//���
	bool Release();
};
