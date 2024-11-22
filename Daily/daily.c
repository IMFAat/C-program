#include <stdio.h>
#include "sqlite3.h"

static char *createsql = "CREATE TABLE Calendar("
                "Year INTEGER,"
                "Month INTEGER,"
                "Day INTEGER,"
                "Begin_Hour INTEGER,"
                "Begin_Minute INTEGER,"
                "End_Hour INTEGER,"
                "End_Minute INTEGER,"
                "Activity TEXT);";

/*static char *insertsql = "INSERT INTO Calendar VALUES(year, month, day, "
                            "begin_hour, begin_minute, end_hour, end_minute, activity);";
                            */

static char *insertsql = "INSERT INTO Calendar VALUES(0,0,0,0,0,0,0,'AAA');";
static char *querysql = "SELECT * FROM Calendar;";

void main(void)
{
   int rows, cols, i, j;
   sqlite3 *db;
   char *errMsg = NULL;
   char **result;

   /* 開啟 database 檔 */
   if (sqlite3_open_v2("daily.db3", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
       return;
   }

   /* 建立 Table */
   sqlite3_exec(db, createsql, 0, 0, &errMsg);

   /* 新增一筆資料 */
   sqlite3_exec(db, insertsql, 0, 0, &errMsg);
   /* 取得該筆資料的 ID */
   printf("%d\n", sqlite3_last_insert_rowid(db));

   /* 取得 database 裡所有的資料 */
   sqlite3_get_table(db , querysql, &result , &rows, &cols, &errMsg);

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
