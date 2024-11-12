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

void *drawerAnimation(gpointer data);

void DrawerButtonOnClick(GtkWidget *button, gpointer data) {
    if(isAnimating) return;
    pthread_t animationThread;
    pthread_create(&animationThread,NULL, drawerAnimation, data);
}

void *drawerAnimation(gpointer data) {
    struct timespec ts;
    ts.tv_sec = 0;            // 秒
    ts.tv_nsec = 0.5 ms;     // 納秒 (1毫秒)

    isAnimating = true;
    GtkWidget *drawer = (GtkWidget *) data;
    int lWidth = 0; //上次的寬度
    int dWidth = 10; //每次增加的寬度
    int addCount = 30; //增加的次數
    dWidth *= isDrawerOpen ? -1 : 1;
    for (int i = 0; i < addCount; i++) {
        int width = gtk_widget_get_width(drawer);
        width += dWidth;
        while (lWidth == width) {
            width = gtk_widget_get_width(drawer);
            width += dWidth;
            nanosleep(&ts, NULL);
        }
        printf("%d\n", width);
        lWidth = width;
        gtk_widget_set_size_request(data, width, -1);
    }
    isDrawerOpen = !isDrawerOpen;
    isAnimating = false;
    pthread_exit(NULL);
}
