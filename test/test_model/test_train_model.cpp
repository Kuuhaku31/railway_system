//
// Created by sy200 on 24-12-17.
//

#include <iostream>
#include "train_model.h"
#include <print>
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
    analyzeChange(&change, buffer, 1024);
    memset(buffer, 0, 1024);
    std::string str(buffer);
    std::cout<<str<<std::endl;
    TrainQuery query;
    memset(&query,0,sizeof(TrainChange));
    query.ticket_remain=5;
    query.query_ticket_remain=IGNORE_THIS;
    query.query_is_running=EQUAL;
    query.is_running= true;
    query.query_ticket_remain=GREATER;
    query.ticket_remain=500;
    analyzeCondition(&query, buffer, 1024);
    time_t time2;
    time2=std::time(&time2);
    std::println("{}",buffer);
    std::println("Spent {} seconds.", time2-time1);
    return 0;

}