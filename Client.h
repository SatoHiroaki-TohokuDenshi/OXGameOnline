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

	//���
	bool Release();

private:
	//���M
	bool Send(unsigned int value);

	//��M
	bool Recv(unsigned int* value);
};
