#include <stdio.h>
#include "sqlite3.h"

//有表就開表，沒表就建表
static char *createsql = "CREATE TABLE Weekly("
                "Year INTEGER,"
                "Month INTEGER,"
                "Day INTEGER,"
                "Day_of_week INTEGER,"
                "Activity TEXT,"
                "Frequency INTEGER,"
                "Now_Frequency INTEGER);";

//新增資料
static char *insertsql = "INSERT INTO Weekly VALUES(year, month, day, day_of_week, "
                        "&activity, frequency, now_frequency);";
// static char *insertsql = "INSERT INTO Weekly VALUES(2024,11,18,4,'Calculus',30,10);";

//刪除資料
static char *deletesql = "DELETE FROM Weekly WHERE Year = year AND Month = month AND Day = day AND "
                        "Day_of_week = day_of_week AND Activity = &activity AND "
                        "Frequency = frequency AND Now_Frequency = now_frequency";
// static char *deletesql = "DELETE FROM Weekly WHERE Year = 0 AND Month = 0 AND Day = 0 AND "
//                         "Day_of_week = 0 AND Activity = 'AAA' AND "
//                         "Frequency = 0 AND Now_Frequency = 0;";

//查詢資料
static char *searchsql = "SELECT * FROM Weekly WHERE Year = year AND Month = month AND Day = day;";
//static char *searchsql = "SELECT * FROM Weekly WHERE Year = 0 AND Month = 0 AND Day = 0;";

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
   int rows, cols, i, j, action=1;
   sqlite3 *db;
   char *errMsg = NULL;
   char **result;

   /* 開啟 database 檔 */
   if (sqlite3_open_v2("Weekly.db3", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
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
  sqlite3_get_table(db, searchsql, &result , &rows, &cols, &errMsg);

   /* 列出所有資料 */
    for (i=0;i<rows;i++) {
       for (j=0;j<cols;j++) {
           printf("%s\t", result[i*cols+j]);
       }
       printf("\n");
    }

    

   /* 釋放 */
   sqlite3_free_table(result);

   /* 關閉 database */
   sqlite3_close(db);
}