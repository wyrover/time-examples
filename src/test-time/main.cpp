//#define CATCH_CONFIG_RUNNER
//#include <catch/catch.hpp>

#include <gtest/gtest.h>
#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <ATLComTime.h>
#include <stdio.h>
#include <iostream>
#include "fun.h"


TEST(time, GetSystemTime)
{
    SYSTEMTIME stUTC;
    ::GetSystemTime(&stUTC);
    // ��ʾʱ��ļ����
    const int nBufSize = 256;
    TCHAR chBuf[nBufSize];
    wsprintf(chBuf, _T("UTC SYSTEMTIME: %u/%u/%u %u:%u:%u:%u ���� %1d"),
             stUTC.wYear, stUTC.wMonth, stUTC.wDay,
             stUTC.wHour, stUTC.wMinute, stUTC.wSecond,
             stUTC.wMilliseconds, stUTC.wDayOfWeek);
    wprintf(L"%s\n\n", chBuf);
}

TEST(time, GetLocalTime)
{
    const int nBufSize = 256;
    TCHAR chBuf[nBufSize];
    // ��ȡ���ص�ʱ�䡣
    SYSTEMTIME stLocal;
    ::GetLocalTime(&stLocal);
    // ��ʾʱ��ļ����
    wsprintf(chBuf, _T("Local SYSTEMTIME: %u/%u/%u %u:%u:%u:%u %d"),
             stLocal.wYear, stLocal.wMonth, stLocal.wDay,
             stLocal.wHour, stLocal.wMinute, stLocal.wSecond,
             stLocal.wMilliseconds, stLocal.wDayOfWeek);
    wprintf(L"%s\n\n", chBuf);
}



TEST(time, SystemTimeToFileTime)
{
    SYSTEMTIME stNow;
    GetSystemTime(&stNow);
    FILETIME ftNow;
    SystemTimeToFileTime(&stNow, &ftNow);
    ULARGE_INTEGER  uli;
    uli.LowPart = ftNow.dwLowDateTime;
    uli.HighPart = ftNow.dwHighDateTime;
    printf("Local FILETIME: %llu\n", uli.QuadPart);
    FILETIME ftUTC;
    LocalFileTimeToFileTime(&ftNow, &ftUTC);
    uli.LowPart = ftUTC.dwLowDateTime;
    uli.HighPart = ftUTC.dwHighDateTime;
    printf("UTC FILETIME: %llu\n", uli.QuadPart);
    SYSTEMTIME stUTC2;
    FileTimeToSystemTime(&ftUTC, &stUTC2);
    printf("UTC System Time2 (YYYY-MM-DD HH:MM:SS): %d-%d-%d %d:%d:%d\n", stUTC2.wYear, stUTC2.wMonth,
           stUTC2.wDay, stUTC2.wHour, stUTC2.wMinute, stUTC2.wSecond);
}



TEST(time, time_t)
{
    //time_t��long���ͣ���ȷ���룬�ǵ�ǰʱ���1970��1��1�����ʱ��Ĳ�
    const time_t t = time(NULL);
    std::cout << "current time is " << t << std::endl;
    /*����ʱ�䣺���ڣ�ʱ�� �����գ����ڣ�ʱ����*/
    struct tm* current_time = localtime(&t);
    printf("current year is %d;current month is %d;current date of month is %d\r\n",
           1900 + current_time->tm_year,
           1 + current_time->tm_mon/*��month�ķ�ΧΪ0-11*/,
           current_time->tm_mday);
    printf("current day of year is %d;current day in week is %d\r\n",
           current_time->tm_yday,/*��ǰ�����ǽ���ĵڶ�����[0,365] */
           current_time->tm_wday/*days since Sunday - [0,6] */);
    printf("time part %d:%d:%d \r\n",
           current_time->tm_hour,
           current_time->tm_min,
           current_time->tm_sec);
    printf("����ʱ�䣺%d-%d-%d %d:%d:%d\r\n",
           current_time->tm_year + 1900,
           current_time->tm_mon + 1,
           current_time->tm_mday,
           current_time->tm_hour,
           current_time->tm_min,
           current_time->tm_sec);
    /*��������ʱ��*/
    struct tm* current_gmtime = gmtime(&t);
    printf("��������ʱ�䣺%d-%d-%d %d:%d:%d\r\n",
           current_gmtime->tm_year + 1900,
           current_gmtime->tm_mon + 1,
           current_gmtime->tm_mday,
           current_gmtime->tm_hour,
           current_gmtime->tm_min,
           current_gmtime->tm_sec);
}

TEST(COleDateTime, GetCurrentTime)
{
    COleDateTime timeNow;
    timeNow = COleDateTime::GetCurrentTime();
    wprintf(TEXT("current time: %s\n"), (LPCTSTR)timeNow.Format( _T("%Y-%m-%d %H:%M:%S" )));

    // �� 5 ����
    COleDateTimeSpan span(0, 0, 5, 0);
    COleDateTime time2;
    time2 = timeNow + span;
    wprintf(TEXT("current time add 5 min: %s\n"), (LPCTSTR)time2.Format( _T( "%Y-%m-%d %H:%M:%S" )));


    // �� 5 ����
    COleDateTimeSpan span2(0, 0, 0, 5);
    COleDateTime time3;
    time3 = timeNow + span2;
    wprintf(TEXT("current time add 5 seconds: %s\n"), (LPCTSTR)time3.Format( _T( "%Y-%m-%d %H:%M:%S" )));


    COleDateTime timeStart, timeEnd;
    timeStart = COleDateTime::GetCurrentTime();
    ::Sleep(1000);
    timeEnd = COleDateTime::GetCurrentTime();
    COleDateTimeSpan spanElapsed = timeEnd - timeStart;


    
}




int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    system("pause");
    return ret;
}