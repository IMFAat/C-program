#include <gtk/gtk.h>
#define ms *1000000

void DrawerButtonOnClick(GtkWidget *button, gpointer data);

void AddButtonOnClick(GtkWidget *button, gpointer data);

void showDrawerContext(gpointer drawer);

void drawerCalendarSelect(GtkWidget *calendar, gpointer data);

void set_all_rounded_radius(GtkWidget *widget, float radius, int type);

void set_only_rounded_radius(GtkWidget *widget, float topRadius,float bottomRadius, int type);

void set_margin(GtkWidget *widget, float margin, int type);

gboolean drawerAnimation(gpointer data);

gboolean bottomButtonAnimation(gpointer data);

struct BottomBoxAnimationData {
    GtkWidget *backBox;
    GtkWidget *button;
    GtkWidget *contentBox;
    int type; //0->Daily, 1->Weekly
};

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
    dailyAddButtonAnimationData->contentBox = (GtkWidget *) gtk_builder_get_object(builder, "DailyContentBox");
    dailyAddButtonAnimationData->type = 0;

    struct BottomBoxAnimationData *weeklyAddButtonAnimationData = g_new0(struct BottomBoxAnimationData, 1);
    weeklyAddButtonAnimationData->button = (GtkWidget *) gtk_builder_get_object(builder, "WeeklyAddButton");
    weeklyAddButtonAnimationData->backBox = (GtkWidget *) gtk_builder_get_object(builder, "WeeklyButtonBox");
    weeklyAddButtonAnimationData->contentBox = (GtkWidget *) gtk_builder_get_object(builder, "WeeklyContentBox");
    weeklyAddButtonAnimationData->type = 1;

    g_signal_connect(dailyAddButtonAnimationData->button, "clicked", G_CALLBACK(AddButtonOnClick), dailyAddButtonAnimationData);
    g_signal_connect(weeklyAddButtonAnimationData->button, "clicked", G_CALLBACK(AddButtonOnClick), weeklyAddButtonAnimationData);

    // 按鈕連接點及觸發器
    g_signal_connect(drawerButton, "clicked", G_CALLBACK(DrawerButtonOnClick), drawerBox);
}

bool isDrawerOpen = false; // 判斷Drawer是否打開
bool isAnimating = false; // 判斷Drawer動畫是否在進行中
bool isDrawerContextShow = false; // 判斷Drawer的內容是否顯示
GtkWidget *drawerContextScrolledWindow = NULL; // Drawer內容(ScrolledWindow)


// Drawer
void DrawerButtonOnClick(GtkWidget *button, gpointer data) {
    // 如果動畫有在進行的話就不重複進行動畫
    if (isAnimating) return;
    isAnimating = true;
    // 將動畫的 function 每10秒一次執行
    g_timeout_add(10, (GSourceFunc)drawerAnimation, data);
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
    GtkWidget *title = gtk_label_new("Select Date");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_set_name(title, "SelectDateTitle");
    gtk_box_append(drawer, title);

    GtkWidget *calendar = gtk_calendar_new();
    gtk_widget_set_name(calendar, "SelectCalendar");
    g_signal_connect(calendar, "day-selected", G_CALLBACK(drawerCalendarSelect), NULL);
    gtk_box_append(drawer, calendar);
}


void AddButtonOnClick(GtkWidget *button, gpointer data) {
    g_timeout_add(10, (GSourceFunc)bottomButtonAnimation, data);
}


void drawerCalendarSelect(GtkWidget *calendar, gpointer data) {
    printf("%d\n", gtk_calendar_get_day((GtkCalendar *)calendar));
}

void set_all_rounded_radius(GtkWidget *widget, float radius, int type) {
    char class[64];
    snprintf(class, sizeof(class), "roundedAnimation%d", (type == 0 ? 0 : 1));
    gtk_widget_add_css_class(widget, class);
    char css[128];
    snprintf(css, sizeof(css), ".roundedAnimation%d { border-radius: %.2fpx; }", (type == 0 ? 0 : 1), radius);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(cssProvider, css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void set_only_rounded_radius(GtkWidget *widget, float topRadius,float bottomRadius, int type) {
    char class[64];
    snprintf(class, sizeof(class), "topRoundedAnimation%d", (type == 0 ? 0 : 1));
    gtk_widget_add_css_class(widget, class);
    char css[256];
    snprintf(css, sizeof(css), ".topRoundedAnimation%d { border-radius: %.2fpx %.2fpx %.2fpx %.2fpx; }", (type == 0 ? 0 : 1), topRadius,topRadius, bottomRadius,bottomRadius);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(cssProvider, css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void set_margin(GtkWidget *widget, float margin, int type) {
    char class[64];
    snprintf(class, sizeof(class), "marginAnimation%d", (type == 0 ? 0 : 1));
    gtk_widget_add_css_class(widget, class);
    char css[64];
    snprintf(css, sizeof(css), ".marginAnimation%d { margin: %fpx; }", (type == 0 ? 0 : 1), margin);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(cssProvider, css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);
}

gboolean bottomButtonAnimation(gpointer data) {
    struct BottomBoxAnimationData *tisData = (struct BottomBoxAnimationData *) data;
    if (tisData == NULL || tisData->backBox == NULL) {
        g_free(tisData); // 防止記憶體洩漏
        return G_SOURCE_REMOVE;
    }
    int height = gtk_widget_get_height((GtkWidget *) tisData->backBox);
    int dh = 5;

    if (height >= 300) {
        gtk_widget_set_size_request(tisData->backBox, -1, 300);
        return G_SOURCE_REMOVE; // 停止動畫
    }
    height += dh;
    float dailyBoxRadius = (float)25.0+(float)(15-30)*(float)(height-50)/(300-50); // 50+(15-30)*(h-50)/(300-50)
    set_all_rounded_radius(tisData->backBox, dailyBoxRadius, tisData->type);
    // float margin = 5.0 * (height - 50) / (300 - 50);
    // set_margin(tisData->button, margin, tisData->type);
    set_only_rounded_radius(tisData->button,0,dailyBoxRadius,tisData->type);

    gtk_widget_set_size_request(tisData->backBox, -1, height);
    gtk_widget_set_size_request(tisData->contentBox, -1, height - 50);
    // usleep(1000);
    return G_SOURCE_CONTINUE; // 繼續動畫
}
