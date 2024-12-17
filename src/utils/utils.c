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

static int callback(void *data, int argc, char **argv, char **azColName) {
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
        ret = sqlite3_exec(db, "create table if not exists train("
                               "id int primary key autoincrement,"
                               "ticket_remain int,"
                               "ticket_price int,"
                               "start_time int,"
                               "arrive_time int,"
                               "number text,"
                               "start_station text,"
                               "arrive_station text,"
                               "is_running int;",
                           callback, NULL, &zErrMsg);
        sqlite3_free(zErrMsg);
        if (ret != 0) {
            return ret;
        }
    }
    return SUCCESS;
}

int finalizeDb() {
    if (db != NULL) {
        return sqlite3_close(db);
    } else {
        return DB_ALREADY_CLOSED;
    }
}


int initConfig() {
    memset(&config, 0, sizeof(Config));
    FILE *fp;
    int ret = SUCCESS;
    if ((fp = fopen(CONFIG_FILE, "r")) == NULL) {
        config = defaultConfig;
        return CONFIG_FILE_BROKEN;
    }
    char buffer[MAX_CONFIG_LINE] = {0};
    char prop_buffer[MAX_PROP] = {0};
    while (fgets(prop_buffer, MAX_PROP, fp) != NULL) {
        if (strcmp(prop_buffer, DB_FILE_CONFIG) == 0) {
            memset(prop_buffer, 0, sizeof(char) * MAX_PROP);
            if (fgets(buffer, MAX_CONFIG_LINE, fp) != NULL) {
                strcpy_s(config.db_path, strlen(buffer), buffer);
                memset(buffer, 0, sizeof(char) * MAX_CONFIG_LINE);
            } else {
                strcpy_s(config.db_path, MAX_DB_FILE_CONFIG, defaultConfig.db_path);
                ret = CONFIG_FILE_BROKEN;
            }
        }
    }
    return ret;
}

#ifdef __cplusplus
}
#endif