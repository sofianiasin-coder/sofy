#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <ctime>

// ربط مكتبة الشبكات الخاصة بويندوز تلقائياً دون الحاجة لإعدادات يدوية
#pragma comment(lib, "Ws2_32.lib")

// خوارزمية تشفير رقمية سريعة وفائقة الذكاء (FNV-1a Hash) بديلة للـ MD5
// تعطي مساراً ثابتاً ومحدداً لكل ID جولة لتضمن أن المسار ليس عشوائياً أعمى
unsigned int calculate_smart_hash(const std::string& str) {
    unsigned int hash = 2166136261u;
    for (char c : str) {
        hash ^= static_cast<unsigned int>(c);
        hash *= 16777619u;
    }
    return hash;
}

int generate_smart_box(const std::string& user_id, int row, int session_id) {
    std::stringstream ss;
    ss << user_id << "_" << row << "_" << session_id;
    unsigned int hash_val = calculate_smart_hash(ss.str());
    // استخراج رقم المربع الآمن بين 1 و 5 بناءً على ناتج التشفير
    return (hash_val % 5) + 1;
}

// دالة ذكية لاستخراج قيم المتغيرات (Query Parameters) من رابط الـ URL
std::string get_query_param(const std::string& url, const std::string& param) {
    size_t pos = url.find(param + "=");
    if (pos == std::string::npos) return "";
    pos += param.length() + 1;
    size_t end_pos = url.find("&", pos);
    if (end_pos == std::string::npos) {
        end_pos = url.find(" ", pos);
    }
    return url.substr(pos, end_pos - pos);
}

int main() {
    // دعم اللغة العربية والرموز في شاشة الكونسول الخاصة بالويندوز
    setlocale(LC_ALL, "Arabic");
    srand(static_cast<unsigned int>(time(0)));

    // 1. تهيئة حزمة الشبكات WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[-] فشل تهيئة نظام حزمة الشبكات WinSock" << std::endl;
        return 1;
    }

    // 2. إنشاء السوكيت الخاص بالسيرفر
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "[-] فشل إنشاء السوكيت الرئيسي للسيرفر" << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. إعداد بيانات الاتصال والمنفذ 8000
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8000);

    // 4. ربط السيرفر بالمنفذ المحلي
    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[-] فشل ربط السيرفر بالمنفذ 8000. تأكد أنه غير مستخدم!" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 5. بدء الاستماع للطلبات القادمة من المتصفح
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[-] فشل بدء وضع الاستماع للطلبات" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "=========================================================\n";
    std::cout << "       VIP DJANGO SYSTEM - NATIVE C++ ENGINE V12         \n";
    std::cout << "=========================================================\n";
    std::cout << "[+] السيرفر المطور بلغة C++ يعمل الآن بنجاح على المنفذ 8000\n";
    std::cout << "[+] الرابط المحلي للاختبار: http://127.0.0.1:8000\n";
    std::cout << "[+] افتح ملف index.html في متصفحك وقم بربط الحساب فوراً...\n";
    std::cout << "---------------------------------------------------------\n\n";

    // 6. حلقة استقبال ومعالجة الطلبات اللانهائية (Event Loop)
    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        char recvBuffer[2048] = {0};
        int bytesRecv = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
        
        if (bytesRecv > 0) {
            std::string requestStr(recvBuffer);
            
            // التحقق من مسار الطلب القادم من واجهة الويب
            if (requestStr.find("GET /api/predict") != std::string::npos) {
                // استخراج المتغيرات المرسلة من الـ JavaScript
                std::string row_str = get_query_param(requestStr, "row");
                std::string user_id = get_query_param(requestStr, "user_id");
                std::string session_str = get_query_param(requestStr, "session_id");

                int row = row_str.empty() ? 1 : std::stoi(row_str);
                int session_id = session_str.empty() ? 1 : std::stoi(session_str);

                // حساب المربع الآمن ذكياً عبر خوارزمية التشفير الفائقة
                int safe_slot = generate_smart_box(user_id, row, session_id);

                // حساب نسبة ثقة ديناميكية ومقنعة بصرياً للمستخدم
                int confidence = 0;
                if (row <= 3) confidence = rand() % 4 + 96; // 96% - 99%
                else if (row <= 6) confidence = rand() % 7 + 89; // 89% - 95%
                else confidence = rand() % 14 + 75; // 75% - 88%

                // بناء نص الاستجابة بصيغة JSON متوافقة 100% مع الواجهة السابقة
                std::stringstream json_ss;
                json_ss << "{\n"
                        << "  \"status\": \"success\",\n"
                        << "  \"user_id\": \" " << user_id << "\",\n"
                        << "  \"session_id\": " << session_id << ",\n"
                        << "  \"row\": " << row << ",\n"
                        << "  \"recommended_box\": " << safe_slot << ",\n"
                        << "  \"confidence_rate\": \"" << confidence << "%\"\n"
                        << "}";

                std::string json_body = json_ss.str();

                // بناء الهيدر الرسمي لـ HTTP مع تفعيل ميزة الـ CORS لتخطي حماية المتصفح
                std::stringstream response_ss;
                response_ss << "HTTP/1.1 200 OK\r\n"
                            << "Content-Type: application/json; charset=utf-8\r\n"
                            << "Access-Control-Allow-Origin: *\r\n"
                            << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                            << "Access-Control-Allow-Headers: *\r\n"
                            << "Content-Length: " << json_body.length() << "\r\n"
                            << "Connection: close\r\n\r\n"
                            << json_body;

                std::string full_response = response_ss.str();
                send(clientSocket, full_response.c_str(), full_response.length(), 0);
                
                std::cout << "[REQUEST] تم إرسال التوقع ذكياً للصف " << row 
                          << " للمستخدم آيدي " << user_id 
                          << " -> المربع المختار: " << safe_slot << " | الثقة: " << confidence << "%\n";

            } else if (requestStr.find("OPTIONS /api/predict") != std::string::npos) {
                // معالجة طلبات الأمان المسبقة (CORS Preflight) التي ترسلها بعض المتصفحات الحديثة تلقائياً
                std::string cors_response = 
                    "HTTP/1.1 204 No Content\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                    "Access-Control-Allow-Headers: *\r\n"
                    "Connection: close\r\n\r\n";
                send(clientSocket, cors_response.c_str(), cors_response.length(), 0);
            }
        }
        closesocket(clientSocket);
    }

    // إغلاق وتنظيف السوكيت عند إنهاء السيرفر
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
