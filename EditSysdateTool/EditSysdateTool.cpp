#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

void setSystemTime(int year, int month, int day) {
    time_t rawtime;
    struct tm timeinfo;

    time(&rawtime);

#ifdef _WIN32
    localtime_s(&timeinfo, &rawtime); // 使用 localtime_s 替代 localtime
#else
    localtime_r(&rawtime, &timeinfo); // 使用 localtime_r 替代 localtime
#endif

    timeinfo.tm_year = year - 1900; // tm_year is the number of years since 1900
    timeinfo.tm_mon = month - 1;    // tm_mon is 0-based (0 = January)
    timeinfo.tm_mday = day;

    time_t newTime = mktime(&timeinfo);

#ifdef _WIN32
    SYSTEMTIME st;
    struct tm newTimeinfo;

    localtime_s(&newTimeinfo, &newTime); // 使用 localtime_s 替代 localtime

    st.wYear = newTimeinfo.tm_year + 1900;
    st.wMonth = newTimeinfo.tm_mon + 1;
    st.wDay = newTimeinfo.tm_mday;
    st.wHour = newTimeinfo.tm_hour;
    st.wMinute = newTimeinfo.tm_min;
    st.wSecond = newTimeinfo.tm_sec;
    st.wMilliseconds = 0;

    if (!SetSystemTime(&st)) {
        std::cerr << "Failed to set system time." << std::endl;
    }
#else
    struct timeval tv;
    struct tm newTimeinfo;

    localtime_r(&newTime, &newTimeinfo); // 使用 localtime_r 替代 localtime

    tv.tv_sec = newTime;
    tv.tv_usec = 0;

    if (settimeofday(&tv, NULL) != 0) {
        std::cerr << "Failed to set system time." << std::endl;
    }
#endif
}

bool createDefaultConfigFile(const std::string& filePath) {
    std::ofstream configFile(filePath);
    if (!configFile.is_open()) {
        std::cerr << "Failed to create config file." << std::endl;
        return false;
    }

    configFile << "year=2011\n";
    configFile << "month=1\n";
    configFile << "day=1\n";

    configFile.close();
    return true;
}

bool readConfigFile(const std::string& filePath, int& year, int& month, int& day) {
    std::ifstream configFile(filePath);
    if (!configFile.is_open()) {
        std::cerr << "Config file does not exist. Creating default config file." << std::endl;
        if (!createDefaultConfigFile(filePath)) {
            return false;
        }
        configFile.open(filePath);
        if (!configFile.is_open()) {
            std::cerr << "Failed to open config file after creating it." << std::endl;
            return false;
        }
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                if (key == "year") {
                    year = std::stoi(value);
                }
                else if (key == "month") {
                    month = std::stoi(value);
                }
                else if (key == "day") {
                    day = std::stoi(value);
                }
            }
        }
    }

    configFile.close();
    return true;
}

int main() {
    const std::string configFilePath = "config.txt";
    int year, month, day;

    if (readConfigFile(configFilePath, year, month, day)) {
        setSystemTime(year, month, day);
        std::cout << "System time changed to " << year << "-" << month << "-" << day << "." << std::endl;
    }
    else {
        std::cerr << "请创建config.txt文件，内容例如：" << std::endl;
        std::cerr << "year=2011" << std::endl;
        std::cerr << "month=1" << std::endl;
        std::cerr << "day=1" << std::endl;
        system("pause");
    }

    return 0;
}

 
 

 