#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>  // inet_pton을 위해 추가
#pragma comment(lib, "ws2_32.lib")  // Winsock 라이브러리 링크

#define PORT 12345
#define BUFFER_SIZE 4096

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in server, client;
    int client_len, recv_size;
    char buffer[BUFFER_SIZE] = { 0 };

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsock 초기화 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 주소 구조체 설정
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;  // 모든 IP에서 접속 허용

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "바인딩 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 연결 대기 상태로 전환
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "대기 상태 전환 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "서버가 대기 중..." << std::endl;

    // 클라이언트 연결 수락
    client_len = sizeof(struct sockaddr_in);
    client_socket = accept(server_fd, (struct sockaddr*)&client, &client_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "연결 수락 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 파일을 받아 저장
    std::ofstream outfile("received_image.jpg", std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "파일 열기 실패" << std::endl;
        closesocket(client_socket);
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 클라이언트로부터 데이터 수신
    while ((recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        outfile.write(buffer, recv_size);
    }

    std::cout << "이미지 수신 완료" << std::endl;

    outfile.close();
    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
