// 编译: x86_64-w64-mingw32-g++ ETS-Leaker.cpp -o ETS-Leaker.exe -static
// g++ ETS-Leaker.cpp lib\include\jsoncpp\jsoncpp.cpp lib\include\jsoncpp\json\json.h lib\include\nlohmann\json.hpp -o ETS-Leaker.exe -std=c++17
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <regex>
#include <filesystem>
#include <io.h>
#include "lib/include/nlohmann/json.hpp"
#include <cstring>
#include <cctype>
#include "lib/include/jsoncpp/json/json.h"

using namespace std;
using json = nlohmann::json;
using namespace std::filesystem;
using namespace Json;

void usage() {
    SetConsoleOutputCP(65001);
    cout << "功能: " << endl;
    cout << "0. 退出程序" << endl;
    cout << "1. 初始化(自动获取E听说目录)" << endl;
    cout << "2. 获取答案并输出" << endl;
}

string GetDirs(string p) {
    SetConsoleOutputCP(65001);
    const string path = p + "content_*";
    // 获取第一个文件夹
    struct _finddata_t fileInfo;
    long handle = _findfirst(path.c_str(), &fileInfo);
    if (handle == -1) {
        cout << "没有找到文件夹" << endl;
        _findclose(handle);
        return "DirNotFound";
    }

    // 抄AI.cpp (^^;
    do {
        // 1. 检查是否是文件夹
        if (fileInfo.attrib & _A_SUBDIR) {
            // 2. 排除 "." 和 ".."
            if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0) {
                continue; // 跳过本次循环
            }

            // 3. 检查文件夹名是否符合 "content_数字" 的格式
            bool isValid = true;
            // 3.1 检查是否以 "content_" 开头
            if (strncmp(fileInfo.name, "content_", 8) != 0) {
                isValid = false;
            } else {
                // 3.2 检查 "content_" 后面的字符是否全是数字
                const char* suffix = fileInfo.name + 8; // 跳过前8个字符（"content_"）
                for (int i = 0; suffix[i] != '\0'; i++) {
                    if (!isdigit(suffix[i])) { // 如果不是数字
                        isValid = false;
                        break;
                    }
                }
            }

            // 4. 输出符合条件的文件夹
            if (isValid) {
                cout << "找到第一个目标文件夹: " << fileInfo.name << endl;
                const char* numStr = fileInfo.name + 8;
                _findclose(handle);
                return string(numStr);
            }
        }
    } while (_findnext(handle, &fileInfo) == 0);
    _findclose(handle);
    return "UKERROR";
}

int ReadAndWrite(string FillPath) {
    SetConsoleOutputCP(65001);
    // 抄 https://blog.csdn.net/shuiyixin/article/details/89330529
    Reader reader;
    Value root;

    // 读文件
    ifstream in(FillPath, ios::binary);
    if (!in.is_open()) {
        // cout << "打开文件失败" << endl;
        return -1;
    }

    if (!reader.parse(in, root)) {
        // cout << "解析文件失败" << endl;
        in.close();
        return -2;
    }

    // 读取节点信息
    Value question = root["info"]["question"];
    int l = question.size();
    for (int i = 0; i < l; i++) {
        cout << "第" << i + 1 << "道题: " << question[i]["question"] << endl;
        cout << "答案: " << question[i]["std"][0]["value"] << endl;
    }
    // 关闭文件
    in.close();
    return 0;
}

int main() {
    SetConsoleOutputCP(65001);
    system("title ETS-Leaker v0.0.1-alpha.0  ---by Yuuzy0721");
    cout << "ETS-Leaker v0.0.1-alpha.0  ---by Yuuzy0721" << endl;
    cout << "一个用脚写的E听说答案获取工具" << endl;
    cout << endl;
    cout << "------------------------------------------------" << endl;
    while (true) {
        int choice;
        string UserName;
        string ETSdir = "null";
        string ETSdir2;
        usage();
        cin >> choice;
        if (choice == 0) {
            // 弹出弹窗
            MessageBoxW(NULL, L"程序结束", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
            // 输出结束消息到控制台
            cout << "程序结束" << endl;
            // 跳出循环
            break;
        }
        else if (choice == 1) {
            /**
             * goto标签在这里！
             * goto标签在这里！
             * goto标签在这里！
             */
            INITDIR: cout << "正在初始化..." << endl;   // goto标签在这里！
            cout << "正在尝试获取E听说目录..." << endl;
            cout << "请输入你的电脑用户名: ";
            cin >> UserName;
            cout << endl;
            // 拼接路径
            ETSdir = "C:\\Users\\" + UserName + "\\AppData\\Roaming\\ETS";
            // 检查目录是否存在
            if (GetFileAttributesA(ETSdir.c_str()) == INVALID_FILE_ATTRIBUTES) {
                cout << "获取目录失败，请检查目录是否存在" << endl;
                cout << "请手动输入E听说目录(\\需替换为\\\\): ";
                cin >> ETSdir;
                cout << endl;
                if (GetFileAttributesA(ETSdir.c_str()) == INVALID_FILE_ATTRIBUTES) {
                    // FUCK
                    MessageBoxW(NULL, L"目录都不会输", L"臭傻逼", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
                    cout << "sb,目录都不会输" << endl;
                    break;
                }
            }
            else {
                cout << "获取目录成功" << endl;
                // 输出目录
                cout << "E听说目录: " << ETSdir << endl;
                cout << "即将前往获取答案" << endl;
                system("pause");
                goto GETDAAN;
            }
        }
        else if (choice == 2) {
            // goto标签在这里！
            // goto标签在这里！
            // goto标签在这里！
            GETDAAN:
            if (ETSdir == "null") {
                MessageBoxW(NULL, L"请先初始化", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
                cout << "请先初始化" << endl;
                goto INITDIR;
            }
            system(("dir " + ETSdir + "\\").c_str());
            cout << "请输入试题的文件夹名(时间越新就代表是越近下载的试题)(试题为纯数字的文件夹)";
            // goto标签在这里！
            // goto标签在这里！
            // goto标签在这里！
            GETTESTDIR: cin >> ETSdir2;
            cout << endl;
            // 正则输入
            regex patteng("^[0-9]+$");
            if (!regex_match(ETSdir2, patteng)) {
                cout << "请输入正确的文件夹名" << endl;
                goto GETTESTDIR;
            }
            // 检查目录
            string TestDIR = ETSdir + "\\" + ETSdir2 + "\\";
            /*filesystem::path TestDIR_p = filesystem::path(ETSdir) / ETSdir2;
            string TestDIR = TestDIR_p.string();*/
            if (GetFileAttributesA(TestDIR.c_str()) == INVALID_FILE_ATTRIBUTES) {
                cout << "获取目录失败，请检查目录是否存在" << endl;
                MessageBoxW(NULL, L"获取目录失败，请检查目录是否存在", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
                cout << "请再次输入正确文件夹名: ";
                goto GETTESTDIR;
            }
            cout << endl;
            cout << "正在尝试获取答案..." << endl;
            // 读取目录
            string res = GetDirs(TestDIR);
            int EorN;
            if (res == "DirNotFound") {
                cout << "没有找到文件夹" << endl;
                MessageBoxW(NULL, L"没有找到文件夹", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
                cout << "0. 退出程序" << endl;
                cout << "1. 重新启动" << endl;
                CHOICE: cout << "请选择: ";
                cin >> EorN;
                if (EorN == 0) {
                    cout << endl;
                    cout << "程序结束" << endl;
                    break;
                } else if (EorN == 1) {
                    cout << endl;
                    system("cls");
                    continue;
                } else {
                    cout <<endl;
                    cout << "输入错误" << endl;
                    goto CHOICE;
                }
            } else if (res == "UKERROR") {
                cout << "未知错误" << endl;
                MessageBoxW(NULL, L"未知错误", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
                break;
            } else {
                cout << "获取答案文件夹成功" << endl;
                cout << "尝试获取并保存答案..." << endl;
                int ctxNum = stoi(res);
                int HowMany;
                cout << "试题内有几个题目？(听选信息2道为一题，回答问题2道一题，完整试卷共7题)(如果你不想让程序出奇奇怪怪的问题就好好填，我已经不想写防小人了): ";
                cin >> HowMany;
                cout << endl;
                for (int i = 0; i < HowMany; i++){
                    // 拼接路径
                    string ETSdir3 = TestDIR + "\\content_" + to_string(ctxNum) + "\\content.json";
                    // 检查文件是否存在
                    if (GetFileAttributesA(ETSdir3.c_str()) == INVALID_FILE_ATTRIBUTES) {
                        cout << "获取文件失败，请检查文件是否存在" << endl;
                        MessageBoxW(NULL, L"获取文件失败，请检查文件是否存在", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
                        break;
                    }
                    // 处理答案
                }
            }
            MessageBoxW(NULL, L"测试结束", L"ETS-Leaker", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
            cout << "测试结束" << endl;
            system("pause");
            break;
        }
    }
    cout << endl;
    cout << "程序将在3秒后自动关闭" << endl;
    Sleep(1000 * 3);
    return 0;
}