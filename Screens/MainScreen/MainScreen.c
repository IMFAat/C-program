#include <gtk/gtk.h>
#define ms *1000000

void DrawerButtonOnClick(GtkWidget *button, gpointer data);

void showDrawerContext(gpointer drawer);

//主畫面
void MainScreen(GtkBuilder *builder) {
    // 取得 DrawerIconBox 並加上圖示
    GtkBox *drawerIconBox = (GtkBox *) gtk_builder_get_object(builder, "DrawerIconBox");
    GtkWidget *drawerIconImage = gtk_image_new_from_file(ROOT_PATH"/src/Icons/menu.png");
    gtk_image_set_icon_size((GtkImage *) drawerIconImage, 100);
    gtk_box_append(drawerIconBox, drawerIconImage);

    // 取得按鈕
    GtkWidget *button = (GtkWidget *) gtk_builder_get_object(builder, "DrawerButton");
    GtkWidget *drawerBox = (GtkWidget *) gtk_builder_get_object(builder, "DrawerBox");

    // 按鈕連接點及觸發器
    g_signal_connect(button, "clicked", G_CALLBACK(DrawerButtonOnClick), drawerBox);
}

bool isDrawerOpen = false; // 判斷Drawer是否打開
bool isAnimating = false; // 判斷Drawer動畫是否在進行中
bool isDrawerContextShow = false; // 判斷Drawer的內容是否顯示
GtkWidget *drawerContextScrolledWindow = NULL; // Drawer內容(ScrolledWindow)

// Drawer 動畫
gboolean drawerAnimation(gpointer data);

// Drawer
void DrawerButtonOnClick(GtkWidget *button, gpointer data) {
    // 如果動畫有在進行的話就不重複進行動畫
    if (isAnimating) return;
    isAnimating = true;
    // 將動畫的 function 在主執行緒空閒的時間執行
    g_idle_add(drawerAnimation, data);
}

gboolean drawerAnimation(gpointer data) {
    // 用來判斷動畫是否在進行最後一步
    int step = 0;
    // 整個Drawer
    GtkWidget *drawer = (GtkWidget *) data;

    // 若Drawer內的東西沒有顯示的話就執行
    if (!isDrawerContextShow) {
        // Drawer內容(ScrolledWindow)
        drawerContextScrolledWindow = (GtkWidget *) gtk_scrolled_window_new();
        // Drawer內容的Box
        GtkWidget *drawerContext = (GtkWidget *) gtk_box_new((GtkOrientation) GTK_ORIENTATION_VERTICAL, 10);
        // 將Drawer的Box放入ScrolledWindow
        gtk_scrolled_window_set_child((GtkScrolledWindow *) drawerContextScrolledWindow, drawerContext);
        // ScrolledWindow的橫向ScrollBar設定為自動顯示（滑鼠放上去就顯示，移開就消失），縱向ScrollBar設定為永不顯示
        gtk_scrolled_window_set_policy((GtkScrolledWindow *) drawerContextScrolledWindow, GTK_POLICY_AUTOMATIC,
                                       GTK_POLICY_NEVER);
        // 將ScrolledWindow的外框設定為不顯示
        gtk_scrolled_window_set_has_frame((GtkScrolledWindow *) drawerContextScrolledWindow,false);
        // 把ScrolledWindow加上name，用在css
        gtk_widget_set_name((GtkWidget *) drawerContextScrolledWindow, "DrawerContextScrolledWindow");
        // 讓ScrolledWindow縱向撐開到整個Drawer Box（最外層的Box）
        gtk_widget_set_vexpand(drawerContextScrolledWindow,true);

        gtk_box_append(data, drawerContextScrolledWindow);
        showDrawerContext(drawerContext);
        isDrawerContextShow = true;
    }


    int dWidth = 10 * (isDrawerOpen ? -1 : 1);
    step++;

    int width = gtk_widget_get_width(drawer);
    width += dWidth;

    if (width > 300) {
        width = 300;
        step = 0;
    }
    if (width < 65) {
        width = 65;
        step = 0;
    }


    double drawerOpacity = (width - 65) * 1. / (300 - 65);
    gtk_widget_set_size_request(drawer, width, -1);
    gtk_widget_set_opacity(drawerContextScrolledWindow, drawerOpacity);

    if (step == 0) {
        isDrawerOpen = !isDrawerOpen;
        if (!isDrawerOpen) {
            gtk_box_remove(data, drawerContextScrolledWindow);
        }
        isDrawerContextShow = isDrawerOpen;
        isAnimating = false;
        return G_SOURCE_REMOVE; // 停止動畫
    }
    return G_SOURCE_CONTINUE; // 繼續動畫
}


void showDrawerContext(gpointer drawer) {
    GtkWidget *title = gtk_label_new("選擇日期");
    gtk_widget_set_halign(title,GTK_ALIGN_START);
    gtk_widget_set_name(title,"SelectDateTitle");
    gtk_box_append(drawer,title);

    GtkWidget *calendar = gtk_calendar_new();
    gtk_widget_set_name(calendar,"SelectCalendar");
    gtk_box_append(drawer,calendar);
}
