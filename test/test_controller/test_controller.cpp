//
// Created by sy200 on 24-12-19.
//
extern "C" {
#include "train_controller.h"
#include "train.h"
#include "utils.h"
}
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

void printTrain(TrainData &p){
    std::wstring status;
    switch (p.train_status) {
        case TRAIN_STATUS_NORMAL:
            status=L"正点";
            break;
        case TRAIN_STATUS_CANCELLED:
            status=L"取消";
            break;
        case TRAIN_STATUS_DELAYED:
            status=L"晚点";
            break;
        case TRAIN_STATUS_STOPPED:
            status=L"停运";
            break;
        case TRAIN_STATUS_UNKNOWN:
            status=L"未知状态";
            break;
        default:
            status=L"错误";
    }
    wchar_t stime[1024]={0};
    wchar_t atime[1024]={0};
    wchar_t number[5]={0};
    tm *stime1=localtime(&p.start_time);
    tm stime2;
    size_t err=wcsftime(stime, 1024, L"%H:%M:%S, %J %B %Y", localtime(&p.start_time));
    wcsftime(atime, 1024, L"%H:%M:%S, %J %m %Y", localtime(&p.arrive_time));
    wchar_t start_station[32];
    wchar_t arrive_station[32];
    size_t ret[32];
    mbstowcs_s(ret, start_station,64, p.start_station,31);
    mbstowcs_s(ret, arrive_station,64, p.arrive_station,31);
    mbstowcs_s(ret, number,5, p.number,31);
    wprintf(L"%3d %4ls %4ls %4ls %ls %ls %4.2f %4d %ls\n",
            p.id, number, start_station, arrive_station,
            stime, atime, ((float)p.ticket_price)/100, p.ticket_remain, status.c_str());
    fflush(stdout);
}
int main(){
    initConfig();
    initDb();

    TrainData data;

    std::srand((unsigned int)time(NULL));
    int r=rand();
    sprintf(data.number, "%x", r);
    data.start_time=3600*24+time(NULL);
    data.arrive_time=3600*24*2+time(NULL);
    r=rand();
    sprintf(data.start_station, "%x", r);
    r=rand();
    sprintf(data.arrive_station, "%x", r);
    r=rand()%1000;
    data.ticket_remain=r;
    r=rand()%114514;
    data.ticket_price=r;
    data.train_status=rand()%4;

    RailwaySystemInsertTrainData(data);

    TrainData queryData[10];

    TrainQuery query;
    memset(&query, 0, sizeof(query));
    query.query_arrive_station=EQUAL;
    strcpy(query.arrive_station,"59fd");
    SearchResult res=RailwaySystemSearchTrainData(queryData, 10, 1, &query);
    for(int i=0; i<res.data_return_count;i++){
        printTrain(queryData[i]);
    }
    queryData[0].arrive_time=data.arrive_time=1145141919810L;
    RailwaySystemUpdateTrainData(queryData[0]);

    RailwaySystemDelTrainData(queryData[1].id);
    finalizeDb();
    return 0;
}