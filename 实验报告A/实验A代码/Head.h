#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define SCR_ROW 25             /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/





//车辆

typedef struct Car
{
	char Name[15];//车主姓名
	char Type[15];//车辆类型
	char Num[15];//车牌号
	char Phone[15];//车主号码

	struct Car *next;
}CAR;


//创建结构体数组
#define MAXSIZE 100
typedef int Position;
typedef CAR ElementType;
typedef struct LNode* PtrToNode;
struct LNode{
	ElementType Data[MAXSIZE];
	Position Last;
};
typedef PtrToNode List;



//基数排序的建立
#define MaxDigit 6
#define Radix 91

//桶结点元素
typedef struct Node* PtrToNode1;
struct Node{
	ElementType key;
	PtrToNode1 next;
};

//桶结点节点头
struct HeadNode{
	PtrToNode1 head,tail;
};
typedef struct HeadNode Bucket[Radix];

//获取字符串的第D位字符
int GetChar(char X[],int D);




//二分查找
#define NotFind -1;//没有找到
BOOL ShowBinarySearch(void);//显示二分查找界面
Position BinarySearch(List Tb1,char k[]);



//分块索引查找(按城市)
BOOL ShowBlockIndex(void);
Position BlockIndex(List L,char k[]);


/**
 * 屏幕窗口信息链结点结构
 */
typedef struct layer_node {
    char LayerNo;            /*弹出窗口层数*/
    SMALL_RECT rcArea;       /*弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /*弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;           /*弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next; /*指向下一结点的指针*/
} LAYER_NODE;

/**
 * 标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;        /*标签字符串数组首地址*/
    COORD *pLoc;           /*标签定位数组首地址*/
    int num;               /*标签个数*/
} LABEL_BUNDLE;

/**
 * 热区结构
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /*热区定位数组首地址*/
    char *pSort;           /*热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /*热区序号数组首地址*/
    int num;               /*热区个数*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;         /*弹出窗口信息链链头*/

CAR *gp_head1 = NULL;                /*院系主链头指针*/

char *gp_sys_name = "辽宁省车牌管理系统";    /*系统名称*/
char *colfile = "col.dat";                  /*院系基本信息数据文件*/


char *ga_main_menu[] = { "使用说明(F)",
                         "数据维护(M)",
                         "数据查询(Q)",
                         "数据统计(S)"
};

char *ga_sub_menu[] = { "[S] 主菜单快捷键说明",
                        "[B] 子菜单快捷键说明",
                        "[R] 其他快捷键说明",
                        "[X] 退出系统",
                        "[D] 处理车辆基本信息",
                        "[P] 批量生成车辆数据",

                        "[D] 显示车辆基本信息",
                        "[P] 二分查找车辆信息 ",
                        "[C] 城市分块索引查找",
                        "[I] 基于车牌号码的基数排序",
                        "[T] 模糊查找",

};

int ga_sub_menu_count[] = { 4, 2, 3, 2 };   /*各主菜单项下子菜单的个数*/
int gi_sel_menu = 1;                        /*被选中的主菜单项号,初始为1*/
int gi_sel_sub_menu = 0;                 /*被选中的子菜单项号,初始为0,表示未选中*/

CHAR_INFO *gp_buff_menubar_info = NULL;    /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/
char gc_sys_state = '\0';   /*用来保存系统状态的字符*/

HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

unsigned long ul;


int CreatList(CAR **pphead1);            //数据链表初始化
void InitInterface(void);                                   //系统界面初始化
void ClearScreen(void);                                     //清屏
void ShowMenu(void);                                        //显示菜单栏
void PopMenu(int num);                                      //显示下拉菜单
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *); //弹出窗口屏幕信息维护
void PopOff(void);                                          //关闭顶层弹出窗口
void DrawBox(SMALL_RECT *parea);                            //绘制边框
void LocSubMenu(int num, SMALL_RECT *parea);              //主菜单下拉菜单定位
void ShowState(void);                                       //显示状态栏
void TagMainMenu(int num);                               //标记被选中的主菜单项
void TagSubMenu(int num);                                 //标记被选中的子菜单项
void SetHotPoint(HOT_AREA *phot_area, int hot_num);         //设置热区
void RunSys();                                       //系统功能模块的选择和运行
BOOL ExeFunction(int main_menu_num, int sub_menu_num);      //功能模块的调用
void CloseSys();                                            //退出系统

BOOL ShowModule(char **pString, int n);

BOOL LoadData(void);             //数据加载
BOOL SaveData(void);             //主菜单快捷键说明
BOOL BackupData(void);           //子菜单快捷键说明
BOOL RestoreData(void);          //其他快捷键说明
BOOL ExitSys(void);              //退出系统

List MakeEmpty();//创建一个空的线性表,将链表数据读入数组
BOOL ShowAllInfor();//显示数组的所有信息
void ShowInfor(List L);


BOOL MaintainCarInfo(void);    //维护车辆基本信息
BOOL ShowRadixSorting(void); //显示车辆信息基数排序
void RadixSorting(List L);



void freedom(void);           //释放所有的内存

//随机数据的生成
BOOL ShowGenrnd(void);//随机生产数据的界面
void Genrnd();//随机结构体的生成
void GenrndNum(char * buff);  //随机车牌的生成
void GenrndPhone(char * buff); //随机号码的生成
void GenrndName(char * buff); // 随机车主名字的生成
void GenrndType(char * buff); //随机汽车类型的生成
char *wholename(int a); //返回对应的项目类型全称

//模糊查找
BOOL ShowFuzzySearch(void);//显示模糊查找界面
void FuzzySearch(List l,char k[]);//模糊查找函数
int intain(char *a,char *b); //判断a是不是b的子串

//
int panduan(char str[]);

#endif
