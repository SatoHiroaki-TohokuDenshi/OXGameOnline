#pragma once

class Client {
private:
	int socket_;

public:
	Client();
	~Client();

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
