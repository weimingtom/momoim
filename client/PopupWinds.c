#include <gtk/gtk.h>
#include <string.h>
#include "PopupWinds.h"
#include "common.h"

/*弹窗的控件*/
static GtkWidget *PopupWindow; //dialog
static GtkWidget *PopupLayout; //放入box
static GtkWidget *PopBack, *PopAnniu;
static GtkWidget *Box, *Action; //dialog的box和action_area
static cairo_surface_t *BackFace, *AnniuFace, *AnniuFace1;

static void create_popupfaces()
{
    BackFace = ChangeThem_png("提示框.png");
    AnniuFace = ChangeThem_png("提示框按钮1.png");
    AnniuFace1 = ChangeThem_png("提示框按钮2.png");

    PopBack = gtk_image_new_from_surface(BackFace);
    PopAnniu = gtk_image_new_from_surface(AnniuFace);
}

static void destroy_popfaces()
{
    g_print("destroying poppuwinds");
    cairo_surface_destroy(BackFace);
    cairo_surface_destroy(AnniuFace);
    cairo_surface_destroy(AnniuFace1);
}

//背景的eventbox拖曳窗口
static gint popback_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(PopupWindow), gdk_cursor_new(GDK_ARROW));
    if (event->button == 1)
    { //gtk_widget_get_toplevel 返回顶层窗口 就是window.
        gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                   event->button,
                                   event->x_root,
                                   event->y_root,
                                   event->time);
    }
    return 0;
}

//确定
//鼠标点击事件
static gint ok_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)
    {
        gdk_window_set_cursor(gtk_widget_get_window(PopupWindow), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
    }
    return 0;
}

//确定
//鼠标抬起事件
static gint ok_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)
    {
        gtk_dialog_response((GtkDialog *) PopupWindow, 1);
        /*执行到这里就可以执行gtk_dialog_run(GTK_DIALOG(PopupWindow));下面的语句了*/
    }
    return 0;
}

//确定
//鼠标移动事件
static gint ok_enter_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(PopupWindow), gdk_cursor_new(GDK_HAND2));
    gtk_image_set_from_surface((GtkImage *) PopAnniu, AnniuFace1);//置换确定按钮图片
    return 0;
}

//确定
//鼠标离开事件
static gint ok_leave_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(PopupWindow), gdk_cursor_new(GDK_ARROW));
    gtk_image_set_from_surface((GtkImage *) PopAnniu, AnniuFace);//置换确定按钮图片
    return 0;
}

int Popup(const char *title, const char *tell)
{
    PopupWindow = gtk_dialog_new();//新建一个dialog
    gtk_window_set_position(GTK_WINDOW(PopupWindow), GTK_WIN_POS_CENTER);//窗口位置
    gtk_window_set_resizable(GTK_WINDOW (PopupWindow), FALSE);//固定窗口大小
    gtk_window_set_decorated(GTK_WINDOW(PopupWindow), FALSE);//去掉边框
    gtk_widget_set_size_request(GTK_WIDGET(PopupWindow), 250, 235);//设置窗体大小

    Box = gtk_dialog_get_content_area((GtkDialog *) PopupWindow);//得到dialog的box
    Action = gtk_dialog_get_action_area((GtkDialog *) PopupWindow);//得到dialog的action_area
    int maWidth, maHeight;
    PopupLayout = gtk_fixed_new();//创建布局容纳控件
    create_popupfaces();

    //声音
    PlayMusic("提示音.wav");

    static GtkEventBox *Popback_event_box, *Anniu_event_box;
    GtkWidget *telltitle, *tellyou;
    gtk_container_add(GTK_CONTAINER(Box), PopupLayout);//将layout放入dialog的box中

    telltitle = gtk_label_new(title);//标题
    tellyou = gtk_label_new(tell);//内容

    PangoContext *context = gtk_widget_get_pango_context(tellyou);

    PangoLayout *layout = pango_layout_new(context);
    //pango_layout_set_font_description(layout, pango_font_description_from_string("Monospace 15"));
    pango_layout_set_text(layout, tell, (int) strlen(tell));
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);
    pango_layout_get_pixel_size(layout, &maWidth, &maHeight);

    // 设置窗体获取鼠标事件
    Popback_event_box = BuildEventBox(PopBack, G_CALLBACK(popback_button_press_event), NULL, NULL, NULL, NULL, NULL);
    gtk_fixed_put(GTK_FIXED(PopupLayout), GTK_WIDGET(Popback_event_box), 0, 0);

    Anniu_event_box = BuildEventBox(PopAnniu,
                                    G_CALLBACK(ok_button_press_event),
                                    G_CALLBACK(ok_enter_notify_event),
                                    G_CALLBACK(ok_leave_notify_event),
                                    G_CALLBACK(ok_button_release_event),
                                    NULL,
                                    NULL);
    gtk_fixed_put(GTK_FIXED(PopupLayout), GTK_WIDGET(Anniu_event_box), 52, 170);

    gtk_fixed_put(GTK_FIXED(PopupLayout), telltitle, 14, 10);
    if (maWidth > 400)
    {
        maWidth = 400;
    }
    //gtk_label_set_justify(GTK_LABEL(tellyou),GTK_JUSTIFY_CENTER);/*设置标号对齐方式为居中对齐*/
    //gtk_label_set_line_wrap(GTK_LABEL(tellyou),TRUE);/*打开自动换行*/

    if (80 + maWidth < 250)
    {
        gtk_fixed_put(GTK_FIXED(PopupLayout), tellyou, (250 - maWidth) / 2, 110);
        gtk_widget_set_size_request(GTK_WIDGET(PopupWindow), 250, 235);
    }
    else
    {
        gtk_fixed_put(GTK_FIXED(PopupLayout), tellyou, 40, 110);
        gtk_widget_set_size_request(GTK_WIDGET(PopupWindow), 80 + maWidth, 235);
    }
    //gtk_widget_show_all(PopupLayout);
    gtk_widget_show_all(PopupWindow);
    gtk_widget_hide(Action);//隐藏留白的action

    /*监控gtk_dialog_response((GtkDialog *) PopupWindow, 1);的信号*/
    gtk_dialog_run(GTK_DIALOG(PopupWindow));

    destroy_popfaces();
    gtk_widget_destroy(PopupWindow);
    return 0;
}

