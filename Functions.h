#pragma once

// �v���g�^�C�v�錾
void PlayServer(int& sock);					// �Q�[����V��
void PlayClient(int& sock);					// �Q�[����V��
bool Send(int sock, unsigned int value);	// 
bool Recv(int sock, unsigned int* value);	// 

int Server();
int Client();
