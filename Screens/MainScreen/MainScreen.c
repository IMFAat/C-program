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

    g_signal_connect(button, "clicked", G_CALLBACK(DrawerButtonOnClick), drawerBox);
}

bool isDrawerOpen = false;
bool isAnimating = false;
bool isDrawerContextShow = false;
GtkWidget *drawerContextScrolledWindow = NULL;

gboolean drawerAnimation(gpointer data);

void DrawerButtonOnClick(GtkWidget *button, gpointer data) {
    if (isAnimating) return;
    isAnimating = true;
    g_idle_add(drawerAnimation, data);
}

gboolean drawerAnimation(gpointer data) {
    int step = 0;
    GtkWidget *drawer = (GtkWidget *) data;

    if (!isDrawerContextShow) {
        drawerContextScrolledWindow = (GtkWidget *) gtk_scrolled_window_new();
        GtkWidget *drawerContext = (GtkWidget *) gtk_box_new((GtkOrientation) GTK_ORIENTATION_VERTICAL, 10);
        gtk_scrolled_window_set_child((GtkScrolledWindow *) drawerContextScrolledWindow, drawerContext);
        gtk_scrolled_window_set_policy((GtkScrolledWindow *) drawerContextScrolledWindow, GTK_POLICY_AUTOMATIC,
                                       GTK_POLICY_NEVER);
        gtk_scrolled_window_set_has_frame((GtkScrolledWindow *) drawerContextScrolledWindow,false);
        gtk_widget_set_name((GtkWidget *) drawerContextScrolledWindow, "DrawerContextScrolledWindow");
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
    GtkWidget *button = (GtkWidget *) gtk_button_new();
    gtk_button_set_label((GtkButton *) button,
                         "adkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadkadk");

    gtk_box_append((GtkBox *) drawer, button);
}
