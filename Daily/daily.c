#include <stdio.h>
#include "sqlite3.h"

//有表就開表，沒表就建表
static char *createsql = "CREATE TABLE Calendar("
                "Year INTEGER,"
                "Month INTEGER,"
                "Day INTEGER,"
                "Begin_Hour INTEGER,"
                "Begin_Minute INTEGER,"
                "End_Hour INTEGER,"
                "End_Minute INTEGER,"
                "Activity TEXT);";

//新增資料
static char *insertsql = "INSERT INTO Calendar VALUES(year, month, day, "
                        "begin_hour, begin_minute, end_hour, end_minute, &activity);";
//static char *insertsql = "INSERT INTO Calendar VALUES(0,0,0,0,0,0,0,'AAA');";

//刪除資料
static char *deletesql = "DELETE FROM Calendar WHERE Year = year AND Month = month AND Day = day AND "
                        "Begin_Hour = begin_hour AND Begin_Minute = begin_minute AND "
                        "End_Hour = end_hour AND End_Minute = end_minute AND Activity = &activity;";
// static char *deletesql = "DELETE FROM Calendar WHERE Year = 0 AND Month = 0 AND Day = 0 AND "
//                         "Begin_Hour = 0 AND Begin_Minute = 0 AND "
//                         "End_Hour = 0 AND End_Minute = 0 AND Activity = 'AAA';";

//查詢資料
static char *searchsql = "SELECT * FROM Calendar WHERE Year = year AND Month = month AND Day = day;";
//static char *searchsql = "SELECT * FROM Calendar WHERE Year = 0 AND Month = 0 AND Day = 0;";

// 用來顯示查詢結果的回呼函數
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s\t", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


void main(void)
{
   int rows, cols, i, j, action;
   sqlite3 *db;
   char *errMsg = NULL;
   char **result;

   /* 開啟 database 檔 */
   if (sqlite3_open_v2("daily.db3", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
       return;
   }

   /* 建立 Table */
   sqlite3_exec(db, createsql, 0, 0, &errMsg);
    
    if(action == 1){
        /* 新增一筆資料 */
        sqlite3_exec(db, insertsql, 0, 0, &errMsg);
    }
    else if(action == 2){
        /*刪除一筆資料*/
        sqlite3_exec(db, deletesql, 0, 0, &errMsg);
    }
    else if(action == 3){
        /*查詢一筆資料*/
        sqlite3_exec(db, searchsql, callback, 0, &errMsg);
        
    }
    else if(action == 4){
        /*修改一筆資料(先刪除再新增)*/
        sqlite3_exec(db, deletesql, 0, 0, &errMsg);
        sqlite3_exec(db, insertsql, 0, 0, &errMsg);
    }
   
   /* 取得該筆資料的 ID */
   //printf("%d\n", sqlite3_last_insert_rowid(db));

   /* 取得 database 裡所有的資料 */
   //sqlite3_get_table(db, searchsql, &result , &rows, &cols, &errMsg);

   /* 列出所有資料 */
//    for (i=0;i<rows;i++) {
//        for (j=0;j<cols;j++) {
//            printf("%s\t", result[i*cols+j]);
//        }
//        printf("\n");
//    }

    

   /* 釋放 */
   sqlite3_free_table(result);

   /* 關閉 database */
   sqlite3_close(db);
}
