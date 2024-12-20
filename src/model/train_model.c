//
// Created by sy200 on 24-12-17.
//

#include "train_model.h"
#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        fflush(stdout);
    }
    printf("\n");
    return 0;
}


uint64_t str2u64(char *str){
    uint64_t i;
    sscanf_s(str, "%llu", &i);
    return i;
}
void analyzeCondition(const TrainQuery* condition, char* buffer, int bufferSize){
    buffer[0]=' ';
    char *temp=(char *) calloc(128, sizeof(char ));
    bool first = true;
    char operator[6][3]={">", "<", "<=", ">=", "=", "!="};
    //printf("%d\n", condition->query_ticket_remain);
    if(condition->query_id!=IGNORE_THIS){
        sprintf_s(temp,128, "id%s%d", operator[condition->query_id-1],condition->id);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_ticket_remain!=IGNORE_THIS){
        sprintf_s(temp, 128, "%s ticket_remain%s%d", first?" ":" and ", operator[condition->query_ticket_remain-1],condition->ticket_remain);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_ticket_price!=IGNORE_THIS){
        sprintf_s(temp, 128, "%s ticket_price%s%d", first?" ":" and ", operator[condition->query_ticket_price-1], (int)(condition->ticket_price));
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_start_time!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s start_time%s%lld", first?" ":" and ", operator[condition->query_start_time-1], condition->start_time);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_arrive_time!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s arrive_time%s%lld", first?" ":" and ", operator[condition->query_arrive_time-1], condition->arrive_time);
        first = false;
        strcat_s(buffer, bufferSize, temp );
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_number!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s number like '%%%s%%'", first?" ":" and ", condition->number);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_start_station!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s start_station like '%%%s%%'", first?" ":" and ", condition->start_station);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_arrive_station!=IGNORE_THIS){
        sprintf_s(temp, 128,"%s arrive_station like '%%%s%%'", first?" ":" and ", condition->arrive_station);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(condition->query_train_status!=IGNORE_THIS){
        if(condition->train_status)
        {
            char status_sql[256]={0};
            char one[16]={0};

            bool is_unknown=(condition->train_status&TRAIN_STATUS_UNKNOWN) == TRAIN_STATUS_UNKNOWN ? true : false;
            bool is_normal=(condition->train_status&TRAIN_STATUS_NORMAL) == TRAIN_STATUS_NORMAL ? true : false;
            bool is_delay=(condition->train_status&TRAIN_STATUS_DELAYED) == TRAIN_STATUS_DELAYED ? true : false;
            bool is_stopped=(condition->train_status&TRAIN_STATUS_STOPPED) == TRAIN_STATUS_STOPPED ? true : false;
            bool is_canceled=(condition->train_status&TRAIN_STATUS_CANCELLED) == TRAIN_STATUS_CANCELLED ? true : false;

            bool conditions[5]={is_unknown,is_normal,is_delay,is_stopped,is_canceled};
            int32_t status[5]={TRAIN_STATUS_UNKNOWN,TRAIN_STATUS_NORMAL,TRAIN_STATUS_DELAYED,TRAIN_STATUS_STOPPED,TRAIN_STATUS_CANCELLED};
            bool first2=true;
            for(int i=0;i<5;i++)
            {
                if(conditions[i])
                {
                    if(first2)
                    {
                        sprintf_s(one, 256, "is_running=%d", status[i]);
                        first2=false;
                    }
                    else
                    {
                        sprintf_s(one, 256, " or is_running=%d", status[i]);
                    }
                    strcat_s(status_sql, 256, one);
                    memset(one, 0, 16*sizeof(char));
                }
            }
            sprintf_s(temp, 128, "%s (%s)", first?" ":" and ", status_sql);
        }
        strcat_s(buffer, bufferSize, temp);
    }
    free(temp);
}

void analyzeChange(TrainChange* change, char* buffer, int bufferSize){
    buffer[0]=' ';
    char *temp=(char *) calloc(128, sizeof(char));
    bool first = true;
    if(change->change_id){
        sprintf_s(temp,128, "id=%d", change);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_ticket_remain){
        sprintf_s(temp, 128, "%s ticket_remain=%d", first?" ":", ", change->ticket_remain);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_ticket_price){
        sprintf_s(temp, 128, "%s ticket_price=%d", first?" ":", ",  (int)(change->ticket_price));
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_start_time){
        sprintf_s(temp, 128,"%s start_time=%lld", first?" ":", ",  change->start_time);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_arrive_time){
        sprintf_s(temp, 128,"%s arrive_time=%lld", first?" ":", ",  change->arrive_time);
        first = false;
        strcat_s(buffer, bufferSize, temp );
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_number){
        sprintf_s(temp, 128,"%s number='%s'", first?" ":", ",  change->number);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_start_station){
        sprintf_s(temp, 128,"%s start_station='%s'", first?" ":", ",  change->start_station);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_arrive_station){
        sprintf_s(temp, 128,"%s arrive_station='%s'", first?" ":", ",  change->arrive_station);
        first = false;
        strcat_s(buffer, bufferSize, temp);
        memset(temp, 0, 128*sizeof(char));
    }
    if(change->change_is_running){
        sprintf_s(temp, 128, "%s is_running=%d", first?" ":", ",  change->train_status);
        strcat_s(buffer, bufferSize, temp);
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
                        train->arrive_station, train->train_status);
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
    analyzeCondition(query, condition, 1024);
    char *change_str=(char *) calloc(1024, sizeof(char));
    analyzeChange(change, change_str, 1024);
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
    analyzeCondition(query, condition, 1024);
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
    train->train_status= atoi(resultp[row*col+8]);
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

int getCount(const TrainQuery *condition, uint32_t *count){
    char *sql=(char *) calloc(1024, sizeof(char));
    char *buffer=(char *) calloc(1024, sizeof(char));
    analyzeCondition(condition, buffer, 1024);
    sprintf_s(sql,1024, "select count(*) from trains where %s;", buffer);
    char** resultp=NULL;
    int row=0;
    int col=0;
    char *errMsg=NULL;
    int ret=sqlite3_get_table(db,sql,&resultp,&row, &col, &errMsg);
    free(sql);
    sql=NULL;
    free(buffer);
    buffer=NULL;
    if(ret!=SQLITE_OK){
        printf("\033[31m%s",errMsg);
        sqlite3_free(errMsg);
        errMsg=NULL;
        return ret;
    }
    *count= atoi(resultp[1]);
    return 0;
}
int getTrainList(const TrainQuery *condition, TrainData *train, uint32_t pageSize, uint32_t pageNum, uint32_t *num) {
    char *sql=(char *) calloc(1024, sizeof(char));
    char *buffer=(char *) calloc(1024, sizeof(char));
    analyzeCondition(condition, buffer, 1024);
    sprintf_s(sql,1024, "select * from trains where %s order by id asc limit %u offset %u;", buffer, pageSize, pageSize*(pageNum-1));
    char** resultp=NULL;
    int row=0;
    int col=0;
    char *errMsg=NULL;
    int ret=sqlite3_get_table(db,sql,&resultp,&row, &col, &errMsg);


    free(sql);
    sql=NULL;
    free(buffer);
    buffer=NULL;
    if(ret!=SQLITE_OK){
        printf("\033[31m%s",errMsg);
        sqlite3_free(errMsg);
        errMsg=NULL;
        return ret;
    }
    sqlite3_free(errMsg);
    errMsg=NULL;
    if(row<1){
        *num=0;
        return NO_SUCH_RECORD;
    }
    memset(train, 0, pageSize* sizeof(TrainData));
    for(int i=1;i<row+1;i++){
        setTrainStruct(&train[i-1],resultp, i, col);
    }
    *num=row;
    return SUCCESS;
}
#ifdef __cplusplus
}
#endif