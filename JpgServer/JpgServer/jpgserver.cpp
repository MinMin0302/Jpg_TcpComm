#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>  // inet_pton�� ���� �߰�
#pragma comment(lib, "ws2_32.lib")  // Winsock ���̺귯�� ��ũ

#define PORT 12345
#define BUFFER_SIZE 4096

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in server, client;
    int client_len, recv_size;
    char buffer[BUFFER_SIZE] = { 0 };

    // Winsock �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsock �ʱ�ȭ ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        return 1;
    }

    // ���� ����
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "���� ���� ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // �ּ� ����ü ����
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;  // ��� IP���� ���� ���

    // ���� ���ε�
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "���ε� ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // ���� ��� ���·� ��ȯ
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "��� ���� ��ȯ ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "������ ��� ��..." << std::endl;

    // Ŭ���̾�Ʈ ���� ����
    client_len = sizeof(struct sockaddr_in);
    client_socket = accept(server_fd, (struct sockaddr*)&client, &client_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "���� ���� ����. ���� �ڵ� : " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // ������ �޾� ����
    std::ofstream outfile("received_image.jpg", std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "���� ���� ����" << std::endl;
        closesocket(client_socket);
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Ŭ���̾�Ʈ�κ��� ������ ����
    while ((recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        outfile.write(buffer, recv_size);
    }

    std::cout << "�̹��� ���� �Ϸ�" << std::endl;

    outfile.close();
    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
