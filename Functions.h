#pragma once

// �v���g�^�C�v�錾
void PlayGame(int& sock);					// �Q�[����V��
bool Send(int sock, unsigned int value);	// 
bool Recv(int sock, unsigned int* value);	// 

int Server();
int Client();
