//
// Created by sy200 on 24-12-17.
//

#include "train_model.h"
#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}


uint64_t str2u64(char *str){
    uint64_t i;
    sscanf_s(str, "%llu", &i);
    return i;
}
void analyzeCondition(TrainQuery* condition, char* buffer){
    char *temp=(char *) calloc(128, sizeof(char ));
    bool first = true;
    char operator[6][3]={">", "<", "<=", ">=", "=", "!="};
    if(condition->query_id!=IGNORE_THIS){
        sprintf_s(temp,128, "id%s%d", operator[condition->query_id-1],condition);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_ticket_remain!=IGNORE_THIS){
        sprintf_s(temp, 128, "%s ticket_remain%s%d", first?"":"and", operator[condition->query_id-1],condition->ticket_remain);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_ticket_price!=IGNORE_THIS){
        sprintf_s(temp, 128, "%s ticket_price%s%d", first?"":"and", operator[condition->query_id-1], (int)(condition->ticket_price*100));
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_start_time!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s start_time%s%lld", first?"":"and", operator[condition->query_id-1], condition->start_time);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_arrive_time!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s arrive_time%s%lld", first?"":"and", operator[condition->query_id-1], condition->arrive_time);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp );
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_number!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s number%s'%s'", first?"":"and", operator[condition->query_id-1], condition->number);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_start_station!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s start_station%s'%s'", first?"":"and", operator[condition->query_id-1], condition->start_station);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_arrive_station!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s arrive_station%s'%s'", first?"":"and", operator[condition->query_id-1], condition->arrive_station);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_is_running!=IGNORE_THIS){
        sprintf_s(temp, 128, "%s is_running%s%d", first?"":"and", operator[condition->query_id-1], condition->is_running?1:0);
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
    }
    free(temp);
}

void analyzeChange(TrainChange* change, char* buffer){
    char *temp=(char *) calloc(128, sizeof(char));
    bool first = true;
    if(change->change_id){
        sprintf_s(temp,128, "id=%d", change);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_ticket_remain){
        sprintf_s(temp, 128, "%s ticket_remain=%d", first?"":", ", change->ticket_remain);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_ticket_price){
        sprintf_s(temp, 128, "%s ticket_price=%d", first?"":", ",  (int)(change->ticket_price*100));
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_start_time){
        sprintf_s(temp, 128,"%s start_time=%lld", first?"":", ",  change->start_time);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_arrive_time){
        sprintf_s(temp, 128,"%s arrive_time=%lld", first?"":", ",  change->arrive_time);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp );
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_number){
        sprintf_s(temp, 128,"%s number='%s'", first?"":", ",  change->number);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_start_station){
        sprintf_s(temp, 128,"%s start_station='%s'", first?"":", ",  change->start_station);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_arrive_station){
        sprintf_s(temp, 128,"%s arrive_station='%s'", first?"":", ",  change->arrive_station);
        first = false;
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_is_running){
        sprintf_s(temp, 128, "%s is_running=%d", first?"":", ",  change->is_running?1:0);
        strcat_s(buffer, strlen(buffer)+ strlen(temp), temp);
    }
    free(temp);
}
int addTrain(TrainData* train){
    char *err=NULL;
    char *sql= calloc(1024,sizeof(char));
    sprintf_s(sql, 1024, "insert into trains ("
                        "ticket_remain,ticket_price,start_time,arrive_time,"
                        "number,start_station,arrive_station,is_running"
                        ") values (%d, %d, %lld, %lld, '%s', '%s', '%s', %d);",
                        train->ticket_remain, train->ticket_price,
                        train->start_time, train->arrive_time,
                        train->number, train->start_station,
                        train->arrive_station, train->is_running?1:0);
    int ret=sqlite3_exec(db, sql, callback,0,&err);
    sqlite3_free(err);
    free(sql);
    if(ret!=SQLITE_OK){
        return ret;
    }else{
        return 0;
    }
}

int addTrainList(TrainData* trains, int num){
    char *err=NULL;
    if(sqlite3_exec(db,"begin;",callback,0,&err)==SQLITE_OK){
        sqlite3_free(err);
        err=NULL;
        for(int i=0;i<num;i++){
            int ret=addTrain(&trains[i]);
            if(ret!=SQLITE_OK){
                sqlite3_exec(db,"rollback;",callback,0,&err);
                puts(err);
                sqlite3_free(err);
                err=NULL;
            }
        }
        if(sqlite3_exec(db,"commit;",callback,0,&err)!=SQLITE_OK){
            sqlite3_exec(db,"rollback;",callback,0,&err);
            puts(err);
            sqlite3_free(err);
            err=NULL;
        }
        sqlite3_free(err);
        err=NULL;
        return SUCCESS;
    }
    return UNKNOWN_SQLITE_ERR;
}

int editTrain(TrainQuery * query, TrainChange * change){
    char *condition=(char *) calloc(1024, sizeof(char));
    analyzeCondition(query, condition);
    char *change_str=(char *) calloc(1024, sizeof(char));
    analyzeChange(change, change_str);
    int length=strlen(condition)+ strlen(change_str)+40;
    char *sql=(char *) calloc(length, sizeof(char));
    sprintf_s(sql, length, "update trains set %s where %s;", change_str, condition);
    char *err=NULL;
    int ret=sqlite3_exec(db, sql,  callback, 0, &err);
    free(condition);
    free(change_str);
    free(sql);
    if(ret!=SQLITE_OK){
        puts(err);
        sqlite3_free(err);
        return ret;
    }
    sqlite3_free(err);
    return SUCCESS;
}

int deleteTrain(TrainQuery * query){
    char *condition=(char *) calloc(1024, sizeof(char));
    analyzeCondition(query, condition);
    int length=strlen(condition)+40;
    char *sql=(char *) calloc(length, sizeof(char));
    sprintf_s(sql, length, "delete from trains where %s;", condition);
    char *err=NULL;
    int ret=sqlite3_exec(db, sql,  callback, 0, &err);
    free(condition);
    free(sql);
    if(ret!=SQLITE_OK){
        puts(err);
        sqlite3_free(err);
        return ret;
    }
    sqlite3_free(err);
    return SUCCESS;
}

static void setTrainStruct(TrainData *train, char **resultp, int row, int col){
    memset(train, 0, sizeof(TrainData));
    train->id = atoi(resultp[row*col+0]);
    train->ticket_remain= atoi(resultp[row*col+1]);
    train->ticket_price= atoi(resultp[row*col+2]);
    train->start_time= str2u64(resultp[row*col+3]);
    train->arrive_time= str2u64(resultp[row*col+4]);
    strcpy_s(train->number, 32, resultp[row*col+5]);
    strcpy_s(train->start_station, 64, resultp[row*col+6]);
    strcpy_s(train->arrive_station, 64, resultp[row*col+7]);
    train->is_running= atoi(resultp[row*col+8]);
}

int getTrainById(uint32_t id, TrainData *train) {
    char sql[64]={0};
    sprintf_s(sql,64, "select * from train where id=%d;", id);
    char** resultp=NULL;
    int row=0;
    int col=0;
    char *errMsg=NULL;
    int ret=sqlite3_get_table(db,sql,&resultp,&row, &col, &errMsg);
    if(ret!=SQLITE_OK){
        return ret;
    }
    if(row<1){
        return NO_SUCH_RECORD;
    }
    else if(row > 1){
        return TOO_MANY_RECORDS;
    }
    setTrainStruct(train,resultp, 1, col);
    return SUCCESS;
}

int getTrainByNumber(char *number, TrainData *train) {
    char sql[64]={0};
    sprintf_s(sql,64, "select * from train where number=%s;", number);
    char** resultp=NULL;
    int row=0;
    int col=0;
    char *errMsg=NULL;
    int ret=sqlite3_get_table(db,sql,&resultp,&row, &col, &errMsg);
    if(ret!=SQLITE_OK){
        return ret;
    }
    if(row<1){
        return NO_SUCH_RECORD;
    }
    else if(row > 1){
        return TOO_MANY_RECORDS;
    }
    setTrainStruct(train,resultp, 1, col);
    return SUCCESS;
}

int getTrainList(TrainQuery *condition, TrainData **train, int32_t *num) {
    char *sql=(char *) calloc(1024, sizeof(char));
    char *buffer=(char *) calloc(1024, sizeof(char));
    analyzeCondition(condition, buffer);
    sprintf_s(sql,1024, "select * from train where %s;", buffer);
    char** resultp=NULL;
    int row=0;
    int col=0;
    char *errMsg=NULL;
    int ret=sqlite3_get_table(db,sql,&resultp,&row, &col, &errMsg);
    sqlite3_free(errMsg);
    errMsg=NULL;
    free(sql);
    sql=NULL;
    free(buffer);
    buffer=NULL;
    if(ret!=SQLITE_OK){
        return ret;
    }
    if(row<1){
        return NO_SUCH_RECORD;
    }
    else if(row > 1){
        return TOO_MANY_RECORDS;
    }
    *num=row;
    (*train)=(TrainData *) calloc(row, sizeof(TrainData));
    for(int i=1;i<row+1;i++){
        setTrainStruct(&(*train)[i-1],resultp, i, col);
    }
    return SUCCESS;
}
#ifdef __cplusplus
}
#endif