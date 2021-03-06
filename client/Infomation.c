#include <gtk/gtk.h>
#include <imcommon/friends.h>
#include <string.h>
#include <logger.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <imcommon/user.h>
#include <math.h>
#include <protocol/base.h>
#include <pwd.h>
#include "ClientSockfd.h"
#include "common.h"
#include "MainInterface.h"
#include "PopupWinds.h"
#include "Infomation.h"

static cairo_surface_t *Surfaceback, *Surfacesave, *Surfacesave1, *Surfacecancel, *Surfacecancel1, *Surfaceend, *Surfaceend1, *Surfaceend2;
static cairo_surface_t *SurfaceHead;
static int RiliFlag = 0;
static guint year = 2011;
static guint month = 10;
static guint day = 11;

static GtkWidget *InfoWind;
static GtkWidget *InfoLayout;
static GtkWidget *InfoBackground, *InfoSave, *InfoCancel, *InfoGuanbi;
static GtkWidget *IId, *ILevel, *ISex, *INickname, *IName, *IBirthday, *IConstellation, *IProvinces, *ICity;
static GtkWidget *ITel, *IMail, *IMotto, *ICalendar;
static GtkWidget *HeadIcon, *BianJi;
char *filename;

static const char *constellations[12] = {
        "水瓶座", "双鱼座", "白羊座", "金牛座",
        "双子座", "巨蟹座", "狮子座", "处女座",
        "天秤座", "天蝎座", "射手座", "魔蝎座"
};

static const char *provinces[35] = {
        "北京", "上海", "天津", "重庆", "河北", "山西", "辽宁", "吉林",
        "河南", "江苏", "浙江", "安徽", "福建", "江西", "山东",
        "湖北", "湖南", "广东", "海南", "四川", "贵州", "云南",
        "陕西", "甘肃", "青海", "黑龙江",
        "内蒙古", "广西", "西藏", "宁夏", "新疆", "台湾省", "香港", "澳门", "其它"
};
static const char *allcity[35][41] = {
//北京市辖区名称
        {
                "东城区", "西城区", "崇文区", "宣武区", "朝阳区", "海淀区", "丰台区", "石景山区", "房山区", "通州区", "顺义区", "门头沟区",
                                                                                                        "昌平区",  "大兴区",   "怀柔区", "平谷区", "密云县",  "延庆县"
        },
//上海市辖区名称
        {
                "黄浦区", "卢湾区", "徐汇区", "长宁区", "静安区", "普陀区",
                                                                  "闸北区", "虹口区", "杨浦区", "宝山区", "闵行区", "嘉定区",
                                                                                                        "浦东新区", "金山区",   "松江区", "青浦区", "南汇区",  "奉贤区", "崇明县"
        },
//天津市辖区名称
        {
                "和平区", "河东区", "河西区", "南开区", "河北区", "红桥区",
                                                                  "塘沽区", "汉沽区", "大港区", "东丽区", "西青区", "津南区",
                                                                                                        "北辰区",  "武清区",   "宝坻区", "宁河县", "静海县",  "蓟县"
        },
//重庆市辖区名称
        {
                "渝中区", "大渡口区", "江北区", "沙坪坝区", "九龙坡区", "南岸区",
                                                                  "北碚区", "万盛区", "双桥区", "渝北区", "巴南区", "万县区",
                                                                                                        "涪陵区",  "永川市",   "合川市", "江津市", "南川市",  "长寿县", "綦江县", "潼南县", "荣昌县", "壁山县", "大足县", "铜梁县",
                "梁平县", "城口县", "垫江县", "武隆县", "丰都县", "忠 县", "开 县",
                "云阳县", "青龙镇青龙嘴", "奉节县", "巫山县", "巫溪县", "南宾镇", "中和镇", "钟多镇", "联合镇", "汉葭镇"
        },
//河北省主要城市名称
        {
                "石家庄", "唐山", "秦皇岛", "邯郸", "邢台", "保定",
                                                                  "张家口", "承德", "沧州", "廊坊", "衡水"
        },
//山西省主要城市名称
        {
                "太原", "大同", "阳泉", "长治", "晋城", "朔州",
                                                                  "晋中", "运城", "忻州", "临汾", "吕梁"
        },
//辽宁省主要城市名称
        {
                "沈阳", "大连", "鞍山", "抚顺", "本溪", "丹东",
                                                                  "锦州", "营口", "阜新", "辽阳", "盘锦", "铁岭",   "朝阳",   "葫芦岛"
        },
//吉林省主要城市名称
        {
                "长春", "吉林", "四平", "辽源", "通化", "白山",
                                                                  "松原", "白城", "延边朝鲜族自治州"
        },
//河南省主要城市名称
        {
                "郑州", "开封", "洛阳", "平顶山", "安阳", "鹤壁",
                                                                  "新乡", "焦作", "濮阳", "许昌", "漯河", "三门峡",
                                                                                                        "南阳",   "商丘",    "信阳",  "周口",  "驻马店",  "济源"
        },
//江苏省主要城市名称
        {
                "南京", "无锡", "徐州", "常州", "苏州", "南通",
                                                                  "连云港", "淮安", "盐城", "扬州", "镇江", "泰州",  "宿迁"
        },
//浙江省主要城市名称
        {
                "杭州", "宁波", "温州", "嘉兴", "湖州", "绍兴",
                                                                  "金华", "衢州", "舟山", "台州", "丽水"
        },
//安徽省主要城市名称
        {
                "合肥", "芜湖", "蚌埠", "淮南", "马鞍山", "淮北",              "铜陵",
                "安庆", "黄山", "滁州", "阜阳", "宿州",                                                           "巢湖",   "六安",    "亳州",  "池州",  "宣城"
        },
//福建省主要城市名称
        {       "福州", "厦门", "莆田", "三明", "泉州", "漳州",               "南平", "龙岩", "宁德"},
//江西省主要城市名称
        {       "南昌", "景德镇", "萍乡", "九江", "新余", "鹰潭",              "赣州", "吉安", "宜春", "抚州", "上饶"},
//山东省主要城市名称
        {
                "济南", "青岛", "淄博", "枣庄", "东营", "烟台",               "潍坊",
                "威海", "济宁", "泰安", "日照", "莱芜",                                                           "临沂",   "德州",    "聊城",  "滨州",  "菏泽"
        },
//湖北省主要城市名称
        {
                "武汉", "黄石", "襄樊", "十堰", "荆州", "宜昌",               "荆门",
                "鄂州", "孝感", "黄冈", "咸宁", "随州",                                                           "恩施",   "仙桃",    "潜江",  "天门",  "神农架林区"
        },
//湖南省主要城市名称
        {
                "长沙", "株洲", "湘潭", "衡阳", "邵阳", "岳阳",               "常德", "张家界",
                "益阳", "郴州", "永州", "怀化市",                                                                "娄底",   "湘西"
        },
//广东省主要城市名称
        {
                "广州", "深圳", "珠海", "汕头", "韶关", "佛山",               "江门", "湛江",
                "茂名", "肇庆", "惠州", "梅州",                                                                 "汕尾",   "河源",    "阳江",  "清远",  "东莞",   "中山",  "潮州",  "揭阳",  "云浮"
        },
//海南省主要城市名称
        {       "海口", "龙华区", "秀英区", "琼山区", "美兰区", "三亚"},
//四川省主要城市名称
        {
                "成都", "自贡", "攀枝花", "泸州", "德阳", "绵阳",              "广元", "遂宁", "内江", "乐山", "南充",
                "宜宾",                                                                                   "广安",   "达州",    "眉山",  "雅安",  "巴中",   "资阳",  "阿坝州", "甘孜州", "凉山州"
        },
//贵州省主要城市名称
        {       "贵阳", "六盘水", "遵义", "安顺", "铜仁地区", "毕节地区",          "黔西南州", "黔东南州", "黔南州"},
//云南省主要城市名称
        {
                "昆明", "大理", "曲靖", "玉溪", "昭通", "楚雄",               "红河", "文山", "思茅", "西双版纳",
                "保山", "德宏",                                                                             "丽江",   "怒江",    "迪庆",  "临沧"
        },
//陕西省主要城市名称
        {       "西安", "铜川", "宝鸡", "咸阳", "渭南", "延安",               "汉中", "榆林", "安康", "商洛"},
//甘肃省主要城市名称
        {
                "兰州", "嘉峪关", "金昌", "白银", "天水", "武威",              "张掖", "平凉",
                "酒泉", "庆阳", "定西", "陇南",                                                                 "临夏州",  "甘南州"
        },
//青海省主要城市名称
        {       "西宁", "海东地区", "海北州", "黄南州", "海南州", "果洛州",         "玉树州", "海西州"},
//黑龙江省主要城市名称
        {
                "哈尔滨", "齐齐哈尔", "鸡西", "鹤岗", "双鸭山", "大庆",           "伊春", "佳木斯",
                "七台河", "牡丹江", "黑河", "绥化",                                                               "大兴安岭地区"
        },
//内蒙古自治区主要城市名称
        {
                "呼和浩特", "包头", "乌海", "赤峰", "通辽", "鄂尔多斯",           "呼伦贝尔", "巴彦淖尔",
                "乌兰察布", "兴安盟", "锡林郭勒盟", "阿拉善盟"
        },
//广西壮族自治区主要城市名称
        {
                "南宁", "柳州", "桂林", "梧州", "北海", "防城港",              "钦州", "贵港", "玉林", "百色",
                "贺州", "河池",                                                                             "来宾",   "崇左"
        },
//西藏自治区主要城市名称
        {       "拉萨", "昌都地区", "山南地区", "日喀则地区", "那曲地区", "阿里地区",    "林芝地区"},
//宁夏回族自治区主要城市名称
        {       "银川", "石嘴山", "吴市", "固原", "中卫"},
//新疆维吾尔自治区主要城市名称
        {
                "乌鲁木齐", "克拉玛依", "吐鲁番地区", "哈密地区", "和田地区", "阿克苏地区", "喀什地区",
                "克孜勒苏柯尔克孜自治州", "巴音郭楞蒙古自治州", "昌吉回族自治州", "博尔塔拉蒙古自治州", "伊犁哈萨克自治州",                         "塔城地区",
                                                                                                                "阿勒泰地区", "石河子", "阿拉尔", "图木舒市", "五家渠"
        },
//台湾省主要城市名称
        {
                "台北", "高雄", "基隆", "台中", "台南", "新竹",               "嘉义", "台北县", "宜兰县",
                "桃园县", "新竹县", "苗栗县",                                                                    "台中县",  "彰化县",   "南投县", "云林县", "嘉义县",  "台南县", "高雄县", "屏东县", "澎湖县", "台东县", "花莲县"
        },
//香港特别行政区主要辖区名称
        {
                "中西区", "东区", "九龙城区", "观塘区", "南区", "深水埗区",         "黄大仙区", "湾仔区",
                "油尖旺区", "离岛区", "葵青区", "北区",                                                             "西贡区",  "沙田区",   "屯门区", "大埔区", "荃湾区",  "元朗区"
        },
//澳门地区
        {       "澳门地区"},
//其它地区
        {       "其它地区"}
};
static const char *xingzuo[12][2] = {
        {"魔蝎座", "水瓶座"},
        {"水瓶座", "双鱼座"},
        {"双鱼座", "白羊座"},
        {"白羊座", "金牛座"},
        {"金牛座", "双子座"},
        {"双子座", "巨蟹座"},
        {"巨蟹座", "狮子座"},
        {"狮子座", "处女座"},
        {"处女座", "天秤座"},
        {"天秤座", "天蝎座"},
        {"天蝎座", "射手座"},
        {"射手座", "魔蝎座"}
};
static UserInfo weinfo;

static void destroy_infosurfaces()
{
    g_print("destroying infoface");
    cairo_surface_destroy(Surfaceback);
    cairo_surface_destroy(Surfacesave);
    cairo_surface_destroy(Surfacesave1);
    cairo_surface_destroy(Surfacecancel);
    cairo_surface_destroy(Surfacecancel1);
    cairo_surface_destroy(Surfaceend);
    cairo_surface_destroy(Surfaceend1);
    cairo_surface_destroy(Surfaceend2);
}

int okinfo(void *data)
{
    Popup("莫默告诉你：", "恭喜你修改成功");
    /*修改成功关闭修改界面*/
    memcpy(CurrentUserInfo, &weinfo, sizeof(weinfo));
    gtk_label_set_label((GtkLabel *) UserId, CurrentUserInfo->nickName);//更新主界面昵称
    //更新主界面分组下用户的头像和昵称
    GdkPixbuf *pixbuf;
    pixbuf = DrawFriend(CurrentUserInfo, 1);
    GtkTreeIter group_iter, useriter;
    uint32_t groupid = 0;

    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(TreeViewListStore), &group_iter);
    gtk_tree_model_get(GTK_TREE_MODEL(TreeViewListStore), &group_iter,
                       FRIENDUID_COL, &groupid,
                       -1);
    if (groupid != 1)
    {
        do
        {
            gtk_tree_model_iter_next((GtkTreeModel *) TreeViewListStore, &group_iter);
            gtk_tree_model_get(GTK_TREE_MODEL(TreeViewListStore), &group_iter,
                               FRIENDUID_COL, &groupid,
                               -1);
        }
        while (!(groupid == 1));
    }
    gtk_tree_model_iter_children((GtkTreeModel *) TreeViewListStore, &useriter, &group_iter);
    gtk_tree_store_set(TreeViewListStore, &useriter,
                       PIXBUF_COL, pixbuf,
                       PRIORITY_COL, 1,
                       -1);
    return 0;
}

int shibaiinfo(void *reason)
{
    Popup("莫默告诉你：", reason);
    free(reason);
    return 0;
}

int shibaiinfo2(void *reason2)
{
    Popup("莫默告诉你：", reason2);
    free(reason2);
    return 0;
}

int infoupdate(CRPBaseHeader *header, void *data)//资料更新（文本资料）处理函数
{
    switch (header->packetID)
    {
        case CRP_PACKET_OK:
        {
            g_idle_add(okinfo, NULL);
            log_info("资料更新", "%s", CurrentUserInfo->nickName);
            return 0;
        };
        case CRP_PACKET_FAILURE:
        {
            CRPPacketFailure *infodata = CRPFailureCast(header);
            log_info("FAILURe reason", infodata->reason);
            char *failreason = (char *) malloc(strlen(infodata->reason));
            memcpy(failreason, infodata->reason, strlen(infodata->reason - 1));
            g_idle_add(shibaiinfo, failreason);
            if ((void *) infodata != header->data)
            {
                free(infodata);
            }
            break;
        };
    }
    return 0;
}

int dealwithheadphoto(void *data)
{
    //memcpy(CurrentUserInfo, &weinfo, sizeof(weinfo));
    char userhead[80] = {0};
    static cairo_t *cr;
    cairo_surface_t *surface, *surfacehead2;
    HexadecimalConversion(userhead, CurrentUserInfo->icon);
    //加载一个图片
    surface = cairo_image_surface_create_from_png(userhead);
    int w = cairo_image_surface_get_width(surface);
    int h = cairo_image_surface_get_height(surface);
    //创建画布
    surfacehead2 = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 125, 125);
    //创建画笔
    cr = cairo_create(surfacehead2);
    //缩放
    cairo_arc(cr, 60, 60, 60, 0, M_PI * 2);
    cairo_clip(cr);
    cairo_scale(cr, 125.0 / w, 126.0 / h);
    //把画笔和图片相结合。
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    gtk_image_set_from_surface((GtkImage *) HeadX, surfacehead2);//更新主界面头像

    //更新主界面分组下用户的头像和昵称
    GdkPixbuf *pixbuf;
    pixbuf = DrawFriend(CurrentUserInfo, 1);
    GtkTreeIter group_iter, useriter;
    uint32_t groupid = 0;

    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(TreeViewListStore), &group_iter);
    gtk_tree_model_get(GTK_TREE_MODEL(TreeViewListStore), &group_iter,
                       FRIENDUID_COL, &groupid,
                       -1);
    if (groupid != 1)
    {
        do
        {
            gtk_tree_model_iter_next((GtkTreeModel *) TreeViewListStore, &group_iter);
            gtk_tree_model_get(GTK_TREE_MODEL(TreeViewListStore), &group_iter,
                               FRIENDUID_COL, &groupid,
                               -1);
        }
        while (!(groupid == 1));
    }
    gtk_tree_model_iter_children((GtkTreeModel *) TreeViewListStore, &useriter, &group_iter);
    gtk_tree_store_set(TreeViewListStore, &useriter,
                       PIXBUF_COL, pixbuf,
                       PRIORITY_COL, 1,
                       -1);

    /**********保存修改后的头像至本地***********/
    typedef struct cunchu
    {
        char cunchu_name[40];
        char cunchu_pwd[16];
        uint32_t cunchu_uid;
        char cunchu_lujing[16];
    };
    struct cunchu str_cunchu[20];
    char mulu_username[80] = "", mulu_benji[80] = "";
    FILE *passwdfp;
    int i = 0, y = 0;

//从本地读取账号记录
    sprintf(mulu_benji, "%s/.momo", getpwuid(getuid())->pw_dir);//获取本机主目录
    mkdir(mulu_benji, 0700);
    sprintf(mulu_username, "%s/username", mulu_benji);
    // 读取并用结构体数组存储
    if ((passwdfp = fopen(mulu_username, "r")) != NULL)
    {
        y = fread(str_cunchu, sizeof(struct cunchu), 20, passwdfp);
        fclose(passwdfp);
    }
    //重新写入头像路径
    for (i = 0; i < y; ++i)
    {
        if (CurrentUserInfo->uid == str_cunchu[i].cunchu_uid)//在数组中查找
        {
            memcpy(str_cunchu[i].cunchu_lujing, CurrentUserInfo->icon, 16);
            int fd = open(mulu_username, O_RDWR);
            if (fd == -1)
            {
                log_error("User", "Cannot read user friends file %s.\n", mulu_username);
                break;
            }
            lseek(fd, 0, SEEK_SET);
            write(fd, str_cunchu, sizeof(struct cunchu) * y);
            close(fd);
            break;
        }
    }
/*****************保存头像END*****************/
    return 0;
}

int infoheadphoto(CRPBaseHeader *header, void *data)//资料更新(换头像)处理函数
{
    switch (header->packetID)
    {
        case CRP_PACKET_OK:
        {
            log_info("资料更新", "%s", CurrentUserInfo->nickName);
            g_idle_add(dealwithheadphoto, NULL);
            return 0;
        };
        case CRP_PACKET_FAILURE:
        {
            CRPPacketFailure *infodata = CRPFailureCast(header);
            log_info("FAILURe reason", infodata->reason);
            char *failreason = (char *) malloc(strlen(infodata->reason));
            memcpy(failreason, infodata->reason, strlen(infodata->reason - 1));
            g_idle_add(shibaiinfo2, failreason);
            if ((void *) infodata != header->data)
            {
                free(infodata);
            }
            break;
        };
    }
    return 0;
}

/*城市对应相应的省份*/
int sheng_change_city()
{
    const gchar *buf;
    int shengfen = 0;
    buf = gtk_combo_box_text_get_active_text((GtkComboBoxText *) IProvinces);
    for (int i = 0; i < 35; ++i)
    {
        if (strcmp(provinces[i], buf) == 0)
        {
            shengfen = i;
            break;
        }
    }
    gtk_combo_box_text_remove_all((GtkComboBoxText *) ICity);//清除所有城市
    for (int j = 0; j < 41; ++j)
    {
        gtk_combo_box_text_append((GtkComboBoxText *) ICity, NULL, allcity[shengfen][j]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(ICity), 0);//设置默认城市
    return 0;
}

/*日历按钮选择后实时变化事件
并且匹配相应的星座*/
int calendar_event()
{
    gtk_calendar_get_date(GTK_CALENDAR(ICalendar), &year, &month, &day);/*取得选择的年月日*/
    char *buf = (char *) malloc(12);
    int x = month + 1;//日历获取月份是0～11
    sprintf(buf, "%d-%d-%d", year, x, day);
    gtk_button_set_label((GtkButton *) IBirthday, buf);//双击选择日期后更换按钮显示
    free(buf);

    int y = day / 21;
    for (int i = 0; i < 12; ++i)
    {
        if (strcmp(constellations[i], xingzuo[month][y]) == 0)
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(IConstellation), i);
            break;
        }
    }
    gtk_widget_destroy(ICalendar);
    RiliFlag = 0;
    return 0;
}

/*将字符串型的生日信息转化成日历上展示*/
int calendar_change_birthday()
{
    if (RiliFlag == 0)
    {
        ICalendar = gtk_calendar_new();//创建日历并显示未修改前的生日日期
        if (strlen(CurrentUserInfo->birthday) != 0)
        {
            char nian[4] = {0}, yue[2] = {0}, ri[2] = {0};
            strncpy(nian, CurrentUserInfo->birthday, 4);
            strncpy(yue, CurrentUserInfo->birthday + 5, 2);
            if (yue[1] == '-')
            {
                yue[1] = 0;
            }
            if (CurrentUserInfo->birthday[6] != '-')
            {
                ri[0] = CurrentUserInfo->birthday[8];
                ri[1] = CurrentUserInfo->birthday[9];
            }
            else
            {
                ri[0] = CurrentUserInfo->birthday[7];
                ri[1] = CurrentUserInfo->birthday[8];
            }
            if (ri[1] == '-')
            {
                ri[1] = 0;
            }
            log_info("年年", "%s%s%s", nian, yue, ri);
            int i = 0;
            i = atoi(yue) - 1;
            gtk_calendar_select_month(GTK_CALENDAR(ICalendar), (guint) i, (guint) atoi(nian));
            gtk_calendar_select_day(GTK_CALENDAR(ICalendar), (guint) atoi(ri));
        }
        else
        {
            //没有生日信息则显示2011-11-11
            gtk_calendar_select_month(GTK_CALENDAR(ICalendar), 10, 2011);
            gtk_calendar_select_day(GTK_CALENDAR(ICalendar), 11);
        }
        gtk_fixed_put(GTK_FIXED(InfoLayout), ICalendar, 140, 263);
        gtk_widget_show(ICalendar);
        g_signal_connect(ICalendar, "day-selected-double-click", G_CALLBACK(calendar_event), NULL);
        RiliFlag = 1;
    }
    else
    {
        gtk_widget_destroy(ICalendar);
        RiliFlag = 0;
    }
    return 0;
}

/*处理服务器回复的消息
收到ok包将图片文件发给服务器*/
int dealwith_photo(CRPBaseHeader *header, void *data)
{
    struct HeadPictureData *photo_message = (struct HeadPictureData *) data;
    char *imagedata = (char *) malloc(4096);
    size_t num;
    int ret = 1;
    if (header->packetID == CRP_PACKET_FAILURE)
    {
        CRPPacketFailure *infodata = CRPFailureCast(header);
        log_info("FAILURE reason", infodata->reason);
        if ((void *) infodata != header->data)
        {
            free(infodata);
        }
        fclose(photo_message->fp);
        free(photo_message);
        free(imagedata);
        return 0;
    }
    log_info("Message", "Packet id :%d,SessionID:%d\n", header->packetID, header->sessionID);

    if (header->packetID == CRP_PACKET_OK)
    {
        if (photo_message->fp != NULL)
        {
            num = fread(imagedata, sizeof(char), 4096, photo_message->fp);
            log_info("头像2", "%d", num);
            if (num > 0)
            {
                CRPFileDataSend(sockfd, header->sessionID, num, photo_message->seq, imagedata);
                photo_message->seq++;
            }
            else
            {
                fclose(photo_message->fp);
                photo_message->fp = NULL;
                CRPFileDataEndSend(sockfd, header->sessionID, FEC_OK);
            }
        }
        else
        {
            session_id_t newinfoid = CountSessionId();
            memcpy(weinfo.icon, photo_message->code, 16);
            AddMessageNode(newinfoid, infoheadphoto, NULL);
            CRPInfoDataSend(sockfd, newinfoid, 0, &weinfo);
            memcpy(CurrentUserInfo->icon, photo_message->code, 16);
            free(photo_message);
        }
    }
    else if (header->packetID == CRP_PACKET_FILE_DATA_END)
    {
        session_id_t newinfoid = CountSessionId();
        memcpy(weinfo.icon, photo_message->code, 16);
        AddMessageNode(newinfoid, infoheadphoto, NULL);
        CRPInfoDataSend(sockfd, newinfoid, 0, &weinfo);
        memcpy(CurrentUserInfo->icon, photo_message->code, 16);
        fclose(photo_message->fp);
        free(photo_message);
        ret = 0;
    }
    free(imagedata);
    return ret;
}

//对获取的图片名进行处理
int new_head(const char *filename)
{
    session_id_t session_id;
    struct HeadPictureData *mynewhead = (struct HeadPictureData *) malloc(sizeof(struct HeadPictureData));
    struct stat stat_buf;
    unsigned char *codetext = (unsigned char *) malloc(16);
    char finalfile[512] = {0};
    Md5Coding(filename, codetext);//将文件名转化成16位字符码
    HexadecimalConversion(finalfile, codetext);
    CopyFile(filename, finalfile);//拷贝文件
    stat(finalfile, &stat_buf);//获取文件状态
    session_id = CountSessionId();//生成唯一的SessionId
    //初始化结构体
    mynewhead->seq = 0;
    mynewhead->fp = (fopen(finalfile, "r"));
    mynewhead->uid = CurrentUserInfo->uid;
    mynewhead->code = (unsigned char *) malloc(16);
    memcpy(mynewhead->code, codetext, 16);
    AddMessageNode(session_id, dealwith_photo, mynewhead);
    CRPFileStoreRequestSend(sockfd, session_id, (size_t) stat_buf.st_size, 0, codetext);
    free(codetext);
    return 0;
}


static void create_infofaces()
{
    Surfaceback = ChangeThem_png("资料2.png");
    Surfacesave = ChangeThem_png("保存.png");
    Surfacesave1 = ChangeThem_png("保存2.png");
    Surfacecancel = ChangeThem_png("资料取消.png");
    Surfacecancel1 = ChangeThem_png("资料取消2.png");
    Surfaceend = ChangeThem_png("关闭按钮1.png");
    Surfaceend1 = ChangeThem_png("关闭按钮2.png");
    Surfaceend2 = ChangeThem_png("关闭按钮3.png");

    InfoBackground = gtk_image_new_from_surface(Surfaceback);
    InfoSave = gtk_image_new_from_surface(Surfacesave);
    InfoCancel = gtk_image_new_from_surface(Surfacecancel);
    InfoGuanbi = gtk_image_new_from_surface(Surfaceend);
    gtk_widget_set_size_request(GTK_WIDGET(InfoBackground), 550, 488);
}

//保存按钮后，保存用户更改的信息
int infosockfd()
{
    if (filename)
    {
        new_head(filename);
    }
    weinfo = *CurrentUserInfo;
    const gchar *buf;

    buf = gtk_entry_get_text(GTK_ENTRY(IName));
    memset(weinfo.name, 0, strlen(weinfo.name));
    memcpy(weinfo.name, buf, strlen(buf));

    buf = gtk_entry_get_text(GTK_ENTRY(ITel));
    memset(weinfo.tel, 0, strlen(weinfo.tel));
    memcpy(weinfo.tel, buf, strlen(buf));

    buf = gtk_entry_get_text(GTK_ENTRY(IMail));
    memset(weinfo.mail, 0, strlen(weinfo.mail));
    memcpy(weinfo.mail, buf, strlen(buf));

    buf = gtk_entry_get_text(GTK_ENTRY(IMotto));
    memset(weinfo.motto, 0, strlen(weinfo.motto));
    memcpy(weinfo.motto, buf, strlen(buf));

    buf = gtk_combo_box_text_get_active_text((GtkComboBoxText *) ISex);
    if (strcmp(buf, "男") == 0)
    {
        weinfo.sex = 1;
    }
    else
    {
        weinfo.sex = 0;
    }

    buf = gtk_combo_box_text_get_active_text((GtkComboBoxText *) IProvinces);
    int weizhi = 0;
    for (int i = 0; i < 35; ++i)
    {
        if (strcmp(provinces[i], buf) == 0)
        {
            memset(weinfo.provinces, 0, strlen(weinfo.provinces));
            memcpy(weinfo.provinces, buf, strlen(buf));
            weizhi = i;
            break;
        }
    }

    buf = gtk_combo_box_text_get_active_text((GtkComboBoxText *) ICity);
    memset(weinfo.city, 0, strlen(weinfo.city));
    memcpy(weinfo.city, allcity[weizhi][0], strlen(allcity[weizhi][0]));
    for (int j = 0; allcity[weizhi][j]; j++)
    {
        if (strcmp(allcity[weizhi][j], buf) == 0)
        {
            memset(weinfo.city, 0, strlen(weinfo.city));
            memcpy(weinfo.city, buf, strlen(buf));
            break;
        }
    }

    gtk_calendar_get_date(GTK_CALENDAR(ICalendar), &year, &month, &day);//取得选择的年月日
    gtk_button_get_label((GtkButton *) IBirthday);
    int x = month + 1;
    int y = day / 21;
    sprintf(buf, "%d-%d-%d", year, x, day);
    memset(weinfo.birthday, 0, strlen(weinfo.birthday));
    memcpy(weinfo.birthday, buf, strlen(buf));
    //buf = gtk_combo_box_text_get_active_text((GtkComboBoxText *) IConstellation);
    for (int i = 0; i < 12; ++i)
    {
        if (strcmp(constellations[i], xingzuo[month][y]) == 0)
        {
            weinfo.constellation = i;
            break;
        }
    }
    log_info("获取的日期", "%c", weinfo.constellation);
    //log_info("获取的日期", "Year:%d Month:%d Day:%d DATE:%s", year, month, day, buf);
    buf = gtk_entry_get_text(GTK_ENTRY(INickname));
    if (strlen(buf) == 0)
    {
        Popup("莫默告诉你", "给自己起个昵称");
    }
    else
    {
        memset(weinfo.nickName, 0, strlen(weinfo.nickName));
        memcpy(weinfo.nickName, buf, strlen(buf));

        session_id_t newinfoid = CountSessionId();
        AddMessageNode(newinfoid, infoupdate, NULL);
        CRPInfoDataSend(sockfd, newinfoid, 0, &weinfo);
        destroy_infosurfaces();
        gtk_widget_destroy(InfoWind);
        MarkUpdateInfo = 0;
    }
    //memcpy(CurrentUserInfo, &weinfo, sizeof(weinfo));
    return 0;
}

//背景的eventbox拖曳窗口
static gint Infobackg_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_ARROW));
    if (event->button == 1)
    { //gtk_widget_get_toplevel 返回顶层窗口 就是window.
        gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)), event->button,
                                   event->x_root, event->y_root, event->time);
        gtk_widget_destroy(ICalendar);
        RiliFlag = 0;
    }
    return 0;
}

//更新
//鼠标点击事件
static gint save_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

    if (event->button == 1)
    {
        gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
    }
    return 0;
}

//更新
//鼠标抬起事件
static gint save_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)
    {
        infosockfd();
    }
    return 0;
}

//更新
//鼠标移动事件
static gint save_enter_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));
    gtk_image_set_from_surface((GtkImage *) InfoSave, Surfacesave1); //置换图标
    return 0;
}

//更新
//鼠标离开事件
static gint save_leave_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_ARROW));
    gtk_image_set_from_surface((GtkImage *) InfoSave, Surfacesave);
    return 0;
}

//取消
//鼠标点击事件
static gint cancel_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

    if (event->button == 1)
    {
        gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
    }
    return 0;
}

//取消
//鼠标抬起事件
static gint cancel_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{

    if (event->button == 1)
    {
        free(filename);
        destroy_infosurfaces();
        gtk_widget_destroy(InfoWind);
        MarkUpdateInfo = 0;
    }
    return 0;
}

//取消
//鼠标移动事件
static gint cancel_enter_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));
    gtk_image_set_from_surface((GtkImage *) InfoCancel, Surfacecancel1); //置换图标
    return 0;
}

//取消
//鼠标离开事件
static gint cancel_leave_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_ARROW));
    gtk_image_set_from_surface((GtkImage *) InfoCancel, Surfacecancel);

    return 0;
}

//关闭
//鼠标点击事件
static gint guanxx_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)
    {
        gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
        gtk_image_set_from_surface((GtkImage *) InfoGuanbi, Surfaceend1); //置换图标
    }
    return 0;
}

//关闭
//鼠标抬起事件
static gint guanxx_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)
    {
        destroy_infosurfaces();
        gtk_widget_destroy(InfoWind);
        MarkUpdateInfo = 0;
    }
    return 0;
}

//关闭
//鼠标移动事件
static gint guanxx_enter_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));
    gtk_image_set_from_surface((GtkImage *) InfoGuanbi, Surfaceend2);
    return 0;
}

//关闭
//鼠标离开事件
static gint guanxx_leave_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_ARROW));
    gtk_image_set_from_surface((GtkImage *) InfoGuanbi, Surfaceend);
    return 0;
}

//头像
//鼠标点击事件
static gint touxiang_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)
    {
        gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
    }
    return 0;
}

//鼠标抬起事件
static gint touxiang_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == 1)   // 判断是否是点击关闭图标
    {
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new("请选择PNG格式的图片作为头像",
                                             (GtkWindow *) InfoWind,
                                             GTK_FILE_CHOOSER_ACTION_OPEN,
                                             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                             GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                             NULL);
        while (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
        {
            filename = (char *) malloc(256);
            //获取到图片名
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            FILE *fp = fopen(filename, "rb");
            if (fp == NULL)
            {
                perror("打开失败\n");
                return -1;
            }
            else
            {//PNG文件，其文件偏移第1个字节处（以0起始）为PNG。
                char buf[11] = "";
                fread(buf, sizeof(buf), 1, fp);
                if (strncmp("PNG", buf + 1, strlen("PNG")) != 0)
                {
                    Popup("莫默告诉你：", "请选择png格式的图片");
                    //gtk_widget_destroy(dialog);
                }
                else
                {
                    gtk_widget_destroy(dialog);
                    /*选择的图片更换到编辑界面的头像框内,以便用户选择*/
                    static cairo_t *cr;
                    cairo_surface_t *surface;
                    //加载一个图片
                    surface = cairo_image_surface_create_from_png(filename);
                    int w = cairo_image_surface_get_width(surface);
                    int h = cairo_image_surface_get_height(surface);

                    //创建画布
                    SurfaceHead = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 125, 125);
                    //创建画笔
                    cr = cairo_create(SurfaceHead);
                    //缩放
                    cairo_arc(cr, 60, 60, 60, 0, M_PI * 2);
                    cairo_clip(cr);
                    cairo_scale(cr, 125.0 / w, 126.0 / h);
                    //把画笔和图片相结合。
                    cairo_set_source_surface(cr, surface, 0, 0);
                    cairo_paint(cr);
                    gtk_image_set_from_surface((GtkImage *) HeadIcon, SurfaceHead);
                    cairo_destroy(cr);
                }
            }
            fclose(fp);
        }
        gtk_widget_destroy(dialog);
    }
    return 0;
}

//鼠标移动事件
static gint touxiang_enter_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_HAND2));  //设置鼠标光标
    return 0;
}

//鼠标离开事件
static gint touxiang_leave_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    gdk_window_set_cursor(gtk_widget_get_window(InfoWind), gdk_cursor_new(GDK_ARROW));
    return 0;
}

/*资料编辑的控件,跟资料展示不同的是可编辑性*/
void infotv()
{
    char idstring[80] = {0};//id
    sprintf(idstring, "%d", CurrentUserInfo->uid);
    IId = gtk_label_new(idstring);
    gtk_fixed_put(GTK_FIXED(InfoLayout), IId, 255, 29);
    memset(idstring, 0, strlen(idstring));

    int mylevel = CurrentUserInfo->level;
    mylevel = log10(mylevel) / log10(2) + 1;//等级计算
    sprintf(idstring, "%d", mylevel);//等级
    ILevel = gtk_label_new(idstring);
    gtk_fixed_put(GTK_FIXED(InfoLayout), ILevel, 270, 70);

    INickname = gtk_entry_new();//昵称
    gtk_entry_set_max_length((GtkEntry *) INickname, 8);
    gtk_entry_set_has_frame((GtkEntry *) INickname, FALSE);
    gtk_entry_set_text((GtkEntry *) INickname, CurrentUserInfo->nickName);
    gtk_fixed_put(GTK_FIXED(InfoLayout), INickname, 58, 165);

    IName = gtk_entry_new();//姓名
    gtk_entry_set_max_length((GtkEntry *) IName, 8);
    gtk_entry_set_has_frame((GtkEntry *) IName, FALSE);
    gtk_entry_set_text((GtkEntry *) IName, CurrentUserInfo->name);
    gtk_fixed_put(GTK_FIXED(InfoLayout), IName, 48, 225);

    ITel = gtk_entry_new();//电话
    gtk_entry_set_max_length((GtkEntry *) ITel, 11);
    gtk_entry_set_has_frame((GtkEntry *) ITel, FALSE);
    gtk_entry_set_text((GtkEntry *) ITel, CurrentUserInfo->tel);
    gtk_fixed_put(GTK_FIXED(InfoLayout), ITel, 48, 358);

    IMail = gtk_entry_new();//邮箱
    gtk_entry_set_max_length((GtkEntry *) IMail, 30);
    gtk_entry_set_has_frame((GtkEntry *) IMail, FALSE);
    gtk_entry_set_text((GtkEntry *) IMail, CurrentUserInfo->mail);
    gtk_fixed_put(GTK_FIXED(InfoLayout), IMail, 305, 358);

    IMotto = gtk_entry_new();//个人说明
    gtk_widget_set_size_request(IMotto, 300, 10);
    gtk_entry_set_max_length((GtkEntry *) IMotto, 256);
    gtk_entry_set_has_frame((GtkEntry *) IMotto, FALSE);
    gtk_entry_set_text((GtkEntry *) IMotto, CurrentUserInfo->motto);
    gtk_fixed_put(GTK_FIXED(InfoLayout), IMotto, 75, 390);

    if (strlen(CurrentUserInfo->birthday) == 0)
    {
        IBirthday = gtk_button_new_with_label("2011-11-11");
    }
    else
    {
        IBirthday = gtk_button_new_with_label(CurrentUserInfo->birthday);//生日
    }
    gtk_fixed_put(GTK_FIXED(InfoLayout), IBirthday, 48, 260);
    g_signal_connect(IBirthday, "clicked", G_CALLBACK(calendar_change_birthday), NULL);

    ISex = gtk_combo_box_text_new();//性别
    gtk_combo_box_text_append((GtkComboBoxText *) ISex, "0", "女");
    gtk_combo_box_text_append((GtkComboBoxText *) ISex, "1", "男");
    if (1 == CurrentUserInfo->sex)
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(ISex), 1);
    }
    else
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(ISex), 0);
    }
    gtk_fixed_put(GTK_FIXED(InfoLayout), ISex, 305, 216);
    gtk_widget_show(ISex);

    IConstellation = gtk_combo_box_text_new();//星座
    gtk_combo_box_set_wrap_width((GtkComboBox *) IConstellation, 4);
    //gtkcomboboxset
    for (int i = 0; i < 12; ++i)
    {
        gtk_combo_box_text_append((GtkComboBoxText *) IConstellation, NULL, constellations[i]);
    }
    for (int i = 0; i < 12; ++i)
    {
        if (i == CurrentUserInfo->constellation)
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(IConstellation), i);
            break;
        }
    }
    gtk_fixed_put(GTK_FIXED(InfoLayout), IConstellation, 305, 255);
    gtk_widget_show(IConstellation);

    IProvinces = gtk_combo_box_text_new();//省份
    ICity = gtk_combo_box_text_new();//城市
    g_signal_connect(IProvinces, "changed", G_CALLBACK(sheng_change_city), NULL);
    int weizhi = 0;
    for (int i = 0; i < 35; ++i)
    {
        gtk_combo_box_text_append((GtkComboBoxText *) IProvinces, NULL, provinces[i]);
    }
    for (int i = 0; i < 35; ++i)
    {
        if (strcmp(provinces[i], CurrentUserInfo->provinces) == 0)
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(IProvinces), i);
            weizhi = i;
            break;
        }
        else
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(IProvinces), 0);
        }
    }
    for (int j = 0; allcity[weizhi][j]; ++j)
    {
        if (strcmp(allcity[weizhi][j], CurrentUserInfo->city) == 0)
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(ICity), j);
            break;
        }
        else
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(ICity), 0);
        }
    }
    gtk_fixed_put(GTK_FIXED(InfoLayout), IProvinces, 48, 293);
    gtk_fixed_put(GTK_FIXED(InfoLayout), ICity, 305, 295);
    gtk_widget_show(IProvinces);
    gtk_widget_show(ICity);

    char infohead[80] = {0};
    static cairo_t *cr;
    cairo_surface_t *surface;
    HexadecimalConversion(infohead, CurrentUserInfo->icon);
    //加载一个图片
    surface = cairo_image_surface_create_from_png(infohead);
    int w = cairo_image_surface_get_width(surface);
    int h = cairo_image_surface_get_height(surface);

    //创建画布
    SurfaceHead = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 125, 125);
    //创建画笔
    cr = cairo_create(SurfaceHead);
    //缩放
    cairo_arc(cr, 60, 60, 60, 0, M_PI * 2);
    cairo_clip(cr);
    cairo_scale(cr, 125.0 / w, 126.0 / h);
    //把画笔和图片相结合。
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    HeadIcon = gtk_image_new_from_surface(SurfaceHead);
    cairo_destroy(cr);

    BianJi = gtk_label_new("编 辑");
    //gtk_fixed_put(GTK_FIXED(InfoLayout), HeadIcon, 23, 16);
    //gtk_fixed_put(GTK_FIXED(InfoLayout), BianJi, 70, 110);
}

/*资料编辑所需函数
返回值为int型,无须参数*/
int ChangeInfo()
{
    static GtkEventBox *Infobackg_event_box, *Save_event_box, *Cancel_event_box, *Guanxx_event_box;
    static GtkEventBox *touxiang_event_box;
    InfoWind = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(InfoWind), GTK_WIN_POS_CENTER);//窗口位置
    gtk_window_set_resizable(GTK_WINDOW(InfoWind), FALSE);//固定窗口大小
    gtk_window_set_decorated(GTK_WINDOW(InfoWind), FALSE);//去掉边框
    gtk_widget_set_size_request(GTK_WIDGET(InfoWind), 550, 488);

    InfoLayout = gtk_fixed_new();
    create_infofaces();
    gtk_container_add(GTK_CONTAINER(InfoWind), InfoLayout);

    Infobackg_event_box = BuildEventBox(InfoBackground,
                                        G_CALLBACK(Infobackg_button_press_event),
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL);
    gtk_fixed_put(GTK_FIXED(InfoLayout), (GtkWidget *) Infobackg_event_box, 0, 0);

    Save_event_box = BuildEventBox(InfoSave,
                                   G_CALLBACK(save_button_press_event),
                                   G_CALLBACK(save_enter_notify_event),
                                   G_CALLBACK(save_leave_notify_event),
                                   G_CALLBACK(save_button_release_event),
                                   NULL,
                                   NULL);
    gtk_fixed_put(GTK_FIXED(InfoLayout), (GtkWidget *) Save_event_box, 350, 440);

    Cancel_event_box = BuildEventBox(InfoCancel,
                                     G_CALLBACK(cancel_button_press_event),
                                     G_CALLBACK(cancel_enter_notify_event),
                                     G_CALLBACK(cancel_leave_notify_event),
                                     G_CALLBACK(cancel_button_release_event),
                                     NULL,
                                     NULL);
    gtk_fixed_put(GTK_FIXED(InfoLayout), (GtkWidget *) Cancel_event_box, 450, 440);

    Guanxx_event_box = BuildEventBox(InfoGuanbi,
                                     G_CALLBACK(guanxx_button_press_event),
                                     G_CALLBACK(guanxx_enter_notify_event),
                                     G_CALLBACK(guanxx_leave_notify_event),
                                     G_CALLBACK(guanxx_button_release_event),
                                     NULL,
                                     NULL);
    gtk_fixed_put(GTK_FIXED(InfoLayout), (GtkWidget *) Guanxx_event_box, 509, 0);

    infotv();

    touxiang_event_box = BuildEventBox(HeadIcon,
                                       G_CALLBACK(touxiang_button_press_event),
                                       G_CALLBACK(touxiang_enter_notify_event),
                                       G_CALLBACK(touxiang_leave_notify_event),
                                       G_CALLBACK(touxiang_button_release_event),
                                       NULL,
                                       NULL);
    gtk_fixed_put(GTK_FIXED(InfoLayout), (GtkWidget *) touxiang_event_box, 25, 15);
    gtk_fixed_put(GTK_FIXED(InfoLayout), BianJi, 70, 110);

    gtk_widget_show_all(InfoWind);
    return 0;
}