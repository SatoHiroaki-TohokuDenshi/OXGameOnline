#pragma once

// プロトタイプ宣言
void PlayGame(int& sock);					// ゲームを遊ぶ
bool Send(int sock, unsigned int value);	// 
bool Recv(int sock, unsigned int* value);	// 

int Server();
int Client();
