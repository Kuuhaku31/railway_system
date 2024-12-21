//
// Created by sy200 on 24-12-17.
//
#include "utils.h"
#include "error.h"
#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>

sqlite3 *db = NULL;
Config config;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

const Config defaultConfig = {"data.db"};

int initDb() {
    char *zErrMsg = NULL;
    if (db != NULL) {
        return DB_ALREADY_OPENED;
    } else {
        int ret = sqlite3_open(config.db_path, &db);
        if (ret != 0) {
            return ret;
        }
        ret = sqlite3_exec(db, "create table if not exists trains("
                               "id integer primary key autoincrement,"
                               "ticket_remain integer,"
                               "ticket_price integer,"
                               "start_time integer,"
                               "arrive_time integer,"
                               "number text unique,"
                               "start_station text,"
                               "arrive_station text,"
                               "is_running integer);",
                           callback, NULL, &zErrMsg);

        if (ret != 0) {
            printf("\033[31m%s",zErrMsg);
            sqlite3_free(zErrMsg);
            return ret;
        }
    }
    sqlite3_free(zErrMsg);
    return SUCCESS;
}

int finalizeDb() {
    if (db != NULL) {
        return sqlite3_close(db);
    } else {
        return DB_ALREADY_CLOSED;
    }
}

/**
 * 加载配置文件
 * @return 错误码
 */
int initConfig() {
    //全设为0，防止忘记字符串结尾的'\0'。
    memset(&config, 0, sizeof(Config));
    FILE *fp;
    int ret = SUCCESS;
    //打开文件
    if ((fp = fopen(CONFIG_FILE, "r")) == NULL) {
        //打开失败则加载默认配置
        config = defaultConfig;
        return CONFIG_FILE_BROKEN;
    }
    char buffer[MAX_CONFIG_LINE] = {0};
    char prop_buffer[MAX_PROP] = {0};
    //逐行读取文件
    while (fgets(prop_buffer, MAX_PROP, fp) != NULL) {
        //首先，把行尾的换行符去掉
        for(int i = 0; prop_buffer[i]!='\0'; i++)
        {
            if (prop_buffer[i] == '\n')
            {
                prop_buffer[i] = '\0';
                break;
            }
        }
        //匹配数据库名称的配置项
        if (strcmp(prop_buffer, DB_FILE_CONFIG) == 0) {
            memset(prop_buffer, 0, sizeof(char) * MAX_PROP);
            //匹配上了，读取配置
            if (fgets(buffer, MAX_CONFIG_LINE, fp) != NULL) {
                //干掉换行符
                for(int i = 0; buffer[i]!='\0'; i++)
                {
                    if (buffer[i] == '\n')
                    {
                        buffer[i] = '\0';
                        break;
                    }
                }
                //全设为零，备用————虽然目前就这一个配置，不存咋备用
                strcpy_s(config.db_path, MAX_DB_FILE_CONFIG, buffer);
                memset(buffer, 0, sizeof(char) * MAX_CONFIG_LINE);
            } else {
                //读取失败的话，说明配置文件损坏，为这一项加载默认配置
                strcpy_s(config.db_path, MAX_DB_FILE_CONFIG, defaultConfig.db_path);
                ret = CONFIG_FILE_BROKEN;
            }
        }
    }
    fclose(fp);
    return ret;
}

#ifdef __cplusplus
}
#endif