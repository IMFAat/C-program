#include <gtk/gtk.h>
#include <pthread.h>
#define ms *1000000

void DrawerButtonOnClick(GtkWidget *button, gpointer data);

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

gboolean drawerAnimation(gpointer data);

void DrawerButtonOnClick(GtkWidget *button, gpointer data) {
    if (isAnimating) return;
    isAnimating = true;
    g_idle_add(drawerAnimation, data);
}

gboolean drawerAnimation(gpointer data) {
    static int step = 0;
    static int dWidth = 10;
    GtkWidget *drawer = (GtkWidget *) data;

    if (step == 0) {
        dWidth = 10 * (isDrawerOpen ? -1 : 1);
    }
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
    printf("%d\n", width);
    gtk_widget_set_size_request(drawer, width, -1);
    if (step == 0) {
        isDrawerOpen = !isDrawerOpen;
        isAnimating = false;
        return G_SOURCE_REMOVE; // 停止動畫
    }
    return G_SOURCE_CONTINUE; // 繼續動畫
}
