#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>  // inet_pton�� ���� �߰�
#pragma comment(lib, "ws2_32.lib")  // Winsock ���̺귯�� ��ũ

#define SERVER_IP "127.0.0.1" // ���� IP �ּ� (���� �׽�Ʈ��)
#define PORT 12345
#define BUFFER_SIZE 4096

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE] = { 0 };

    // Winsock �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsock �ʱ�ȭ ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        return 1;//
    }

    // ���� ����
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "���� ���� ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // ���� �ּ� ����
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // IP �ּҸ� ��ȯ�Ͽ� ���� �ּҿ� ����
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        std::cerr << "IP �ּ� ��ȯ ����" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ������ ���� ��û
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "���� ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ������ ���� ����
    std::ifstream infile("send_image.jpg", std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "���� ���� ����" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    int bytes_read = 0;
    // ���� �����͸� �а� ������ ����
    while (!infile.eof()) {
        infile.read(buffer, BUFFER_SIZE);
        bytes_read = infile.gcount();
        send(sock, buffer, bytes_read, 0);
    }

    std::cout << "�̹��� ���� �Ϸ�" << std::endl;

    infile.close();
    closesocket(sock);
    WSACleanup();

    return 0;
}
