#include"Head.h"
#pragma warning(disable:4996)


int main(){
    COORD size = { SCR_COL, SCR_ROW };              /*���ڻ�������С*/
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);   /* ��ȡ��׼����豸���*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);     /* ��ȡ��׼�����豸���*/
    SetConsoleTitle(gp_sys_name);                   /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(gh_std_out, size);   /*���ô��ڻ�������С80*25*/
    LoadData();                               /*���ݼ���*/
    InitInterface();                          /*�����ʼ��*/
    RunSys();              /*ϵͳ����ģ���ѡ������*/
    CloseSys();              /*�˳�ϵͳ*/
    system("pause");
    return 0;
}


BOOL LoadData(){
    int Re = 0;
    Re = CreatList(&gp_head1);
    gc_sys_state |= Re;
    gc_sys_state &= ~(1 - Re);
    if (gc_sys_state < 1){
        printf("\nϵͳ�������ݲ�����!\n");
        printf("\n�����������...\n");
        getch();
    }
    return TRUE;
}


int CreatList(CAR **phead1){
	CAR *hd1 = NULL, *pCollege, tmp1;
	FILE *pFile;
    int find;
    int re=0;
    if ((pFile = fopen(colfile, "rb")) == NULL){
        return re;
    }
 	/*�������ļ��ж�Ժϵ��Ϣ���ݣ������Ժ���ȳ���ʽ������������*/
    while (fread(&tmp1, sizeof(CAR), 1, pFile) == 1){
        pCollege = (CAR *)malloc(sizeof(CAR));
        *pCollege = tmp1;
        pCollege->next = hd1;
        hd1 = pCollege;
    }
    fclose(pFile);
    
    if (hd1== NULL){
        return re;
    }
    *phead1 = hd1;
    re += 1;
    return re;


}

/**
 * ��������:��ʼ������
 */
void InitInterface(){
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*��ɫǰ������ɫ����*/
    SetConsoleTextAttribute(gh_std_out, att);  /*���ÿ���̨��Ļ�������ַ�����*/
    ClearScreen();  /* ����*/
    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*�������ڵĲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;  /*�������ڵ�����Ϊ������Ļ����*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*��ʾ�˵���*/
    ShowState();    /*��ʾ״̬��*/
    return;
}

/**
 * ��������:�����Ļ��Ϣ
*/
void ClearScreen(void){
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = { 0, 0 };
    unsigned long size;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*ȡ��Ļ��������Ϣ*/
    size =(unsigned long) bInfo.dwSize.X * bInfo.dwSize.Y; /*������Ļ�������ַ���Ԫ��*/
    /*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);
    /*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
    return;
}


/**
 * ��������:����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��
 */
void ShowMenu(){
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    CONSOLE_CURSOR_INFO lpCur;
    COORD size;
    COORD pos = { 0, 0 };
    int i, j;
    int PosA = 2, PosB;
    char ch;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos);
    /*�ڴ��ڵ�һ�е�һ�д�������˵���*/
    for (i = 0; i < 4; i++) printf("  %s  ", ga_main_menu[i]);
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*���ع��*/
    /*���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu = { 0, 0, (short)(size.X - 1), 0 };
    /*�����ڵ�һ�е����ݶ��뵽��Ų˵�����Ļ���ַ���Ϣ�Ļ�������*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    /*����һ����Ӣ����ĸ��Ϊ��ɫ�������ַ���Ԫ��Ϊ�׵׺���*/
    for (i = 0; i<size.X; i++){
        (gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                 | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
            (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*�޸ĺ�Ĳ˵����ַ���Ϣ��д�����ڵĵ�һ��*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = { 0, 1 };
    SetConsoleCursorPosition(gh_std_out, endPos);  /*�����λ�������ڵ�2�е�1��*/
    /*���˵�����Ϊ�������������Ϊ�˵���ţ���������Ϊ0(��ť��)*/
    i = 0;
    do{
        PosB = PosA + strlen(ga_main_menu[i]);  /*��λ��i+1�Ų˵������ֹλ��*/
        for (j = PosA; j<PosB; j++)
            gp_scr_att[j] |= (i + 1) << 2; /*���ò˵��������ַ���Ԫ������ֵ*/
        PosA = PosB + 4;
        i++;
    }while (i<4);
    TagMainMenu(gi_sel_menu);  /*��ѡ�����˵���������ǣ�gi_sel_menu��ֵΪ1*/
    return;
}

/**
 * ��������:��ʾ״̬��,״̬���ַ�����Ϊ�׵׺���
*/
void ShowState(){
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = { 0, 0 };
    int i;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SMALL_RECT rcMenu = { 0, (short)(bInfo.dwSize.Y - 1),(short) (size.X - 1), (short)(bInfo.dwSize.Y - 1 )};
    if (gp_buff_stateBar_info == NULL){
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }
    for (i = 0; i<size.X; i++){
        (gp_buff_stateBar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                  | BACKGROUND_RED;
    }
    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    return;
}


/**
 * ��������:��ָ�����˵�������ѡ�б�־,�������:num ѡ�е����˵����
*/
void TagMainMenu(int num){
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = { 0, 0 };
    int PosA = 2, PosB;
    char ch;
    int i;
    /*numΪ0ʱ������ȥ�����˵���ѡ�б��*/
    if (num == 0) {
        PosA = 0;
        PosB = 0;
    }
    else{
        /*���򣬶�λѡ�����˵������ֹλ��: PosAΪ��ʼλ��, PosBΪ��ֹλ��*/
        for (i = 1; i<num; i++)  PosA += strlen(ga_main_menu[i - 1]) + 4;
        PosB = PosA + strlen(ga_main_menu[num - 1]);
    }
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    /*ȥ��ѡ�в˵���ǰ��Ĳ˵���ѡ�б��*/
    for (i = 0; i<PosA; i++){
        (gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                 | BACKGROUND_RED;
        ch = (gp_buff_menubar_info + i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
            (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*��ѡ�в˵���������ǣ��ڵװ���*/
    for (i = PosA; i<PosB; i++){
        (gp_buff_menubar_info + i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                                 | FOREGROUND_RED;
    }
    /*ȥ��ѡ�в˵������Ĳ˵���ѡ�б��*/
    for (i = PosB; i<bInfo.dwSize.X; i++){
        (gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                 | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
            (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*�����ñ�ǵĲ˵�����Ϣд�����ڵ�һ��*/
    SMALL_RECT rcMenu = { 0, 0, (short)(size.X - 1), 0 };
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}

/**
 * ��������:�ر�ϵͳ
*/
void CloseSys() {
    freedom();
    ClearScreen();        /*����*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);
    /*�رձ�׼���������豸���*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);
    /*�����ڱ�������Ϊ���н���*/
    SetConsoleTitle("���н���");
    return;
}

/**
 * ��������:����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��
 * �������:phead ����ͷָ��ĵ�ַ
 */
void RunSys(){
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos;
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    while (bRet) {
        /*�ӿ���̨���뻺�����ж�һ����¼*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        /*�����¼������¼�����*/
        if (inRec.EventType == MOUSE_EVENT) {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*��ȡ�������λ��*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*ȡ��λ�õĲ��*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*ȡ���ַ���Ԫ����*/
            /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
            if (cNo == 0) {
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0) {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    InitInterface();
                    PopMenu(cAtt);       /*��������������˵����Ӧ���Ӳ˵�*/
                }
            }
            else   /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
               if (cAtt > 0) TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
            /*������������ߵ�һ��*/
            if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                if (cNo == 0) {
                    /*�����λ�ô�������(���˵����ַ���Ԫ)*/
                    if(cAtt > 0) {
                        InitInterface();
                        PopMenu(cAtt);/*��������������˵����Ӧ���Ӳ˵�*/
                    }
                    else /*�����λ�ò��������˵����ַ���Ԫ�������Ӳ˵�����*/
                        if (gp_top_layer->LayerNo > 0) {
                            PopOff();            /*�رյ������Ӳ˵�*/
                            gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                        }
                }
                else /*��Ų�Ϊ0��������λ�ñ������Ӳ˵�����*/{
                    /*�����λ�ô�������(�Ӳ˵����ַ���Ԫ)*/
                    if (cAtt > 0) {
                        PopOff(); /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                        /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/{
                /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                if (cNo == 0) {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ��������Ҽ�������*/
                 && inRec.Event.KeyEvent.bKeyDown) {
            vkc = (char)inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/
            /*ϵͳ��ݼ��Ĵ���*/
            /*�������F1��*/
            if (vkc == 112) {
                /*�����ǰ���Ӳ˵�����*/
                if (gp_top_layer->LayerNo != 0) {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                bRet = ExeFunction(1, 3);  /*����������ݼ����ܺ���*/
            }
                /*������������Alt��*/
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) {
                switch (vkc) { /*�ж���ϼ�Alt+��ĸ*/
                    case 88:  /*Alt+X �˳�*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1, 4);
                        break;
                    case 70:  /*Alt+F*/
                        InitInterface();
                        PopMenu(1);
                        break;
                    case 77: /*Alt+M*/
                        InitInterface();
                        PopMenu(2);
                        break;
                    case 81: /*Alt+Q*/
                        InitInterface();
                        PopMenu(3);
                        break;
                    case 83: /*Alt+S*/
                        InitInterface();
                        PopMenu(4);
                        break;
                    default:
                        break;
                }
            }
                /*�������Ƽ��Ĵ���*/
            else if (asc == 0) {
                /*���δ�����Ӳ˵�*/
                if (gp_top_layer->LayerNo == 0) {
                    /*�������(���ҡ���)������Ӧ�������Ƽ�*/
                    switch (vkc) {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu == 0) gi_sel_menu = 4;
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu == 5) gi_sel_menu = 1;
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 40:
                            InitInterface();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        default:
                            break;
                    }
                }
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    for (loc = 0, i = 1; i<gi_sel_menu; i++)
                        loc += ga_sub_menu_count[i - 1];
                    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    /*�����(���ҡ��ϡ���)�Ĵ���*/
                    switch (vkc) {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1) gi_sel_menu = 4;
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            InitInterface();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 38:
                            num = gi_sel_sub_menu - 1;
                            if (num < 1) num = ga_sub_menu_count[gi_sel_menu - 1];
                            if (strlen(ga_sub_menu[loc + num - 1]) == 0) num--;
                            TagSubMenu(num);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > 4) gi_sel_menu = 1;
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            InitInterface();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 40:
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu - 1]) num = 1;
                            if (strlen(ga_sub_menu[loc + num - 1]) == 0) num++;
                            TagSubMenu(num);
                            break;
                        default:
                            break;
                    }
                }
            }
                /*������ͨ��*/
            else if ((asc - vkc == 0) || (asc - vkc == 32)) {
                /*���δ�����Ӳ˵�*/
                if (gp_top_layer->LayerNo == 0){
                    switch (vkc) {
                        case 70: /*f��F*/
                            InitInterface();
                            PopMenu(1);
                            break;
                        case 77: /*m��M*/
                            InitInterface();
                            PopMenu(2);
                            break;
                        case 81: /*q��Q*/
                            InitInterface();
                            PopMenu(3);
                            break;
                        case 83: /*s��S*/
                            InitInterface();
                            PopMenu(4);
                            break;
                        case 13: /*�س�*/
                            InitInterface();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        default:
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/{
                    if (vkc == 27) /*�������ESC��*/{
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if (vkc == 13) /*������»س���*/{
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*������ͨ���Ĵ���*/{
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc = 0, i = 1; i<gi_sel_menu; i++)
                            loc += ga_sub_menu_count[i - 1];
                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i = loc; i<loc + ga_sub_menu_count[gi_sel_menu - 1]; i++) {
                            if (strlen(ga_sub_menu[i])>0 && vkc == ga_sub_menu[i][1]) { /*���ƥ��ɹ�*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i - loc + 1);
                            }
                        }
                    }
                }
            }
        }
    }
}


/**
 * ��������:����ָ�����˵����Ӧ���Ӳ˵�
 * �������:num ָ�������˵����
 */
void PopMenu(int num) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;
    /*���ָ�����˵�������ѡ�в˵�*/
    if (num != gi_sel_menu) {
        /*�����ǰ�����Ӳ˵�����*/
        if (gp_top_layer->LayerNo != 0){
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
        /*���ѵ������Ӳ˵����򷵻�*/
    else if (gp_top_layer->LayerNo != 0) return;
    gi_sel_menu = num;    /*��ѡ�����˵�����Ϊָ�������˵���*/
    TagMainMenu(gi_sel_menu); /*��ѡ�е����˵����������*/
    LocSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
    for (i = 1; i<gi_sel_menu; i++) loc += ga_sub_menu_count[i - 1];
    /*�������Ӳ˵������������ǩ���ṹ����*/
    labels.ppLabel = ga_sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    COORD aLoc[labels.num];/*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����*/
    /*ȷ����ǩ�ַ��������λ�ã����������������*/
    for (i = 0; i<labels.num; i++) {
        aLoc[i].X = (short)(rcPop.Left + 2);
        aLoc[i].Y = (short)(rcPop.Top + i + 1);
    }
    labels.pLoc =aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
    /*����������Ϣ*/
    areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    SMALL_RECT aArea[areas.num];                    /*������������������λ��*/
    char aSort[areas.num];                      /*��������������������Ӧ���*/
    char aTag[areas.num];                         /*����������ÿ�������ı��*/
    for (i = 0; i<areas.num; i++) {
        aArea[i].Left =(short)(rcPop.Left + 2);  /*������λ*/
        aArea[i].Top = (short)(rcPop.Top + i + 1);
        aArea[i].Right =(short)(rcPop.Right - 2);
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*�������Ϊ0(��ť��)*/
        aTag[i] =(char)(i + 1);           /*������˳����*/
    }
    areas.pArea = aArea;/*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;/*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag = aTag;   /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*���������ڻ��߿�*/
    pos.X = (short)(rcPop.Left + 2);
    for (pos.Y = (short)(rcPop.Top + 1); pos.Y<rcPop.Bottom; pos.Y++) {
        /*��ѭ�������ڿմ��Ӳ���λ�û����γɷָ�����ȡ���˲˵������������*/
        pCh = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
        if (strlen(pCh) == 0) /*����Ϊ0������Ϊ�մ�*/{   /*���Ȼ�����*/
            FillConsoleOutputCharacter(gh_std_out, '-',(rcPop.Right - rcPop.Left - 3), pos, &ul);
            for (j = rcPop.Left + 2; j<rcPop.Right - 1; j++) {
                /*ȡ���������ַ���Ԫ����������*/
                gp_scr_att[pos.Y*SCR_COL + j] &= 3; /*��λ��Ľ�������˵���λ*/
            }
        }
    }
    /*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
    pos.X = (short)(rcPop.Left + 3);
    att = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y =(short)( rcPop.Top + 1); pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0) {
            continue;  /*�����մ�*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/**
* ��������: ��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.
* �������: pRc ��������λ�����ݴ�ŵĵ�ַ
*          att �������������ַ�����
*          pLabel ���������б�ǩ����Ϣ��ŵĵ�ַ
*          pHotArea ����������������Ϣ��ŵĵ�ַ
*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea) {
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = { 0, 0 };
    char *pCh;
    int i, j, row;
    /*������������λ���ַ���Ԫ��Ϣ��ջ*/
    size.X =(short)( pRc->Right - pRc->Left + 1);    /*�������ڵĿ��*/
    size.Y =(short)( pRc->Bottom - pRc->Top + 1);    /*�������ڵĸ߶�*/
    /*�����ŵ������������Ϣ�Ķ�̬�洢��*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo =(char)(gp_top_layer->LayerNo + 1);
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y * sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y * sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i = pRc->Top; i <= pRc->Bottom; i++) {
        /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
        for (j = pRc->Left; j <= pRc->Right; j++) {
            *pCh = gp_scr_att[i*SCR_COL + j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����*/
    /*���õ������������ַ���������*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i = pRc->Top; i <= pRc->Bottom; i++) {
        FillConsoleOutputAttribute(gh_std_out, att, (DWORD)size.X, pos, &ul);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i = 0; i<pLabel->num; i++) {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0) {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i = pRc->Top; i <= pRc->Bottom; i++) {
        /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j = pRc->Left; j <= pRc->Right; j++)
            gp_scr_att[i*SCR_COL + j] = gp_top_layer->LayerNo;
    }
    for (i = 0; i<pHotArea->num; i++) {
        /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
        row = pHotArea->pArea[i].Top;
        for (j = pHotArea->pArea[i].Left; j <= pHotArea->pArea[i].Right; j++) {
            gp_scr_att[row*SCR_COL + j] |= (pHotArea->pSort[i] << 6)
                                           | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

/**
 * ��������:�رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����
*/
void PopOff(void) {
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = { 0, 0 };
    char *pCh;
    int i, j;
    if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL)) {
        /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*�ָ�������������ԭ���*/
    size.X =(short)( gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1);
    size.Y =(short)( gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1);
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*�ָ��ַ���Ԫԭ����*/
    pCh = gp_top_layer->pScrAtt;
    for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++) {
        for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++) {
            gp_scr_att[i*SCR_COL + j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*�ͷŶ�̬�洢��*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**
* ��������: ��ָ���Ӳ˵�������ѡ�б��.
* �������: num ѡ�е��Ӳ˵����
*/
void TagSubMenu(int num) {
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;
    LocSubMenu(gi_sel_menu, &rcPop);  /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom - rcPop.Top - 1))
        return;
    pos.X = (short)(rcPop.Left + 2);
    width = rcPop.Right - rcPop.Left - 3;
    /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
    if (gi_sel_sub_menu != 0) {
        pos.Y = (short)(rcPop.Top + gi_sel_sub_menu);
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, (DWORD)width, pos, &ul);
        pos.X += 1;
        att |= FOREGROUND_RED;/*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*���ƶ��Ӳ˵�������ѡ�б��*/
    pos.X =(short)( rcPop.Left + 2);
    pos.Y =(short)( rcPop.Top + num);
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    FillConsoleOutputAttribute(gh_std_out, att, (DWORD)width, pos, &ul);
    gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
    return;
}

/**
* ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
* �������: m ���˵����
*          s �Ӳ˵����
* �� �� ֵ: BOOL����, TRUE �� FALSE
* ����˵��: ����ִ�к���ExitSysʱ, �ſ��ܷ���FALSE, ������������Ƿ���TRUE
*/
BOOL ExeFunction(int m, int s) {
    BOOL bRet = TRUE;
    /*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
    int(*pFunction[ga_sub_menu_count[0] + ga_sub_menu_count[1] + ga_sub_menu_count[2] + ga_sub_menu_count[3]])(void);
    int i, loc;
    /*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/
    pFunction[0] = SaveData;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;
    pFunction[4] = MaintainCarInfo;
    pFunction[5] = ShowGenrnd;
    pFunction[6] = ShowAllInfor;
    pFunction[7] = ShowBinarySearch;
    pFunction[8] = ShowBlockIndex;
    pFunction[9] = ShowRadixSorting;
    pFunction[10] = ShowFuzzySearch;

    /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
    for (i = 1, loc = 0; i<m; i++) loc += ga_sub_menu_count[i - 1];
    loc += s - 1;
    if (pFunction[loc] != NULL) bRet = (*pFunction[loc])();
    return bRet;/*�ú���ָ�������ָ��Ĺ��ܺ���*/
}

/**
 * �������ܣ����˵���ݼ�ʹ��˵��
 */
BOOL SaveData(void) {
    BOOL bRet = TRUE;
    InitInterface();
    char *plabel_name[] = { "���˵���ݼ�ʹ��˵��",
                            "Alt+F:ʹ��˵��",
                            "Alt+M:����ά��",
                            "Alt+Q:���ݲ�ѯ",
                            "Alt+S:����ͳ��",
                            "", "", "", "", "", "",
                            "ȷ��"
    };
    ShowModule(plabel_name, 12);
    return bRet;
}

/**
* ��������: ���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��.
* �������: num ѡ�е����˵����
* �������: rc �������λ����Ϣ�ĵ�ַ
*/
void LocSubMenu(int num, SMALL_RECT *rc) {
    int i, len, loc = 0;
    rc->Top = 1; /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
    rc->Left = 1;
    for (i = 1; i<num; i++) {
        /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
        rc->Left += strlen(ga_main_menu[i - 1]) + 4;
        loc += ga_sub_menu_count[i - 1];
    }
    rc->Right =(short)strlen(ga_sub_menu[loc]);/*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
    for (i = 1; i<ga_sub_menu_count[num - 1]; i++) {
        /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
        len = strlen(ga_sub_menu[loc + i]);
        if (rc->Right < len)
            rc->Right = (short)len;
    }
    /*����������ұ߽�*/
    rc->Right += rc->Left + 3;
    /*���������±ߵ��к�*/
    rc->Bottom = (short)(rc->Top + ga_sub_menu_count[num - 1] + 1);
    /*�ұ߽�Խ��Ĵ���*/
    if (rc->Right >= SCR_COL) {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**
* ��������: ��ָ�����򻭱߿�.
* �������: pRc �������λ����Ϣ�ĵ�ַ
*/
void DrawBox(SMALL_RECT *pRc) {
    char chBox[] = { '+','-','|' };  /*�����õ��ַ�*/
    COORD pos = { pRc->Left, pRc->Top };  /*��λ����������Ͻ�*/
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.X =(short)(pRc->Left + 1); pos.X < pRc->Right; pos.X++) {
        /*��ѭ�����ϱ߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.Y = (short)(pRc->Top + 1); pos.Y < pRc->Bottom; pos.Y++) {
        /*��ѭ�����߿�����ߺ��ұ���*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    for (pos.X = (short)(pRc->Left + 1); pos.X < pRc->Right; pos.X++) {
        /*���±߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    return;
}

/**
 * �������ܣ���������˵���˵�
 */
BOOL ShowModule(char **pString, int n) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len;
    for (i = 0, maxlen = 0; i<n; i++) {
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }
    pos.X = (short)(maxlen + 6);
    pos.Y = (short)(n + 5);
    rcPop.Left = (short)((SCR_COL - pos.X) / 2);
    rcPop.Right = (short)(rcPop.Left + pos.X - 1);
    rcPop.Top = (short)((SCR_ROW - pos.Y) / 2);
    rcPop.Bottom = (short)(rcPop.Top + pos.Y - 1);
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];
    for (i = 0; i<n; i++) {
        aLoc[i].X = (short)(rcPop.Left + 3);
        aLoc[i].Y = (short)(rcPop.Top + 2 + i);
    }
    str_len = strlen(pString[n - 1]);
    aLoc[n - 1].X =(short)( rcPop.Left + 3 + (maxlen - str_len) / 2);
    aLoc[n - 1].Y = (short)(aLoc[n - 1].Y + 2);
    labels.pLoc = aLoc;
    areas.num = 1;
    SMALL_RECT aArea[] = { { aLoc[n - 1].X, aLoc[n - 1].Y,
                                   (short)(aLoc[n - 1].X + 3), aLoc[n - 1].Y } };
    char aSort[] = { 0 };
    char aTag[] = { 1 };
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = (short)(rcPop.Left + 1);
    pos.Y = (short)(rcPop.Top + 2 + n);
    FillConsoleOutputCharacter(gh_std_out, '-', (rcPop.Right - rcPop.Left - 1), pos, &ul);
    DealInput(&areas, &iHot);
    PopOff();
    return bRet;
}


/**
* ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
* �������: pHotArea
*          piHot ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
* �������: piHot ����굥�������س���ո�ʱ���ص�ǰ�������
*/
int DealInput(HOT_AREA *pHotArea, int *piHot) {
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = { 0, 0 };
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    SetHotPoint(pHotArea, *piHot);
    while (TRUE) {
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if ((inRec.EventType == MOUSE_EVENT) &&
            (inRec.Event.MouseEvent.dwButtonState
             == FROM_LEFT_1ST_BUTTON_PRESSED)) {
            pos = inRec.Event.MouseEvent.dwMousePosition;
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;
            if ((cNo == gp_top_layer->LayerNo) && cTag > 0) {
                *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0) {
                    iRet = 13;
                    break;
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) {
            vkc = (char)inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
            if (asc == 0) {
                arrow = 0;
                switch (vkc) {  /*�����(���ϡ��ҡ���)�Ĵ���*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 2; break;
                    case 39: arrow = 3; break;
                    case 40: arrow = 4; break;
                    default:
                        break;
                }
                if (arrow > 0) {
                    num = *piHot;
                    while (TRUE) {
                        if (arrow < 3) num--;
                        else num++;
                        if ((num < 1) || (num > pHotArea->num) ||
                            ((arrow % 2) && (pHotArea->pArea[num - 1].Top
                                             == pHotArea->pArea[*piHot - 1].Top)) || ((!(arrow % 2)) && (pHotArea->pArea[num - 1].Top != pHotArea->pArea[*piHot - 1].Top))) {
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num) {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27) {  /*ESC��*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32) {
                /*�س�����ո��ʾ���µ�ǰ��ť*/
                iRet = 13;
                break;
            }
        }
    }
    return iRet;
}

/**
 * �������ܣ����ü�������״̬
 * ���������pHotArea������������
 *         iHot�����������ж���
 */
void SetHotPoint(HOT_AREA *pHotArea, int iHot) {
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = { 0, 0 };
    WORD att1, att2;
    int i, width;
    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    for (i = 0; i<pHotArea->num; i++) {
        /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        /*�����ǰ�ť��*/
        if (pHotArea->pSort[i] == 0)
            FillConsoleOutputAttribute(gh_std_out, att2, (DWORD)width, pos, &ul);
    }
    pos.X = pHotArea->pArea[iHot - 1].Left;
    pos.Y = pHotArea->pArea[iHot - 1].Top;
    width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
    if (pHotArea->pSort[iHot - 1] == 0) {
        /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(gh_std_out, att1, (DWORD)width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot - 1] == 1) {
        /*�������������ı�����*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}


/**
 * �������ܣ��Ӳ˵���ݼ�ʹ��˵��
 */
BOOL BackupData(void) {
    BOOL bRet = TRUE;
    InitInterface();
    char *plabel_name[] = { "              �Ӳ˵���ݼ�ʹ��˵��",
                            "S/s:���˵���ݼ�˵��",
                            "B/b:�Ӳ˵���ݼ�˵��",
                            "R/r:������ݼ�˵��",
                            "D/d:����������Ϣ",
                            "P/p:�����������",
                            "P/p:���ֲ���",
                            "C/c:���зֿ���������",
                            "I/i:���ڳ��ƺ���Ļ�������",
                            "T/t:ģ������",

                            "ȷ��"
    };
    ShowModule(plabel_name, 11);
    return bRet;
}

/**
 * �������ܣ�������ݼ�ʹ��˵��
 */
BOOL RestoreData(void) {
    BOOL bRet = TRUE;
    InitInterface();
    char *plabel_name[] = { "������ݼ�ʹ��˵��",
                            "Alt+X:�˳�ϵͳ",
                            "F1:��ʾ���ֿ�ݼ�ʹ�÷�ʽ",
                            "�����:�����˵�/ѡ���ƶ�",
                            "Enter:�����˵�/���ȷ��",
                            "Esc:�رղ˵�",
                            "", "", "", "", "",
                            "ȷ��"
    };
    ShowModule(plabel_name, 12);
    return bRet;
}



/**
 * �������ܣ��ͷ����е��ڴ�
 */
void freedom(void){
	CAR *pCollege1 = gp_head1, *pCollege2;
	 while(pCollege1!=NULL){
        pCollege2=pCollege1->next;
        free(pCollege1);
        pCollege1=pCollege2;
    }

}

/**
 * �������ܣ��˳�ϵͳ
 */
BOOL ExitSys(void) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = { "ȷ���˳�ϵͳ��", "ȷ��    ȡ��" };
    int iHot = 1;
    pos.X = (short)(strlen(pCh[0]) + 6);
    pos.Y = 7;
    rcPop.Left = (short)((SCR_COL - pos.X) / 2);
    rcPop.Right =(short)( rcPop.Left + pos.X - 1);
    rcPop.Top = (short)((SCR_ROW - pos.Y) / 2);
    rcPop.Bottom = (short)(rcPop.Top + pos.Y - 1);
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {
            {(short)(rcPop.Left + 3),(short)(rcPop.Top + 2)},
            {(short)(rcPop.Left + 5),(short)(rcPop.Top + 5)}
    };
    labels.pLoc = aLoc;
    areas.num = 2;
    SMALL_RECT aArea[] = {
            { (short)(rcPop.Left + 5), (short)(rcPop.Top + 5),
                    (short)(rcPop.Left + 8), (short)(rcPop.Top + 5) },
            { (short)(rcPop.Left + 13), (short)(rcPop.Top + 5),
                    (short)(rcPop.Left + 16), (short)(rcPop.Top + 5)}
    };
    char aSort[] = { 0, 0 };
    char aTag[] = { 1, 2 };
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    pos.X = (short)(rcPop.Left + 1);
    pos.Y = (short)(rcPop.Top + 4);
    FillConsoleOutputCharacter(gh_std_out, '-',
                               (rcPop.Right - rcPop.Left - 1), pos, &ul);
    if (DealInput(&areas, &iHot) == 13 && iHot == 1) bRet = FALSE;
    else bRet = TRUE;
    PopOff();
    return bRet;
}

/**
* �������ܣ�ά������������Ϣ
*/
BOOL MaintainCarInfo(void) {
	BOOL bRet = TRUE;
	CAR *c1 = gp_head1, *c2 = c1, *c3 = NULL, *c4 = NULL, gcol, kcol;
	FILE *fin1, *fin2, *fin3;
	InitInterface();
	int n;
	if (c2 == NULL) puts("��ǰδ�г�����Ϣ\n\n");
	else {
		puts("������Ϣ���£�\n");
		printf("%-17s%-10s%-16s%-14s%-32c\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
	}
	while (c2 != NULL) {
		printf("%-17s%-10s%-16s%-14s\n", c2->Num, c2->Name, c2->Phone, c2->Type);
		c2 = c2->next;
	}
	printf("%s", "\n1.¼�복��������Ϣ\n2.�޸ĳ���������Ϣ\n3.ɾ������������Ϣ\n\n");
	scanf("%d", &n);
	for (;;) {
		if (n != 1 && n != 2 && n != 3) {
			printf("���ٴ����룺");
			scanf("%d", &n);
		}
		else break;
	}

	/*¼�복��������Ϣ*/
	if (n == 1) {
		CAR *newcol = NULL;
		newcol = (CAR*)malloc(sizeof(CAR));
		puts("����д������Ϣ��ֻ��¼��һ��������:\n");
		puts("¼���ʽ��1M758Y  ����ͷ��һ���ַ����������֣���ĸ����Ϊ��д���ܹ�����6Ϊ���֣�\n");
		/*¼�복�ƺ��벢�����쳣����*/
		printf("%s", "���ƺ��룺");
		scanf("%20s", newcol->Num);
		getchar();
		int flag1;
		for (;;) {
			flag1 = 1;
			c2 = gp_head1;
			while (c2 != NULL) {
				if (strcmp(c2->Num, newcol->Num) == 0) {
					flag1 = 0;
					printf("����Ҫ��ӵĳ����Ѵ��ڣ����ٴ����룺");
					scanf("%15s", newcol->Num);
					getchar();
				}
				c2 = c2->next;
			}
			if (flag1 == 1) break;
		}
		/*���복������*/
		printf("%s", "����������");
		scanf("%15s", newcol->Name);
		getchar();
		/*���복����ϵ��ʽ*/
		printf("%s", "������ϵ��ʽ��");
		scanf("%15s", newcol->Phone);
		getchar();
		/*���복������*/
		printf("%s", "�������ͣ�");
		scanf("%15s", newcol->Type);
		getchar();

		

		fin1 = fopen(colfile, "a+b");
		kcol = *newcol;
		fwrite(&kcol, sizeof(CAR), 1, fin1);
		fclose(fin1);
	}

	/*�޸ĳ���������Ϣ*/
	if (n == 2) {
		c3 = gp_head1;
		if (c3 == NULL) printf("%s\n\n", "û��ʲô���޸ĵġ�");
		else {
			char l[20];
			printf("%s", "������Ҫ�޸ĵĳ��ƺ��루ֻ���޸�һ����������");
			puts("¼���ʽ��1M758Y  ����ͷ��һ���ַ����������֣���ĸ����Ϊ��д���ܹ�����6Ϊ���֣�\n");
			scanf("%s", l);
			while (c3 != NULL && strcmp(c3->Num, l) != 0) {
				c3 = c3->next;
			}
			if (c3 == NULL) printf("%s\n\n", "û�з��������ĳ�����");
			else {
				printf("%s\n", "�޸�ǰ����������");
				printf("%s%-15s\n", "���ƺ��룺", c3->Num);
				printf("%s%-15s\n", "����������", c3->Name);
				printf("%s%-15s\n", "������ϵ��ʽ��", c3->Phone);
				printf("%s%-15s\n", "�������ͣ�", c3->Type);

				printf("%s\n", "�������޸ĺ������");
				/*�޸ĳ��ƺ��벢�����쳣����*/
				printf("%s", "���ƺ��룺");
				scanf("%20s", c3->Num);
				getchar();
				int flag1;
				for (;;) {
					flag1 = 1;
					c2 = gp_head1;
					while (c2 != c3 && c2 != NULL) {
						if (strcmp(c2->Num, c3->Num) == 0) {
							flag1 = 0;
							printf("����Ҫ��ӵĳ����Ѵ��ڣ����ٴ����룺");
							scanf("%20s", c3->Num);
							getchar();
							break;
						}
						c2 = c2->next;
					}
					if (flag1 == 1) break;
				}
				/*���복������*/
				printf("%s", "����������");
				scanf("%15s", c3->Name);
				getchar();
				/*���복����ϵ��ʽ*/
				printf("%s", "������ϵ��ʽ��");
				scanf("%15s", c3->Phone);
				getchar();
				/*���복������*/
				printf("%s", "�������ͣ�");
				scanf("%15s", c3->Type);
				getchar();

			
			}
		}
		fin2 = fopen(colfile, "w+b");
		c4 = gp_head1;
		while (c4 != NULL) {
			gcol = *c4;
			fwrite(&gcol, sizeof(CAR), 1, fin2);
			c4 = c4->next;
		}
		fclose(fin2);
	}

	/*ɾ�����ƻ�����Ϣ*/
	if (n == 3) {
		c3 = gp_head1; c4 = gp_head1;
		if (c3 == NULL) printf("%s\n\n", "û��ʲô��ɾ���ġ�");
		else {
			char l[20];
			printf("%s", "������Ҫɾ���ĳ��ƺ��루ֻ��ɾ��һ����������");
			scanf("%s", l);
			while (c3 != NULL && strcmp(c3->Num, l) != 0) {
				c4 = c3;
				c3 = c3->next;
			}
			if (c3 == NULL) printf("%s\n\n", "û�з��������ĳ�����");
			else {
				printf("%s\n", "ɾ��ǰ����������");
				printf("%s%-15s\n", "���ƺ��룺", c3->Num);
				printf("%s%-15s\n", "����������", c3->Name);
				printf("%s%-15s\n", "������ϵ��ʽ��", c3->Phone);
				printf("%s%-15s\n", "�������ͣ�", c3->Type);

				if (c3 == gp_head1) gp_head1 = c3->next;
				else c4->next = c3->next;
				free(c3);
			}
		}
		fin3 = fopen(colfile, "w+b");
		c4 = gp_head1;
		while (c4 != NULL) {
			gcol = *c4;
			fwrite(&gcol, sizeof(CAR), 1, fin3);
			c4 = c4->next;
		}
		fclose(fin3);
	}

	/*���¼�������*/
	freedom();
	LoadData();
	c2 = gp_head1;
	if (c2 == NULL) puts("��ǰδ�г�����Ϣ\n\n");
	else {
		puts("������Ϣ���£�\n");
		printf("%-17s%-10s%-16s%-14s\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
	}
	while (c2 != NULL) {
		printf("%-17s%-10s%-16s%-14s\n", c2->Num, c2->Name, c2->Phone, c2->Type);
		c2 = c2->next;
	}
	return bRet;
}

List MakeEmpty()//����һ���յ����Ա�,���������ݶ������� 
{
	CAR *c1 = gp_head1, *c2 = c1;
	List L;
	L=(List)malloc(sizeof(struct LNode));
	L->Last=-1;
	
	int num=0;
	while(c1!=NULL)
	{
		strcpy(L->Data[num].Num,c1->Num);
		strcpy(L->Data[num].Type,c1->Type);
		strcpy(L->Data[num].Name,c1->Name);
		strcpy(L->Data[num].Phone,c1->Phone);
		num++;
		L->Last++;
		c1=c1->next;
	}

	return L;
}

//��ʾ���г�����Ϣ 
BOOL ShowAllInfor()
 {
 	CAR *c1=gp_head1;
 	if(c1==NULL)
 	{
	 	printf("��ǰδ�г�����Ϣ\n\n");
	 	return;
	}
 	List L=MakeEmpty();
 	ShowInfor(L);
 }
 
 //��ȡ�ַ����ĵ�Dλ�ַ� 
 int GetChar(char X[],int D)
 {
 	int d;
 	d=(int)X[D-1];
 	return d;
 	
 }
 

 
 
 //���ڳ��ƺ���������� 
 BOOL ShowRadixSorting(void){
 	//printf("hello    ");
 	CAR *c1=gp_head1;
 	if(c1==NULL)
 	{
	 	printf("��ǰδ�г�����Ϣ\n\n");
	 	return;
	}
	
 	List L=MakeEmpty();
 	RadixSorting(L);	
	ShowInfor(L);	
 } 
 
 //�������� 
 void RadixSorting(List L){
 	int N=L->Last+1;
 	int D,Di,i;
 	
 	Bucket B;
 	PtrToNode1 tmp,p,List1=NULL;
 	
 	for(i=0;i<Radix;i++)
 	{
	 	B[i].head=B[i].tail=NULL;
 	}
 	
 	//��ʼ��ÿ��ͨΪ������
	 for(i=0;i<N;i++)
	 {
 		tmp=(PtrToNode1)malloc(sizeof(struct Node));
 		strcpy(tmp->key.Num,L->Data[i].Num);
		strcpy(tmp->key.Type,L->Data[i].Type);
		strcpy(tmp->key.Name,L->Data[i].Name);
		strcpy(tmp->key.Phone,L->Data[i].Phone);

 		tmp->next=List1;
 		List1=tmp;
 	}
 	
 	//��ʼ����
	 for(D=MaxDigit;D>=1;D--)
	 {
 		p=List1;
 		while(p)
 		{
 
		 	Di=GetChar(p->key.Num,D);
		 	
		 	tmp=p;
		 	p=p->next;
		 	
		 	tmp->next=NULL;
		 	if(B[Di].head==NULL)
		 	{
	 			B[Di].head=B[Di].tail=tmp;
	 		}
	 		else
	 		{
		 		B[Di].tail->next=tmp;
		 		B[Di].tail=tmp;
		 	}
	 	}
	 	
	 	List1=NULL;
	 	for(Di=Radix-1;Di>=0;Di--)
	 	{
	 		if(B[Di].head)
	 		{
		 		B[Di].tail->next=List1;
		 		List1=B[Di].head;
		 		B[Di].head=B[Di].tail=NULL;
		 	}
	 	}
	 	
	 
	 	
	 	
 	} 
 		for(i=0;i<N;i++)
	 	{
	 		tmp=List1;
	 		List1=List1->next;
	 		
	 		strcpy(L->Data[i].Num,tmp->key.Num);
			strcpy(L->Data[i].Type,tmp->key.Type);
			strcpy(L->Data[i].Name,tmp->key.Name);
			strcpy(L->Data[i].Phone,tmp->key.Phone);
		
			free(tmp);
	 	}
 	
 	
 }
 
 //��ʾ��̬�����������Ϣ 
 void ShowInfor(List L)
 {
 	int i;
 	printf("%-17s%-10s%-16s%-14s\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
 	for(i=0;i<=L->Last;i++)
 	{
		printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);	 	
 	}
 	if(i==0)
 	{
	 	printf("��ǰδ�г�����Ϣ\n\n");
	}
 	
 }
 
 //���ֲ��ҽ��� 
 
 BOOL ShowBinarySearch(void){
 	List L;
 	L=MakeEmpty();
 	RadixSorting(L);
 	char ToFind[15];
 	printf("%s", "������������ҵĳ��ƺ��룺");
	scanf("%15s",ToFind);
	getchar();
	int i=BinarySearch(L,ToFind);
	if(i==-1)
	{
		printf("û��������ƺ���\n");
	}else
	{	
		printf("%-17s%-10s%-16s%-14s\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
		printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);
	}
 	
 }
 
 //���ֲ���
 Position BinarySearch(List Tb1,char k[]){
 	Position left,right,mid;
 	
 	left=0;
 	right=Tb1->Last;
 	while(left<=right)
 	{
		mid=(left+right)/2 ;
		if(strcmp(k,Tb1->Data[mid].Num)<0)
		{
			right=mid-1;
		}else if(strcmp(k,Tb1->Data[mid].Num)>0)
		{
			left=mid+1;
		}else
		{
			return mid;
		}	
 	}
 	
 	return NotFind;
 	
 }
 
 
 //�ֿ�����������ʾ���� 
 BOOL ShowBlockIndex(void)
 {
 	List L;
 	L=MakeEmpty();
 	RadixSorting(L);
 	char ToFind[15];
 	printf("%s", "������������ҵĳ��ƺ��룺");
	scanf("%15s",ToFind);
	getchar();
	int i=BlockIndex(L,ToFind);
	if(i==-1)
	{
		printf("û��������ƺ���\n");
	}else
	{	
		printf("%-17s%-10s%-16s%-14s\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
		printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);
	}
	
 	
 }
 
 //�ֿ��������Һ��� 
 Position BlockIndex(List L,char k[])
 {	
 	//��ʼ���������� 
 	int number[10];
 	int xx;
 	for(xx=0;xx<10;xx++)
 	{
	 	number[xx]=10000;
	 }
 	int i;
 	for(i=0;i<=L->Last;i++)
 	{
 		if(number[(int)L->Data[i].Num[0]-48]>i)
 		{
		 	number[(int)L->Data[i].Num[0]-48]=i;
		}
	 	
 	}

 	for(i=number[(int)k[0]-48];i<=L->Last;i++)
 	{
	 	if(strcmp(L->Data[i].Num,k)==0)
	 	{
	 		return i;
	 	}
	}
 	
 	return -1;
 }
 
 //����������ݵĽ��� 
 BOOL ShowGenrnd(void)
 {
 	BOOL bRet = TRUE;
 	printf("��������������������ݵĸ�����\n");
 	int i=0,j;
 	scanf("%d",&i);
 	getchar();
 	printf("%-17s%-10s%-16s%-14s\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
 	for(j=0;j<i;j++)
 	{
	 	Genrnd();
	 	Sleep(1000);
 	}
 	freedom();
    LoadData();
 	return bRet;
 }
 
//����ṹ������� 
void Genrnd()
 {
 	srand(time(NULL));
 	FILE *fin1;
 	CAR *car=NULL,kcol;
 	car=(CAR*)malloc(sizeof(CAR));
	GenrndType(car->Type);
	GenrndName(car->Name);
	GenrndPhone(car->Phone);
	GenrndNum(car->Num);
	car->next=NULL;	
	printf("%-17s%-10s%-16s%-14s\n", car->Num, car->Name, car->Phone, car->Type);
		
 	fin1 = fopen(colfile, "a+b");
	kcol = *car;
	fwrite(&kcol, sizeof(CAR), 1, fin1);
	fclose(fin1);
 }
 
 //������Ƶ�����
void GenrndNum(char * buff)
{
	int n = 7;
	char First[] = "01234567890123456789012345678901234567890123456789";
	char Second[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890123456789";
	srand(time(NULL));
	int i = 0;
	buff[i] = First[rand() % 50];
	for (i = 1; i < 6; i++)
	{	
		buff[i] = Second[rand() % 46];
	}
	buff[n - 1] = '\0';
}

//������������

void GenrndPhone(char * buff)
{
	int n = 12;
	char metachar[] = "09182736455463728190";
	srand(time(NULL));
	int i = 0;
	buff[i] = '1';
	for (i = 1; i < 11; i++)
	{
		buff[i] = metachar[rand() % 20];
	}
	buff[n - 1] = '\0';
}


// ����������ֵ�����
void GenrndName(char * buff)
{
	int n = 6;
	char First[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char Second[] = "qwertyuiopasdfghjklzxcvbnm";
	srand(time(NULL));
	int i = 0;
	buff[i] = First[rand() % 26];
	for (i = 1; i <5; i++)
	{
		buff[i] = Second[rand() % 26];
	}
	buff[n - 1] = '\0';
}

//����������͵�����
void GenrndType(char * buff)
{
	char* p;
	char *First[] = {"����","����","����","�ղ�","�µ�","ѩ����" ,"�ִ�","����","���" ,"����" ,"���ǵ�", "����", "���Դ�" };
	int i = rand() % 13;
	strcpy(buff, wholename(i));
	buff[strlen(buff)] = '\0';
}

char *wholename(int a){
    if(a==0) return "����";
    else if(a==1) return "����";
    else if(a==2) return "����";
    else if(a==3) return "�ղ�";
    else if(a==4) return "�µ�";
    else if(a==5) return "�ִ�";
    else if(a==6) return "����";
    else if(a==7) return "���";
    else if(a==8) return "����";
    else if(a==9) return "����";
    else if(a==10) return "����";
    else if(a==11) return "���Դ�";
    else return "��";
}

//��ʾģ�����ҽ��� 
BOOL ShowFuzzySearch(void)
{
	BOOL bRet = TRUE;
	//printf("hello world");
	List L;
 	L=MakeEmpty();
 	char ToFind[15];
 	printf("%s", "������������ҵĺ��йؼ��ֵ����ݣ�");
	scanf("%15s",ToFind);
	getchar();
	printf("%-17s%-10s%-16s%-14s\n", "���ƺ���", "��������", "������ϵ��ʽ", "��������");
	FuzzySearch(L,ToFind);
	
	
	
	
	
	return bRet;
}
//ģ�����Һ��� 
void FuzzySearch(List L,char k[])
{
	int i;
	int temp=1;
	for(i=0;i<=L->Last;i++)
	{
		if((intain(k,L->Data[i].Name)||intain(k,L->Data[i].Type)||intain(k,L->Data[i].Num)||intain(k,L->Data[i].Phone))==1)
		{
				printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);
				temp=0;
		}
	}
	
	if(temp==1)
	{
		printf("û���໥ƥ������ݡ�\n\n");
	}
}

//�ж�a�ǲ���b���Ӵ� 
int intain(char *a,char *b){
    int j=0,k;
    for(;b[j]!='\0';j++){
        if(b[j]==a[0]){
            k=1;
            while(b[j+k]==a[k]&&a[k]!='\0') k++;
            if(k==strlen(a))  return 1;
        }
    }
    return 0;
}
 
 


