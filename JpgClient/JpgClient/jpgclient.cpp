#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>  // inet_pton을 위해 추가
#pragma comment(lib, "ws2_32.lib")  // Winsock 라이브러리 링크

#define SERVER_IP "127.0.0.1" // 서버 IP 주소 (로컬 테스트용)
#define PORT 12345
#define BUFFER_SIZE 4096

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE] = { 0 };

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsock 초기화 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        return 1;//
    }

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // IP 주소를 변환하여 서버 주소에 설정
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        std::cerr << "IP 주소 변환 실패" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 서버에 연결 요청
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "연결 실패. 오류 코드 : " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 전송할 파일 열기
    std::ifstream infile("send_image.jpg", std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "파일 열기 실패" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    int bytes_read = 0;
    // 파일 데이터를 읽고 서버에 전송
    while (!infile.eof()) {
        infile.read(buffer, BUFFER_SIZE);
        bytes_read = infile.gcount();
        send(sock, buffer, bytes_read, 0);
    }

    std::cout << "이미지 전송 완료" << std::endl;

    infile.close();
    closesocket(sock);
    WSACleanup();

    return 0;
}
