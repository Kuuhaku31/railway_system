//
// Created by sy200 on 24-12-17.
//

#include <print>
#include "train_model.h"
#include "train.h"
#include <cstring>
#include <ctime>
int main(){

    time_t time1;
    time1=std::time(&time1);
    char *buffer=(char *)calloc(1024, sizeof(char ));
    char num[]="G2222";
    TrainChange change;
    memset(&change,0,sizeof(TrainChange));
    change.ticket_remain=5;
    change.change_ticket_remain=true;
    change.ticket_price=50000;
    change.change_ticket_price=true;
    change.number=num;
    change.change_number=true;
    change.arrive_station=num;
    change.is_running=true;
    change.change_is_running=true;
    analyzeChange(&change, buffer);
    std::string str(buffer);
    std::println("{:}",str);
    time_t time2;
    time2=std::time(&time2);
    return 0;

}