//
// Created by sy200 on 24-12-17.
//

#ifndef RAILWAYSYSTEM_UTILS_H
#define RAILWAYSYSTEM_UTILS_H
#include "sqlite3.h"
#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>

#define CONFIG_FILE "config.ini"
#define CONFIG_LINES_COUNT 2
#define MAX_PROP 32
#define MAX_CONFIG_LINE 1024
#define DB_FILE_CONFIG "[db_path]"
#define MAX_DB_FILE_CONFIG 128
extern sqlite3* db;
int initDb();
int finalizeDb();
int initConfig();
typedef struct {
    char db_path[MAX_DB_FILE_CONFIG];
}Config;
extern Config config;
extern const Config defaultConfig;

#ifdef __cplusplus
};
#endif

#endif //RAILWAYSYSTEM_UTILS_H
