//
// Created by Kai on 11/21/24.
//
#include <gtk/gtk.h>
#include "MainScreenFunctions.h"
bool isDrawerOpen = false; // 判斷Drawer是否打開
bool isAnimating = false; // 判斷Drawer動畫是否在進行中
bool isDrawerContextShow = false; // 判斷Drawer的內容是否顯示
GtkWidget *drawerContextScrolledWindow = NULL; // Drawer內容(ScrolledWindow)

bool isAddContentShowing[2] = {false,false};

// Drawer
void DrawerButtonOnClick(GtkWidget *button, gpointer data) {
    // 如果動畫有在進行的話就不重複進行動畫
    if (isAnimating) return;
    isAnimating = true;
    // 將動畫的 function 每10秒一次執行
    g_timeout_add(5, (GSourceFunc) drawerAnimation, data);
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
        gtk_scrolled_window_set_has_frame((GtkScrolledWindow *) drawerContextScrolledWindow, false);
        // 把ScrolledWindow加上name，用在css
        gtk_widget_set_name((GtkWidget *) drawerContextScrolledWindow, "DrawerContextScrolledWindow");
        // 讓ScrolledWindow縱向撐開到整個Drawer Box（最外層的Box）
        gtk_widget_set_vexpand(drawerContextScrolledWindow, true);

        gtk_box_append((GtkBox *) data, drawerContextScrolledWindow);
        showDrawerContext(drawerContext);
        isDrawerContextShow = true;
    }


    int dWidth = 10 * (isDrawerOpen ? -1 : 1);
    step++;

    int width = gtk_widget_get_width(drawer);
    width += dWidth;

    if (width > 330) {
        width = 330;
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
            gtk_box_remove((GtkBox *) data, drawerContextScrolledWindow);
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
    gtk_box_append((GtkBox *) drawer, title);

    GtkWidget *calendar = gtk_calendar_new();
    gtk_widget_set_name(calendar, "SelectCalendar");
    g_signal_connect(calendar, "day-selected", G_CALLBACK(drawerCalendarSelect), NULL);
    gtk_box_append((GtkBox *) drawer, calendar);
}

void AddButtonOnClick(GtkWidget *button, gpointer data) {
    g_timeout_add(5, (GSourceFunc) bottomButtonAnimation, data);
    // g_idle_add((GSourceFunc) bottomButtonAnimation, data);
}

void drawerCalendarSelect(GtkWidget *calendar, gpointer data) {
    printf("%d\n", gtk_calendar_get_day((GtkCalendar *) calendar));
}

void set_all_rounded_radius(GtkWidget *widget, float radius, int type) {
    char cssClass[64];
    snprintf(cssClass, sizeof(cssClass), "roundedAnimation%d", (type == 0 ? 0 : 1));
    gtk_widget_add_css_class(widget, cssClass);
    char css[128];
    snprintf(css, sizeof(css), ".roundedAnimation%d { border-radius: %.2fpx; }", (type == 0 ? 0 : 1), radius);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(cssProvider, css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),GTK_STYLE_PROVIDER(cssProvider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void set_margin(GtkWidget *widget, float margin, int type) {
    char cssClass[64];
    snprintf(cssClass, sizeof(cssClass), "marginAnimation%d", (type == 0 ? 0 : 1));
    gtk_widget_add_css_class(widget, cssClass);
    char css[64];
    snprintf(css, sizeof(css), ".marginAnimation%d { margin: %fpx; }", (type == 0 ? 0 : 1), margin);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(cssProvider, css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),GTK_STYLE_PROVIDER(cssProvider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
}

gboolean bottomButtonAnimation(gpointer data) {
    struct BottomBoxAnimationData *tisData = (struct BottomBoxAnimationData *) data;
    if (tisData == NULL || tisData->backBox == NULL) {
        g_free(tisData); // 防止記憶體洩漏
        return G_SOURCE_REMOVE;
    }
    if (!isAddContentShowing[tisData->type]) {
        GtkWidget *addContextScrolledWindow = (GtkWidget *) gtk_scrolled_window_new();
        GtkWidget *contentBox = (GtkWidget *) gtk_box_new((GtkOrientation) GTK_ORIENTATION_VERTICAL, 10);
        gtk_scrolled_window_set_child((GtkScrolledWindow *) addContextScrolledWindow, contentBox);
        gtk_scrolled_window_set_policy((GtkScrolledWindow *) addContextScrolledWindow, GTK_POLICY_NEVER,
                                       GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_has_frame((GtkScrolledWindow *) addContextScrolledWindow, false);
        // gtk_widget_set_name((GtkWidget *) addContextScrolledWindow, "AddContextScrolledWindow");
        gtk_widget_set_vexpand(addContextScrolledWindow, false);
        gtk_widget_set_hexpand(addContextScrolledWindow, true);

        gtk_box_append((GtkBox *) tisData->scrollBox, addContextScrolledWindow);
        tisData->contentBox = contentBox;

        showAddContext(*tisData);
        isAddContentShowing[tisData->type] = true;
    }
    int height = gtk_widget_get_height((GtkWidget *) tisData->backBox);
    int dh = 10;

    if (height >= 300) {
        gtk_widget_set_size_request(tisData->backBox, -1, 300);
        return G_SOURCE_REMOVE; // 停止動畫
    }
    height += dh;
    float dailyBoxRadius = (float) 25.0 + (float) (15 - 30) * (float) (height - 50) / (300 - 50);
    // 50+(15-30)*(h-50)/(300-50)
    set_all_rounded_radius(tisData->backBox, dailyBoxRadius, tisData->type);
    float margin = (float) 8.0 * (height - 50) / (300 - 50);
    set_margin(tisData->button, margin, tisData->type);

    gtk_widget_set_size_request(tisData->backBox, -1, height);
    gtk_widget_set_size_request(tisData->scrollBox, -1, height - 50);
    // usleep(1000);
    return G_SOURCE_CONTINUE; // 繼續動畫
}

void showAddContext(struct BottomBoxAnimationData data) {
    //彈出要加的東西放在data.contentBox裡面
    GtkWidget *label = gtk_label_new("Hello");
    gtk_box_append((GtkBox *) data.contentBox, label);
    gtk_widget_set_name(label,"LABEL");
}
