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

#define SCR_ROW 25             /*��Ļ����*/
#define SCR_COL 80             /*��Ļ����*/





//����

typedef struct Car
{
	char Name[15];//��������
	char Type[15];//��������
	char Num[15];//���ƺ�
	char Phone[15];//��������

	struct Car *next;
}CAR;


//�����ṹ������
#define MAXSIZE 100
typedef int Position;
typedef CAR ElementType;
typedef struct LNode* PtrToNode;
struct LNode{
	ElementType Data[MAXSIZE];
	Position Last;
};
typedef PtrToNode List;



//��������Ľ���
#define MaxDigit 6
#define Radix 91

//Ͱ���Ԫ��
typedef struct Node* PtrToNode1;
struct Node{
	ElementType key;
	PtrToNode1 next;
};

//Ͱ���ڵ�ͷ
struct HeadNode{
	PtrToNode1 head,tail;
};
typedef struct HeadNode Bucket[Radix];

//��ȡ�ַ����ĵ�Dλ�ַ�
int GetChar(char X[],int D);




//���ֲ���
#define NotFind -1;//û���ҵ�
BOOL ShowBinarySearch(void);//��ʾ���ֲ��ҽ���
Position BinarySearch(List Tb1,char k[]);



//�ֿ���������(������)
BOOL ShowBlockIndex(void);
Position BlockIndex(List L,char k[]);


/**
 * ��Ļ������Ϣ�����ṹ
 */
typedef struct layer_node {
    char LayerNo;            /*�������ڲ���*/
    SMALL_RECT rcArea;       /*����������������*/
    CHAR_INFO *pContent;     /*�������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;           /*�������������ַ���Ԫԭ����ֵ�洢������*/
    struct layer_node *next; /*ָ����һ����ָ��*/
} LAYER_NODE;

/**
 * ��ǩ���ṹ
 */
typedef struct labe1_bundle {
    char **ppLabel;        /*��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;           /*��ǩ��λ�����׵�ַ*/
    int num;               /*��ǩ����*/
} LABEL_BUNDLE;

/**
 * �����ṹ
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /*������λ�����׵�ַ*/
    char *pSort;           /*�������(�������ı���ѡ���)�����׵�ַ*/
    char *pTag;            /*������������׵�ַ*/
    int num;               /*��������*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;         /*����������Ϣ����ͷ*/

CAR *gp_head1 = NULL;                /*Ժϵ����ͷָ��*/

char *gp_sys_name = "����ʡ���ƹ���ϵͳ";    /*ϵͳ����*/
char *colfile = "col.dat";                  /*Ժϵ������Ϣ�����ļ�*/


char *ga_main_menu[] = { "ʹ��˵��(F)",
                         "����ά��(M)",
                         "���ݲ�ѯ(Q)",
                         "����ͳ��(S)"
};

char *ga_sub_menu[] = { "[S] ���˵���ݼ�˵��",
                        "[B] �Ӳ˵���ݼ�˵��",
                        "[R] ������ݼ�˵��",
                        "[X] �˳�ϵͳ",
                        "[D] ������������Ϣ",
                        "[P] �������ɳ�������",

                        "[D] ��ʾ����������Ϣ",
                        "[P] ���ֲ��ҳ�����Ϣ ",
                        "[C] ���зֿ���������",
                        "[I] ���ڳ��ƺ���Ļ�������",
                        "[T] ģ������",

};

int ga_sub_menu_count[] = { 4, 2, 3, 2 };   /*�����˵������Ӳ˵��ĸ���*/
int gi_sel_menu = 1;                        /*��ѡ�е����˵����,��ʼΪ1*/
int gi_sel_sub_menu = 0;                 /*��ѡ�е��Ӳ˵����,��ʼΪ0,��ʾδѡ��*/

CHAR_INFO *gp_buff_menubar_info = NULL;    /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*���״̬����Ļ���ַ���Ϣ�Ļ�����*/

char *gp_scr_att = NULL;    /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/
char gc_sys_state = '\0';   /*��������ϵͳ״̬���ַ�*/

HANDLE gh_std_out;          /*��׼����豸���*/
HANDLE gh_std_in;           /*��׼�����豸���*/

unsigned long ul;


int CreatList(CAR **pphead1);            //���������ʼ��
void InitInterface(void);                                   //ϵͳ�����ʼ��
void ClearScreen(void);                                     //����
void ShowMenu(void);                                        //��ʾ�˵���
void PopMenu(int num);                                      //��ʾ�����˵�
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *); //����������Ļ��Ϣά��
void PopOff(void);                                          //�رն��㵯������
void DrawBox(SMALL_RECT *parea);                            //���Ʊ߿�
void LocSubMenu(int num, SMALL_RECT *parea);              //���˵������˵���λ
void ShowState(void);                                       //��ʾ״̬��
void TagMainMenu(int num);                               //��Ǳ�ѡ�е����˵���
void TagSubMenu(int num);                                 //��Ǳ�ѡ�е��Ӳ˵���
void SetHotPoint(HOT_AREA *phot_area, int hot_num);         //��������
void RunSys();                                       //ϵͳ����ģ���ѡ�������
BOOL ExeFunction(int main_menu_num, int sub_menu_num);      //����ģ��ĵ���
void CloseSys();                                            //�˳�ϵͳ

BOOL ShowModule(char **pString, int n);

BOOL LoadData(void);             //���ݼ���
BOOL SaveData(void);             //���˵���ݼ�˵��
BOOL BackupData(void);           //�Ӳ˵���ݼ�˵��
BOOL RestoreData(void);          //������ݼ�˵��
BOOL ExitSys(void);              //�˳�ϵͳ

List MakeEmpty();//����һ���յ����Ա�,���������ݶ�������
BOOL ShowAllInfor();//��ʾ�����������Ϣ
void ShowInfor(List L);


BOOL MaintainCarInfo(void);    //ά������������Ϣ
BOOL ShowRadixSorting(void); //��ʾ������Ϣ��������
void RadixSorting(List L);



void freedom(void);           //�ͷ����е��ڴ�

//������ݵ�����
BOOL ShowGenrnd(void);//����������ݵĽ���
void Genrnd();//����ṹ�������
void GenrndNum(char * buff);  //������Ƶ�����
void GenrndPhone(char * buff); //������������
void GenrndName(char * buff); // ����������ֵ�����
void GenrndType(char * buff); //����������͵�����
char *wholename(int a); //���ض�Ӧ����Ŀ����ȫ��

//ģ������
BOOL ShowFuzzySearch(void);//��ʾģ�����ҽ���
void FuzzySearch(List l,char k[]);//ģ�����Һ���
int intain(char *a,char *b); //�ж�a�ǲ���b���Ӵ�

//
int panduan(char str[]);

#endif
