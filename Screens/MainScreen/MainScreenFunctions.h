//
// Created by Kai on 11/21/24.
//

#ifndef MAINSCREENFUNCTIONS_H
#define MAINSCREENFUNCTIONS_H
#include<gtk/gtk.h>
struct BottomBoxAnimationData {
    GtkWidget *backBox;
    GtkWidget *button;
    GtkWidget *scrollBox;
    GtkWidget *contentBox;
    int type; //0->Daily, 1->Weekly
};

void DrawerButtonOnClick(GtkWidget *button, gpointer data);

void AddButtonOnClick(GtkWidget *button, gpointer data);

void showDrawerContext(gpointer drawer);

void drawerCalendarSelect(GtkWidget *calendar, gpointer data);

void set_all_rounded_radius(GtkWidget *widget, float radius, int type);

void set_margin(GtkWidget *widget, float margin, int type);

gboolean drawerAnimation(gpointer data);

gboolean bottomButtonAnimation(gpointer data);

void showAddContext(struct BottomBoxAnimationData data);
#endif //MAINSCREENFUNCTIONS_H
