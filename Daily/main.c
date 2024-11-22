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

   /* �}�� database �� */
   if (sqlite3_open_v2("daily.db3", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
       return;
   }

   /* �إ� Table */
   sqlite3_exec(db, createsql, 0, 0, &errMsg);

   /* �s�W�@����� */
   sqlite3_exec(db, insertsql, 0, 0, &errMsg);
   /* ���o�ӵ���ƪ� ID */
   printf("%d\n", sqlite3_last_insert_rowid(db));

   /* ���o database �̩Ҧ������ */
   sqlite3_get_table(db , querysql, &result , &rows, &cols, &errMsg);

   /* �C�X�Ҧ���� */
   for (i=0;i<rows;i++) {
       for (j=0;j<cols;j++) {
           printf("%s\t", result[i*cols+j]);
       }
       printf("\n");
   }

   /* ���� */
   sqlite3_free_table(result);

   /* ���� database */
   sqlite3_close(db);
}