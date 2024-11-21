#include <gtk/gtk.h>
#include "MainScreenFunctions.h"

//主畫面
void MainScreen(GtkBuilder *builder) {
    // 取得 DrawerIconBox 並加上圖示
    GtkBox *drawerIconBox = (GtkBox *) gtk_builder_get_object(builder, "DrawerIconBox");
    GtkWidget *drawerIconImage = gtk_image_new_from_file(ROOT_PATH"/src/Icons/menu.png");
    gtk_image_set_icon_size((GtkImage *) drawerIconImage, 100);
    gtk_box_append(drawerIconBox, drawerIconImage);

    // 取得按鈕
    GtkWidget *drawerButton = (GtkWidget *) gtk_builder_get_object(builder, "DrawerButton");
    GtkWidget *drawerBox = (GtkWidget *) gtk_builder_get_object(builder, "DrawerBox");

    struct BottomBoxAnimationData *dailyAddButtonAnimationData = g_new0(struct BottomBoxAnimationData, 1);
    dailyAddButtonAnimationData->button = (GtkWidget *) gtk_builder_get_object(builder, "DailyAddButton");
    dailyAddButtonAnimationData->backBox = (GtkWidget *) gtk_builder_get_object(builder, "DailyButtonBox");
    dailyAddButtonAnimationData->scrollBox = (GtkWidget *) gtk_builder_get_object(builder, "DailyContentBox");
    dailyAddButtonAnimationData->type = 0;

    struct BottomBoxAnimationData *weeklyAddButtonAnimationData = g_new0(struct BottomBoxAnimationData, 1);
    weeklyAddButtonAnimationData->button = (GtkWidget *) gtk_builder_get_object(builder, "WeeklyAddButton");
    weeklyAddButtonAnimationData->backBox = (GtkWidget *) gtk_builder_get_object(builder, "WeeklyButtonBox");
    weeklyAddButtonAnimationData->scrollBox = (GtkWidget *) gtk_builder_get_object(builder, "WeeklyContentBox");
    weeklyAddButtonAnimationData->type = 1;

    g_signal_connect(dailyAddButtonAnimationData->button, "clicked", G_CALLBACK(AddButtonOnClick),
                     dailyAddButtonAnimationData);
    g_signal_connect(weeklyAddButtonAnimationData->button, "clicked", G_CALLBACK(AddButtonOnClick),
                     weeklyAddButtonAnimationData);

    // 按鈕連接點及觸發器
    g_signal_connect(drawerButton, "clicked", G_CALLBACK(DrawerButtonOnClick), drawerBox);
}

