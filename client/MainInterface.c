#include <gtk/gtk.h>
#include <protocol/info/Data.h>
#include "MainInterface.h"
#include "ClientSockfd.h"
#include <cairo.h>
#include <logger.h>
#include <imcommon/friends.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <math.h>
#include "chart.h"

static GtkWidget *background, *headx, *search, *friend, *closebut;
static GtkWidget *window;
static GtkTreeView *treeView;
static GtkWidget *frameLayout, *MainLayout;
static GtkTreeIter iter1, iter2;
static cairo_surface_t *surfacemainbackgroud, *surfacehead2, *surfaceresearch, *surfacefriendimage, *surfaceclose51, *surfaceclose52, *surfaceclose53;


static GtkTreeStore *store;
static GdkPixbuf *pixbuf;
static cairo_t *cr;
static GtkWidget *vbox;
GtkWidget  *closebut_event_box, *background_event_box;

enum {
    PIXBUF_COL = 0,
    FRIENDUID_COL = 1,
};
//typedef struct node {
//    uint32_t uid;
//    struct node *next;
//} frienduid;
//
//frienduid *uidhead = NULL;

GtkTreeModel *createModel() {
    gint i, j;
    cairo_surface_t *surface;
    cairo_surface_t *surfaceIcon;

    store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_UINT);
    for (i = 0; i < friends->groupCount; i++) {
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 260, 33);
        cr = cairo_create(surface);
        cairo_move_to(cr, 0, 20);
        cairo_set_font_size(cr, 14);
        cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_show_text(cr, friends->groups[i].groupName);
        pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, 260, 33);
        gtk_tree_store_append(store, &iter1, NULL);
        gtk_tree_store_set(store, &iter1,
                PIXBUF_COL, pixbuf,
                FRIENDUID_COL, friends->groups[i].groupId,
                -1);

        g_object_unref(pixbuf);

        for (j = 0; j < friends->groups[i].friendCount; j++) {
            char friendname[20] = {0};
            char mulu[80] = {0};
            sprintf(mulu, "%s/.momo/friend/%u.png", getpwuid(getuid())->pw_dir, friends->groups[i].friends[j]);
            pixbuf = gdk_pixbuf_new_from_file(mulu, NULL);
            friendinfo *rear = friendinfohead;
            while (rear) {
                if (rear->sessionid == friends->groups[i].friends[j]) {
                    memcpy(friendname, rear->user.nickName, sizeof(rear->user.nickName));
                    break;
                }
                rear = rear->next;
            }
            // gint h = gdk_pixbuf_get_height(pixbuf);
            //加载一个图片
            surfaceIcon = cairo_image_surface_create_from_png(mulu);
            int w = cairo_image_surface_get_width( surfaceIcon);
            int h = cairo_image_surface_get_height( surfaceIcon);
            //创建画布
            surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 260, 200);
            //创建画笔
            cr = cairo_create(surface);
            cairo_save(cr);
            cairo_arc(cr, 31, 31, 31, 0, M_PI * 2);
            cairo_clip(cr);
            //缩放
            cairo_scale(cr,62.0/w, 62.0/h);
            //把画笔和图片相结合。
            cairo_set_source_surface(cr, surfaceIcon, 0, 0);
            //把图用画笔画在画布中
            cairo_paint(cr);
            cairo_restore(cr);
            //设置源的颜色
            cairo_set_source_rgb(cr, 0, 0, 0);
            //从图像的w+10,30区域开始加入字体
            cairo_move_to(cr,90 ,40);
            cairo_set_font_size(cr, 20);
            cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

            cairo_show_text(cr, friendname);

            pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0,  260,  60);
            gtk_tree_store_append(store, &iter2, &iter1);
            gtk_tree_store_set(store, &iter2,
                    PIXBUF_COL, pixbuf,
                    FRIENDUID_COL, friends->groups[i].friends[j],
                    -1);
            g_object_unref(pixbuf);
        }
    }
    cairo_destroy(cr);
    return GTK_TREE_MODEL(store);
}


static void create_surfaces() {

    surfacemainbackgroud = cairo_image_surface_create_from_png("主背景.png");
    surfacehead2 = cairo_image_surface_create_from_png("头像2.png");
    surfaceresearch = cairo_image_surface_create_from_png("搜索.png");
    surfacefriendimage = cairo_image_surface_create_from_png("好友.png");
    surfaceclose51 = cairo_image_surface_create_from_png("关闭按钮1.png");
    surfaceclose52 = cairo_image_surface_create_from_png("关闭按钮2.png");
    surfaceclose53 = cairo_image_surface_create_from_png("关闭按钮3.png");

    background = gtk_image_new_from_surface(surfacemainbackgroud);
    gtk_container_add (GTK_CONTAINER(background_event_box),background);
    gtk_fixed_put(GTK_FIXED(MainLayout), background_event_box, 0, 0);//起始坐标

    headx = gtk_image_new_from_surface(surfacehead2);
    gtk_fixed_put(GTK_FIXED(MainLayout), headx, 2, 10);

    search = gtk_image_new_from_surface(surfaceresearch);
    gtk_fixed_put(GTK_FIXED(MainLayout), search, 0, 140);

    friend = gtk_image_new_from_surface(surfacefriendimage);
    gtk_fixed_put(GTK_FIXED(MainLayout), friend, -10, 174);

    closebut = gtk_image_new_from_surface(surfaceclose51);
    gtk_container_add (GTK_CONTAINER(closebut_event_box),closebut);
    gtk_fixed_put(GTK_FIXED(MainLayout), closebut_event_box, 247, 0);

}

static void
destroy_surfaces() {
    g_print("destroying surfaces2");
    cairo_surface_destroy(surfacemainbackgroud);
    cairo_surface_destroy(surfacehead2);
    cairo_surface_destroy(surfaceresearch);
    cairo_surface_destroy(surfacefriendimage);

}

//单击分组显示右键菜单
gboolean button2_press_event2(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    GdkEventButton *event_button;
    GtkWidget *menu = GTK_WIDGET(data);
    GtkTreeIter iter;
    GtkTreeView *treeview = GTK_TREE_VIEW(widget);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    gtk_tree_selection_get_selected(selection, &model, &iter);

    if (event->type == GDK_BUTTON_PRESS) {
        int i;
        GtkTreePath *path;
        path = gtk_tree_model_get_path(model, &iter);
        i = gtk_tree_path_get_indices(path)[0];

        event_button = (GdkEventButton *) event;

        if (event->button == 0x1) {
            return FALSE;
        }
        if (event->button == 0x2) {
            return FALSE;
        }
        if (event->button == 0x3) {
            if ((gtk_tree_model_iter_has_child(model, &iter)) || (friends->groups[i].friendCount == 0)) {
                gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event_button->button, event_button->time);
                return FALSE;
            }
        }
    }

    return FALSE;
}

//树状视图双击列表事件 &&单击好友显示右键菜单
gboolean button2_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    GdkEventButton *event_button;
    GtkTreeIter iter;

    GtkTreeView *treeview = GTK_TREE_VIEW(widget);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    gtk_tree_selection_get_selected(selection, &model, &iter);
    GtkWidget *menu = GTK_WIDGET(data);
    if (event->type == GDK_BUTTON_PRESS) {
        event_button = (GdkEventButton *) event;

        if (event->button == 0x1) {
            return FALSE;
        }
        if (event->button == 0x2) {
            return FALSE;
        }
        if (event->button == 0x3) {
            int i, j;
            GtkTreePath *path;
            path = gtk_tree_model_get_path(model, &iter);
            i = gtk_tree_path_get_indices(path)[0];
            j = gtk_tree_path_get_indices(path)[1];

            if ((gtk_tree_model_iter_has_child(model, &iter) == 0) && !((i == 0) && (j == 0)) && (friends->groups[i].friendCount > 0)) {
                gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event_button->button, event_button->time);
                return FALSE;
            }
        }
    }
    else if (event->type == GDK_2BUTTON_PRESS && event->button == 0x1) {
        int i, j;
        int uidfindflag = 0;
        GtkTreePath *path;
        friendinfo *friendinforear;
        path = gtk_tree_model_get_path(model, &iter);
        i = gtk_tree_path_get_indices(path)[0];
        j = gtk_tree_path_get_indices(path)[1];

        if (gtk_tree_model_iter_has_child(model, &iter) == 0 && ((i == 0 && j > 0) || ((i != 0) && (friends->groups[i].friendCount > 0)))) {
            uint32_t t;
            gtk_tree_model_get(model, &iter, FRIENDUID_COL, &t, -1);
            log_info("DEBUG", "UID:%u\n", t);
            friendinforear = friendinfohead;
            while (friendinforear) {
                if (friendinforear->user.uid == t) {

                    uidfindflag = 1;
                    break;
                }
                else {friendinforear = friendinforear->next;}
            }
            if (uidfindflag == 1) {
                if (friendinforear->chartwindow == NULL) {
                    mainchart(friendinforear);
                }
                else {
                    gtk_window_set_keep_above(GTK_WINDOW(friendinforear->chartwindow), TRUE);
                }
            }

        }
    }
    return FALSE;

}

void recd_server_msg(const gchar *rcvd_text, uint32_t recd_uid) {

    log_info("DEBUG", "Posting Message.From %u,Text:%s\n",recd_uid,rcvd_text);
    int uidfindflag = 0;
    friendinfo *userinfo = friendinfohead;
    while (userinfo) {
        if (userinfo->user.uid == recd_uid) {
            uidfindflag = 1;
            break;
        }
        else {
            userinfo = userinfo->next;
        }
    }
    if (uidfindflag == 1) {
        if (userinfo->chartwindow == NULL) {
            mainchart(userinfo);
        }
        Show_remote_text(rcvd_text,userinfo);
    }
}

static gint background_button_press_event(GtkWidget *widget,

        GdkEventButton *event, gpointer data)
{
    //设置在非按钮区域内移动窗口
    gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));
    if (event->button == 1)
    {
        gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)), event->button,
                event->x_root, event->y_root, event->time);
    }
    return 0;

}
//鼠标点击事件
static gint closebut_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

    if (event->button == 1) {              //设置关闭按钮
        gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
        gtk_image_set_from_surface((GtkImage *) closebut, surfaceclose52); //置换图标
    }
    return 0;
}

//鼠标抬起事件
static gint closebut_button_release_event(GtkWidget *widget, GdkEventButton *event,

        gpointer data) {

//    x = event->x;  // 取得鼠标相对于窗口的位置
//    y = event->y;
    if (event->button == 1)       // 判断是否是点击关闭图标

    {
        gtk_image_set_from_surface((GtkImage *) closebut, surfaceclose51);  //设置关闭按钮
            destroy_surfaces();
            DeleteEvent();
    }

    return 0;
}

//鼠标移动事件
static gint closebut_enter_notify_event(GtkWidget *widget, GdkEventButton *event,

        gpointer data) {
    gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_HAND2));
    gtk_image_set_from_surface((GtkImage *) closebut, surfaceclose53);
    return 0;
}

static gint closebut_leave_notify_event(GtkWidget *widget, GdkEventButton *event,

        gpointer data)         // 离开事件
{
    gdk_window_set_cursor(gtk_widget_get_window(window), gdk_cursor_new(GDK_ARROW));
    gtk_image_set_from_surface((GtkImage *) closebut, surfaceclose51);
    return 0;
}

int maininterface() {

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;//列表
    vbox = gtk_box_new(TRUE, 5);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 284, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);


    MainLayout = gtk_fixed_new();
    frameLayout = gtk_layout_new(NULL, NULL);

    closebut_event_box = gtk_event_box_new ();
    background_event_box = gtk_event_box_new ();

    create_surfaces();
    gtk_widget_set_events( background_event_box,  // 设置窗体获取鼠标事件

            GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK

                    | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
    g_signal_connect(G_OBJECT( background_event_box), "button_press_event",
            G_CALLBACK(background_button_press_event), NULL);



    gtk_widget_set_events(closebut_event_box,  // 设置窗体获取鼠标事件

            GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK

                    | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK

                    | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);

    g_signal_connect(G_OBJECT(closebut_event_box), "button_press_event",
            G_CALLBACK(closebut_button_press_event),NULL);       // 加入事件回调
    g_signal_connect(G_OBJECT(closebut_event_box), "enter_notify_event",
            G_CALLBACK(closebut_enter_notify_event), NULL);
    g_signal_connect(G_OBJECT(closebut_event_box), "button_release_event",
            G_CALLBACK(closebut_button_release_event), NULL);
    g_signal_connect(G_OBJECT(closebut_event_box), "leave_notify_event",
            G_CALLBACK(closebut_leave_notify_event), NULL);

    GtkWidget *userid;
    userid = gtk_label_new(userdata.nickName);
    //设置字体大小
    PangoFontDescription *font;
    font = pango_font_description_from_string("Sans");//"Sans"字体名
    pango_font_description_set_size(font, 20 * PANGO_SCALE);//设置字体大小
    gtk_widget_override_font(userid, font);

    gtk_fixed_put(GTK_FIXED(MainLayout), userid, 170, 90);

    gtk_container_add(GTK_CONTAINER(window), frameLayout);//frameLayout 加入到window
    gtk_container_add(GTK_CONTAINER(frameLayout), MainLayout);

    treeView = (GtkTreeView *) gtk_tree_view_new_with_model(createModel());//list
    //gtk_tree_view_column_set_resizable(column,TRUE);//加了就bug了
    gtk_tree_view_set_headers_visible(treeView, 0);//去掉头部空白

    //添加树形视图
    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes(NULL, renderer,
            "pixbuf", PIXBUF_COL,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);
    gtk_tree_view_column_set_resizable(column, TRUE);


    //添加滚动条

    GtkScrolledWindow *sw = (GtkScrolledWindow *) gtk_scrolled_window_new(NULL, NULL);
    //设置滚动条常在状态
    gtk_scrolled_window_set_policy(sw,
            GTK_POLICY_ALWAYS,
            GTK_POLICY_ALWAYS);
    //获取水平滚动条
    GtkWidget *widget = gtk_scrolled_window_get_hscrollbar(sw);
    gtk_container_add(GTK_CONTAINER(sw), (GtkWidget *) treeView);
    gtk_fixed_put(GTK_FIXED(MainLayout), (GtkWidget *) sw, 0, 225);
    gtk_widget_set_size_request((GtkWidget *) sw, 284, 358);
//
//    g_signal_connect(G_OBJECT(treeView), "button_press_event",
//            G_CALLBACK(button_press_event), treeView);
//
    //    //右键菜单
    GtkWidget *menu1, *menu2;
    GtkWidget *add;
    GtkWidget *delete;
    GtkWidget *addpeople;
    GtkWidget *Refresh;
    GtkWidget *sendmsg;
    GtkWidget *deletefriend;
    GtkWidget *remark;
    GtkWidget *sendfile;
    //分组菜单
    menu1 = gtk_menu_new();
    add = gtk_menu_item_new_with_mnemonic("添加分组");
    gtk_container_add(GTK_CONTAINER(menu1), add);
    gtk_widget_show(add);
    delete = gtk_menu_item_new_with_mnemonic("删除分组");
    gtk_container_add(GTK_CONTAINER(menu1), delete);
    gtk_widget_show(delete);
    addpeople = gtk_menu_item_new_with_mnemonic("添加联系人");
    gtk_container_add(GTK_CONTAINER(menu1), addpeople);
    gtk_widget_show(addpeople);
    Refresh = gtk_menu_item_new_with_mnemonic("刷新好友列表");
    gtk_container_add(GTK_CONTAINER(menu1), Refresh);
    gtk_widget_show(Refresh);

    g_signal_connect(G_OBJECT(treeView), "button_press_event",
            G_CALLBACK(button2_press_event2), (gpointer) menu1);

    //好友菜单
    menu2 = gtk_menu_new();
    sendmsg = gtk_menu_item_new_with_mnemonic("发送即时消息");
    gtk_container_add(GTK_CONTAINER(menu2), sendmsg);
    gtk_widget_show(sendmsg);
    deletefriend = gtk_menu_item_new_with_mnemonic("删除好友");
    gtk_container_add(GTK_CONTAINER(menu2), deletefriend);
    gtk_widget_show(deletefriend);
    remark = gtk_menu_item_new_with_mnemonic("修改备注");
    gtk_container_add(GTK_CONTAINER(menu2), remark);
    gtk_widget_show(remark);
    sendfile = gtk_menu_item_new_with_mnemonic("发送文件");
    gtk_container_add(GTK_CONTAINER(menu2), sendfile);
    gtk_widget_show(sendfile);
    g_signal_connect(G_OBJECT(treeView), "button_press_event",
            G_CALLBACK(button2_press_event), (gpointer) menu2);

    gtk_widget_show_all(window);
    //隐藏水平滚动条
    gtk_widget_hide(widget);
    //gtk_main();

    return 0;
}

gboolean DestoryMainInterFace(gpointer user_data)
{
    gtk_widget_destroy(window);
    return FALSE;
}