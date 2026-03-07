#include <iostream>
#include <ctime>

int
main()
{
    time_t now = time(nullptr);
    struct tm* local_tm = localtime(&now);
    char buffer[128];

    strftime(
        buffer, sizeof(buffer), "%Y-%m-%d %A %H:%M:%S 时区:%Z", local_tm
    );

    std::cout << "Timestamp: " << now << "\n";
    std::cout << "tm: " << local_tm << "\n";
    std::cout << "Date: " << buffer << std::endl;

    // 输出结构体中的具体信息
    std::cout << "年份: " << 1900 + local_tm->tm_year << std::endl;
    std::cout << "月份: " << 1 + local_tm->tm_mon << std::endl;
    std::cout << "日期: " << local_tm->tm_mday << std::endl;
    std::cout << "小时: " << local_tm->tm_hour << std::endl;
    std::cout << "分钟: " << local_tm->tm_min << std::endl;
    std::cout << "秒: " << local_tm->tm_sec << std::endl;
    std::cout << "时区: " << local_tm->tm_zone << std::endl;
    std::cout << "星期: " << local_tm->tm_wday << std::endl;
    return 0;
}