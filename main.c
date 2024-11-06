#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {

    GtkWindow *window = (GtkWindow *) gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    GtkStack *stack = (GtkStack *) gtk_stack_new();
    GtkBuilder *mainBuilder = gtk_builder_new_from_file(ROOT_PATH"/src/MainScreen/MainScreen.ui");

    GtkWidget *main_screen = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "MainScreen"));
    if (main_screen) {
        gtk_stack_add_named(stack, main_screen, "MainScreen");
    } else {
        g_warning("Not found MainScreen Widget");
    }

    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(stack));
    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(mainBuilder);
}

int main(int argc, char **argv) {
    // GtkApplication *app;
    int status;

    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS); //創建app實例
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); //連接觸發器
    status = g_application_run(G_APPLICATION(app), argc, argv); //Run app
    g_object_unref(app); //釋放app

    return status;
}
