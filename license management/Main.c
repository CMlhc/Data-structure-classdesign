#include"Head.h"
#pragma warning(disable:4996)


int main(){
    COORD size = { SCR_COL, SCR_ROW };              /*窗口缓冲区大小*/
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);   /* 获取标准输出设备句柄*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);     /* 获取标准输入设备句柄*/
    SetConsoleTitle(gp_sys_name);                   /*设置窗口标题*/
    SetConsoleScreenBufferSize(gh_std_out, size);   /*设置窗口缓冲区大小80*25*/
    LoadData();                               /*数据加载*/
    InitInterface();                          /*界面初始化*/
    RunSys();              /*系统功能模块的选择及运行*/
    CloseSys();              /*退出系统*/
    system("pause");
    return 0;
}


BOOL LoadData(){
    int Re = 0;
    Re = CreatList(&gp_head1);
    gc_sys_state |= Re;
    gc_sys_state &= ~(1 - Re);
    if (gc_sys_state < 1){
        printf("\n系统基础数据不完整!\n");
        printf("\n按任意键继续...\n");
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
 	/*从数据文件中读院系信息数据，存入以后进先出方式建立的主链中*/
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
 * 函数功能:初始化界面
 */
void InitInterface(){
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*黄色前景和蓝色背景*/
    SetConsoleTextAttribute(gh_std_out, att);  /*设置控制台屏幕缓冲区字符属性*/
    ClearScreen();  /* 清屏*/
    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
    gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*显示菜单栏*/
    ShowState();    /*显示状态栏*/
    return;
}

/**
 * 函数功能:清除屏幕信息
*/
void ClearScreen(void){
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = { 0, 0 };
    unsigned long size;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*取屏幕缓冲区信息*/
    size =(unsigned long) bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/
    /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);
    /*将屏幕缓冲区所有单元填充为空格字符*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
    return;
}


/**
 * 函数功能:在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记
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
    /*在窗口第一行第一列处输出主菜单项*/
    for (i = 0; i < 4; i++) printf("  %s  ", ga_main_menu[i]);
    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*隐藏光标*/
    /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu = { 0, 0, (short)(size.X - 1), 0 };
    /*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    /*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/
    for (i = 0; i<size.X; i++){
        (gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                 | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
            (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*修改后的菜单条字符信息回写到窗口的第一行*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = { 0, 1 };
    SetConsoleCursorPosition(gh_std_out, endPos);  /*将光标位置设置在第2行第1列*/
    /*将菜单项置为热区，热区编号为菜单项号，热区类型为0(按钮型)*/
    i = 0;
    do{
        PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/
        for (j = PosA; j<PosB; j++)
            gp_scr_att[j] |= (i + 1) << 2; /*设置菜单项所在字符单元的属性值*/
        PosA = PosB + 4;
        i++;
    }while (i<4);
    TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/
    return;
}

/**
 * 函数功能:显示状态条,状态条字符属性为白底黑字
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
 * 函数功能:在指定主菜单项上置选中标志,输入参数:num 选中的主菜单项号
*/
void TagMainMenu(int num){
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = { 0, 0 };
    int PosA = 2, PosB;
    char ch;
    int i;
    /*num为0时，将会去除主菜单项选中标记*/
    if (num == 0) {
        PosA = 0;
        PosB = 0;
    }
    else{
        /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
        for (i = 1; i<num; i++)  PosA += strlen(ga_main_menu[i - 1]) + 4;
        PosB = PosA + strlen(ga_main_menu[num - 1]);
    }
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    /*去除选中菜单项前面的菜单项选中标记*/
    for (i = 0; i<PosA; i++){
        (gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                 | BACKGROUND_RED;
        ch = (gp_buff_menubar_info + i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
            (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*在选中菜单项上做标记，黑底白字*/
    for (i = PosA; i<PosB; i++){
        (gp_buff_menubar_info + i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                                 | FOREGROUND_RED;
    }
    /*去除选中菜单项后面的菜单项选中标记*/
    for (i = PosB; i<bInfo.dwSize.X; i++){
        (gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                 | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
            (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
        }
    }
    /*将做好标记的菜单条信息写到窗口第一行*/
    SMALL_RECT rcMenu = { 0, 0, (short)(size.X - 1), 0 };
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}

/**
 * 函数功能:关闭系统
*/
void CloseSys() {
    freedom();
    ClearScreen();        /*清屏*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);
    /*关闭标准输入和输出设备句柄*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);
    /*将窗口标题栏置为运行结束*/
    SetConsoleTitle("运行结束");
    return;
}

/**
 * 函数功能:运行系统, 在系统主界面下运行用户所选择的功能模块
 * 输出参数:phead 主链头指针的地址
 */
void RunSys(){
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos;
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    while (bRet) {
        /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        /*如果记录由鼠标事件产生*/
        if (inRec.EventType == MOUSE_EVENT) {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
            /*层号为0，表明该位置未被弹出子菜单覆盖*/
            if (cNo == 0) {
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0) {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    InitInterface();
                    PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
                }
            }
            else   /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
               if (cAtt > 0) TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            /*如果按下鼠标左边第一键*/
            if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                /*层号为0，表明该位置未被弹出子菜单覆盖*/
                if (cNo == 0) {
                    /*如果该位置处于热区(主菜单项字符单元)*/
                    if(cAtt > 0) {
                        InitInterface();
                        PopMenu(cAtt);/*弹出鼠标所在主菜单项对应的子菜单*/
                    }
                    else /*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
                        if (gp_top_layer->LayerNo > 0) {
                            PopOff();            /*关闭弹出的子菜单*/
                            gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                        }
                }
                else /*层号不为0，表明该位置被弹出子菜单覆盖*/{
                    /*如果该位置处于热区(子菜单项字符单元)*/
                    if (cAtt > 0) {
                        PopOff(); /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                        /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/{
                /*层号为0，表明该位置未被弹出子菜单覆盖*/
                if (cNo == 0) {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生且键被按下*/
                 && inRec.Event.KeyEvent.bKeyDown) {
            vkc = (char)inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/
            /*系统快捷键的处理*/
            /*如果按下F1键*/
            if (vkc == 112) {
                /*如果当前有子菜单弹出*/
                if (gp_top_layer->LayerNo != 0) {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(1, 3);  /*运行其他快捷键功能函数*/
            }
                /*如果按下左或右Alt键*/
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) {
                switch (vkc) { /*判断组合键Alt+字母*/
                    case 88:  /*Alt+X 退出*/
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
                /*其他控制键的处理*/
            else if (asc == 0) {
                /*如果未弹出子菜单*/
                if (gp_top_layer->LayerNo == 0) {
                    /*处理方向键(左、右、下)，不响应其他控制键*/
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
                else  /*已弹出子菜单时*/
                {
                    for (loc = 0, i = 1; i<gi_sel_menu; i++)
                        loc += ga_sub_menu_count[i - 1];
                    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    /*方向键(左、右、上、下)的处理*/
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
                /*按下普通键*/
            else if ((asc - vkc == 0) || (asc - vkc == 32)) {
                /*如果未弹出子菜单*/
                if (gp_top_layer->LayerNo == 0){
                    switch (vkc) {
                        case 70: /*f或F*/
                            InitInterface();
                            PopMenu(1);
                            break;
                        case 77: /*m或M*/
                            InitInterface();
                            PopMenu(2);
                            break;
                        case 81: /*q或Q*/
                            InitInterface();
                            PopMenu(3);
                            break;
                        case 83: /*s或S*/
                            InitInterface();
                            PopMenu(4);
                            break;
                        case 13: /*回车*/
                            InitInterface();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        default:
                            break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/{
                    if (vkc == 27) /*如果按下ESC键*/{
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if (vkc == 13) /*如果按下回车键*/{
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*其他普通键的处理*/{
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc = 0, i = 1; i<gi_sel_menu; i++)
                            loc += ga_sub_menu_count[i - 1];
                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i = loc; i<loc + ga_sub_menu_count[gi_sel_menu - 1]; i++) {
                            if (strlen(ga_sub_menu[i])>0 && vkc == ga_sub_menu[i][1]) { /*如果匹配成功*/
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
 * 函数功能:弹出指定主菜单项对应的子菜单
 * 输入参数:num 指定的主菜单项号
 */
void PopMenu(int num) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;
    /*如果指定主菜单不是已选中菜单*/
    if (num != gi_sel_menu) {
        /*如果此前已有子菜单弹出*/
        if (gp_top_layer->LayerNo != 0){
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
        /*若已弹出该子菜单，则返回*/
    else if (gp_top_layer->LayerNo != 0) return;
    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/
    TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/
    LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i = 1; i<gi_sel_menu; i++) loc += ga_sub_menu_count[i - 1];
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*标签束中标签字符串的个数*/
    COORD aLoc[labels.num];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/
    /*确定标签字符串的输出位置，存放在坐标数组中*/
    for (i = 0; i<labels.num; i++) {
        aLoc[i].X = (short)(rcPop.Left + 2);
        aLoc[i].Y = (short)(rcPop.Top + i + 1);
    }
    labels.pLoc =aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/
    char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/
    char aTag[areas.num];                         /*定义数组存放每个热区的编号*/
    for (i = 0; i<areas.num; i++) {
        aArea[i].Left =(short)(rcPop.Left + 2);  /*热区定位*/
        aArea[i].Top = (short)(rcPop.Top + i + 1);
        aArea[i].Right =(short)(rcPop.Right - 2);
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] =(char)(i + 1);           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*给弹出窗口画边框*/
    pos.X = (short)(rcPop.Left + 2);
    for (pos.Y = (short)(rcPop.Top + 1); pos.Y<rcPop.Bottom; pos.Y++) {
        /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
        pCh = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
        if (strlen(pCh) == 0) /*串长为0，表明为空串*/{   /*首先画横线*/
            FillConsoleOutputCharacter(gh_std_out, '-',(rcPop.Right - rcPop.Left - 3), pos, &ul);
            for (j = rcPop.Left + 2; j<rcPop.Right - 1; j++) {
                /*取消该区域字符单元的热区属性*/
                gp_scr_att[pos.Y*SCR_COL + j] &= 3; /*按位与的结果保留了低两位*/
            }
        }
    }
    /*将子菜单项的功能键设为白底红字*/
    pos.X = (short)(rcPop.Left + 3);
    att = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y =(short)( rcPop.Top + 1); pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0) {
            continue;  /*跳过空串*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/**
* 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
* 输入参数: pRc 弹出窗口位置数据存放的地址
*          att 弹出窗口区域字符属性
*          pLabel 弹出窗口中标签束信息存放的地址
*          pHotArea 弹出窗口中热区信息存放的地址
*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea) {
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = { 0, 0 };
    char *pCh;
    int i, j, row;
    /*弹出窗口所在位置字符单元信息入栈*/
    size.X =(short)( pRc->Right - pRc->Left + 1);    /*弹出窗口的宽度*/
    size.Y =(short)( pRc->Bottom - pRc->Top + 1);    /*弹出窗口的高度*/
    /*申请存放弹出窗口相关信息的动态存储区*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo =(char)(gp_top_layer->LayerNo + 1);
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y * sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y * sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i = pRc->Top; i <= pRc->Bottom; i++) {
        /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
        for (j = pRc->Left; j <= pRc->Right; j++) {
            *pCh = gp_scr_att[i*SCR_COL + j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/
    /*设置弹出窗口区域字符的新属性*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i = pRc->Top; i <= pRc->Bottom; i++) {
        FillConsoleOutputAttribute(gh_std_out, att, (DWORD)size.X, pos, &ul);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i = 0; i<pLabel->num; i++) {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0) {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i = pRc->Top; i <= pRc->Bottom; i++) {
        /*此二重循环设置字符单元的层号*/
        for (j = pRc->Left; j <= pRc->Right; j++)
            gp_scr_att[i*SCR_COL + j] = gp_top_layer->LayerNo;
    }
    for (i = 0; i<pHotArea->num; i++) {
        /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
        row = pHotArea->pArea[i].Top;
        for (j = pHotArea->pArea[i].Left; j <= pHotArea->pArea[i].Right; j++) {
            gp_scr_att[row*SCR_COL + j] |= (pHotArea->pSort[i] << 6)
                                           | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

/**
 * 函数功能:关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性
*/
void PopOff(void) {
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = { 0, 0 };
    char *pCh;
    int i, j;
    if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL)) {
        /*栈底存放的主界面屏幕信息，不用关闭*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*恢复弹出窗口区域原外观*/
    size.X =(short)( gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1);
    size.Y =(short)( gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1);
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*恢复字符单元原属性*/
    pCh = gp_top_layer->pScrAtt;
    for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++) {
        for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++) {
            gp_scr_att[i*SCR_COL + j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*释放动态存储区*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**
* 函数功能: 在指定子菜单项上做选中标记.
* 输入参数: num 选中的子菜单项号
*/
void TagSubMenu(int num) {
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;
    LocSubMenu(gi_sel_menu, &rcPop);  /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom - rcPop.Top - 1))
        return;
    pos.X = (short)(rcPop.Left + 2);
    width = rcPop.Right - rcPop.Left - 3;
    /*首先取消原选中子菜单项上的标记*/
    if (gi_sel_sub_menu != 0) {
        pos.Y = (short)(rcPop.Top + gi_sel_sub_menu);
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
        FillConsoleOutputAttribute(gh_std_out, att, (DWORD)width, pos, &ul);
        pos.X += 1;
        att |= FOREGROUND_RED;/*白底红字*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*在制定子菜单项上做选中标记*/
    pos.X =(short)( rcPop.Left + 2);
    pos.Y =(short)( rcPop.Top + num);
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    FillConsoleOutputAttribute(gh_std_out, att, (DWORD)width, pos, &ul);
    gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
    return;
}

/**
* 函数功能: 执行由主菜单号和子菜单号确定的功能函数.
* 输入参数: m 主菜单项号
*          s 子菜单项号
* 返 回 值: BOOL类型, TRUE 或 FALSE
* 调用说明: 仅在执行函数ExitSys时, 才可能返回FALSE, 其他情况下总是返回TRUE
*/
BOOL ExeFunction(int m, int s) {
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    int(*pFunction[ga_sub_menu_count[0] + ga_sub_menu_count[1] + ga_sub_menu_count[2] + ga_sub_menu_count[3]])(void);
    int i, loc;
    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
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

    /*根据主菜单号和子菜单号计算对应下标*/
    for (i = 1, loc = 0; i<m; i++) loc += ga_sub_menu_count[i - 1];
    loc += s - 1;
    if (pFunction[loc] != NULL) bRet = (*pFunction[loc])();
    return bRet;/*用函数指针调用所指向的功能函数*/
}

/**
 * 函数功能：主菜单快捷键使用说明
 */
BOOL SaveData(void) {
    BOOL bRet = TRUE;
    InitInterface();
    char *plabel_name[] = { "主菜单快捷键使用说明",
                            "Alt+F:使用说明",
                            "Alt+M:数据维护",
                            "Alt+Q:数据查询",
                            "Alt+S:数据统计",
                            "", "", "", "", "", "",
                            "确认"
    };
    ShowModule(plabel_name, 12);
    return bRet;
}

/**
* 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
* 输入参数: num 选中的主菜单项号
* 输出参数: rc 存放区域位置信息的地址
*/
void LocSubMenu(int num, SMALL_RECT *rc) {
    int i, len, loc = 0;
    rc->Top = 1; /*区域的上边定在第2行，行号为1*/
    rc->Left = 1;
    for (i = 1; i<num; i++) {
        /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
        rc->Left += strlen(ga_main_menu[i - 1]) + 4;
        loc += ga_sub_menu_count[i - 1];
    }
    rc->Right =(short)strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/
    for (i = 1; i<ga_sub_menu_count[num - 1]; i++) {
        /*查找最长子菜单字符串，将其长度存放在rc->Right*/
        len = strlen(ga_sub_menu[loc + i]);
        if (rc->Right < len)
            rc->Right = (short)len;
    }
    /*计算区域的右边界*/
    rc->Right += rc->Left + 3;
    /*计算区域下边的行号*/
    rc->Bottom = (short)(rc->Top + ga_sub_menu_count[num - 1] + 1);
    /*右边界越界的处理*/
    if (rc->Right >= SCR_COL) {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**
* 函数功能: 在指定区域画边框.
* 输入参数: pRc 存放区域位置信息的地址
*/
void DrawBox(SMALL_RECT *pRc) {
    char chBox[] = { '+','-','|' };  /*画框用的字符*/
    COORD pos = { pRc->Left, pRc->Top };  /*定位在区域的左上角*/
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/
    for (pos.X =(short)(pRc->Left + 1); pos.X < pRc->Right; pos.X++) {
        /*此循环画上边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/
    for (pos.Y = (short)(pRc->Top + 1); pos.Y < pRc->Bottom; pos.Y++) {
        /*此循环画边框左边线和右边线*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/
    for (pos.X = (short)(pRc->Left + 1); pos.X < pRc->Right; pos.X++) {
        /*画下边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/
    return;
}

/**
 * 函数功能：弹出功能说明菜单
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
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
* 函数功能: 在弹出窗口区域设置热区, 等待并响应用户输入.
* 输入参数: pHotArea
*          piHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针
* 输出参数: piHot 用鼠标单击、按回车或空格时返回当前热区编号
*/
int DealInput(HOT_AREA *pHotArea, int *piHot) {
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = { 0, 0 };
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
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
                switch (vkc) {  /*方向键(左、上、右、下)的处理*/
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
            else if (vkc == 27) {  /*ESC键*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32) {
                /*回车键或空格表示按下当前按钮*/
                iRet = 13;
                break;
            }
        }
    }
    return iRet;
}

/**
 * 函数功能：设置激活热区状态
 * 输入参数：pHotArea，热区缓冲区
 *         iHot，热区类型判断数
 */
void SetHotPoint(HOT_AREA *pHotArea, int iHot) {
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = { 0, 0 };
    WORD att1, att2;
    int i, width;
    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    for (i = 0; i<pHotArea->num; i++) {
        /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        /*热区是按钮类*/
        if (pHotArea->pSort[i] == 0)
            FillConsoleOutputAttribute(gh_std_out, att2, (DWORD)width, pos, &ul);
    }
    pos.X = pHotArea->pArea[iHot - 1].Left;
    pos.Y = pHotArea->pArea[iHot - 1].Top;
    width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
    if (pHotArea->pSort[iHot - 1] == 0) {
        /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(gh_std_out, att1, (DWORD)width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot - 1] == 1) {
        /*被激活热区是文本框类*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}


/**
 * 函数功能：子菜单快捷键使用说明
 */
BOOL BackupData(void) {
    BOOL bRet = TRUE;
    InitInterface();
    char *plabel_name[] = { "              子菜单快捷键使用说明",
                            "S/s:主菜单快捷键说明",
                            "B/b:子菜单快捷键说明",
                            "R/r:其他快捷键说明",
                            "D/d:车辆基本信息",
                            "P/p:随机生成数据",
                            "P/p:二分查找",
                            "C/c:城市分块索引查找",
                            "I/i:基于车牌号码的基数排序",
                            "T/t:模糊查找",

                            "确认"
    };
    ShowModule(plabel_name, 11);
    return bRet;
}

/**
 * 函数功能：其他快捷键使用说明
 */
BOOL RestoreData(void) {
    BOOL bRet = TRUE;
    InitInterface();
    char *plabel_name[] = { "其他快捷键使用说明",
                            "Alt+X:退出系统",
                            "F1:显示部分快捷键使用方式",
                            "方向键:弹出菜单/选项移动",
                            "Enter:弹出菜单/点击确定",
                            "Esc:关闭菜单",
                            "", "", "", "", "",
                            "确认"
    };
    ShowModule(plabel_name, 12);
    return bRet;
}



/**
 * 函数功能：释放所有的内存
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
 * 函数功能：退出系统
 */
BOOL ExitSys(void) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = { "确认退出系统吗？", "确定    取消" };
    int iHot = 1;
    pos.X = (short)(strlen(pCh[0]) + 6);
    pos.Y = 7;
    rcPop.Left = (short)((SCR_COL - pos.X) / 2);
    rcPop.Right =(short)( rcPop.Left + pos.X - 1);
    rcPop.Top = (short)((SCR_ROW - pos.Y) / 2);
    rcPop.Bottom = (short)(rcPop.Top + pos.Y - 1);
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
* 函数功能：维护车辆基本信息
*/
BOOL MaintainCarInfo(void) {
	BOOL bRet = TRUE;
	CAR *c1 = gp_head1, *c2 = c1, *c3 = NULL, *c4 = NULL, gcol, kcol;
	FILE *fin1, *fin2, *fin3;
	InitInterface();
	int n;
	if (c2 == NULL) puts("当前未有车辆信息\n\n");
	else {
		puts("车辆信息如下：\n");
		printf("%-17s%-10s%-16s%-14s%-32c\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
	}
	while (c2 != NULL) {
		printf("%-17s%-10s%-16s%-14s\n", c2->Num, c2->Name, c2->Phone, c2->Type);
		c2 = c2->next;
	}
	printf("%s", "\n1.录入车辆基本信息\n2.修改车辆基本信息\n3.删除车辆基本信息\n\n");
	scanf("%d", &n);
	for (;;) {
		if (n != 1 && n != 2 && n != 3) {
			printf("请再次输入：");
			scanf("%d", &n);
		}
		else break;
	}

	/*录入车辆基本信息*/
	if (n == 1) {
		CAR *newcol = NULL;
		newcol = (CAR*)malloc(sizeof(CAR));
		puts("请填写以下信息（只能录入一个车辆）:\n");
		puts("录入格式：1M758Y  （开头第一个字符必须是数字，字母必须为大写，总共输入6为数字）\n");
		/*录入车牌号码并进行异常处理*/
		printf("%s", "车牌号码：");
		scanf("%20s", newcol->Num);
		getchar();
		int flag1;
		for (;;) {
			flag1 = 1;
			c2 = gp_head1;
			while (c2 != NULL) {
				if (strcmp(c2->Num, newcol->Num) == 0) {
					flag1 = 0;
					printf("你所要添加的车辆已存在，请再次输入：");
					scanf("%15s", newcol->Num);
					getchar();
				}
				c2 = c2->next;
			}
			if (flag1 == 1) break;
		}
		/*输入车主姓名*/
		printf("%s", "车主姓名：");
		scanf("%15s", newcol->Name);
		getchar();
		/*输入车主联系方式*/
		printf("%s", "车主联系方式：");
		scanf("%15s", newcol->Phone);
		getchar();
		/*输入车辆类型*/
		printf("%s", "车辆类型：");
		scanf("%15s", newcol->Type);
		getchar();

		

		fin1 = fopen(colfile, "a+b");
		kcol = *newcol;
		fwrite(&kcol, sizeof(CAR), 1, fin1);
		fclose(fin1);
	}

	/*修改车辆基本信息*/
	if (n == 2) {
		c3 = gp_head1;
		if (c3 == NULL) printf("%s\n\n", "没有什么好修改的。");
		else {
			char l[20];
			printf("%s", "请输入要修改的车牌号码（只能修改一个车辆）：");
			puts("录入格式：1M758Y  （开头第一个字符必须是数字，字母必须为大写，总共输入6为数字）\n");
			scanf("%s", l);
			while (c3 != NULL && strcmp(c3->Num, l) != 0) {
				c3 = c3->next;
			}
			if (c3 == NULL) printf("%s\n\n", "没有符合条件的车辆。");
			else {
				printf("%s\n", "修改前的内容如下");
				printf("%s%-15s\n", "车牌号码：", c3->Num);
				printf("%s%-15s\n", "车主姓名：", c3->Name);
				printf("%s%-15s\n", "车主联系方式：", c3->Phone);
				printf("%s%-15s\n", "车辆类型：", c3->Type);

				printf("%s\n", "请输入修改后的内容");
				/*修改车牌号码并进行异常处理*/
				printf("%s", "车牌号码：");
				scanf("%20s", c3->Num);
				getchar();
				int flag1;
				for (;;) {
					flag1 = 1;
					c2 = gp_head1;
					while (c2 != c3 && c2 != NULL) {
						if (strcmp(c2->Num, c3->Num) == 0) {
							flag1 = 0;
							printf("你所要添加的车牌已存在，请再次输入：");
							scanf("%20s", c3->Num);
							getchar();
							break;
						}
						c2 = c2->next;
					}
					if (flag1 == 1) break;
				}
				/*输入车主姓名*/
				printf("%s", "车主姓名：");
				scanf("%15s", c3->Name);
				getchar();
				/*输入车主联系方式*/
				printf("%s", "车主联系方式：");
				scanf("%15s", c3->Phone);
				getchar();
				/*输入车辆类型*/
				printf("%s", "车辆类型：");
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

	/*删除车牌基本信息*/
	if (n == 3) {
		c3 = gp_head1; c4 = gp_head1;
		if (c3 == NULL) printf("%s\n\n", "没有什么好删除的。");
		else {
			char l[20];
			printf("%s", "请输入要删除的车牌号码（只能删除一个车辆）：");
			scanf("%s", l);
			while (c3 != NULL && strcmp(c3->Num, l) != 0) {
				c4 = c3;
				c3 = c3->next;
			}
			if (c3 == NULL) printf("%s\n\n", "没有符合条件的车辆。");
			else {
				printf("%s\n", "删除前的内容如下");
				printf("%s%-15s\n", "车牌号码：", c3->Num);
				printf("%s%-15s\n", "车主姓名：", c3->Name);
				printf("%s%-15s\n", "车主联系方式：", c3->Phone);
				printf("%s%-15s\n", "车辆类型：", c3->Type);

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

	/*重新加载数据*/
	freedom();
	LoadData();
	c2 = gp_head1;
	if (c2 == NULL) puts("当前未有车辆信息\n\n");
	else {
		puts("车辆信息如下：\n");
		printf("%-17s%-10s%-16s%-14s\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
	}
	while (c2 != NULL) {
		printf("%-17s%-10s%-16s%-14s\n", c2->Num, c2->Name, c2->Phone, c2->Type);
		c2 = c2->next;
	}
	return bRet;
}

List MakeEmpty()//创建一个空的线性表,将链表数据读入数组 
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

//显示所有车牌信息 
BOOL ShowAllInfor()
 {
 	CAR *c1=gp_head1;
 	if(c1==NULL)
 	{
	 	printf("当前未有车辆信息\n\n");
	 	return;
	}
 	List L=MakeEmpty();
 	ShowInfor(L);
 }
 
 //获取字符串的第D位字符 
 int GetChar(char X[],int D)
 {
 	int d;
 	d=(int)X[D-1];
 	return d;
 	
 }
 

 
 
 //基于车牌号码基数排序 
 BOOL ShowRadixSorting(void){
 	//printf("hello    ");
 	CAR *c1=gp_head1;
 	if(c1==NULL)
 	{
	 	printf("当前未有车辆信息\n\n");
	 	return;
	}
	
 	List L=MakeEmpty();
 	RadixSorting(L);	
	ShowInfor(L);	
 } 
 
 //基数排序 
 void RadixSorting(List L){
 	int N=L->Last+1;
 	int D,Di,i;
 	
 	Bucket B;
 	PtrToNode1 tmp,p,List1=NULL;
 	
 	for(i=0;i<Radix;i++)
 	{
	 	B[i].head=B[i].tail=NULL;
 	}
 	
 	//初始化每个通为空链表
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
 	
 	//开始排序
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
 
 //显示静态链表的所有信息 
 void ShowInfor(List L)
 {
 	int i;
 	printf("%-17s%-10s%-16s%-14s\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
 	for(i=0;i<=L->Last;i++)
 	{
		printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);	 	
 	}
 	if(i==0)
 	{
	 	printf("当前未有车辆信息\n\n");
	}
 	
 }
 
 //二分查找界面 
 
 BOOL ShowBinarySearch(void){
 	List L;
 	L=MakeEmpty();
 	RadixSorting(L);
 	char ToFind[15];
 	printf("%s", "请输入你想查找的车牌号码：");
	scanf("%15s",ToFind);
	getchar();
	int i=BinarySearch(L,ToFind);
	if(i==-1)
	{
		printf("没有这个车牌号码\n");
	}else
	{	
		printf("%-17s%-10s%-16s%-14s\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
		printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);
	}
 	
 }
 
 //二分查找
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
 
 
 //分块索引查找显示界面 
 BOOL ShowBlockIndex(void)
 {
 	List L;
 	L=MakeEmpty();
 	RadixSorting(L);
 	char ToFind[15];
 	printf("%s", "请输入你想查找的车牌号码：");
	scanf("%15s",ToFind);
	getchar();
	int i=BlockIndex(L,ToFind);
	if(i==-1)
	{
		printf("没有这个车牌号码\n");
	}else
	{	
		printf("%-17s%-10s%-16s%-14s\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
		printf("%-17s%-10s%-16s%-14s\n", L->Data[i].Num, L->Data[i].Name, L->Data[i].Phone, L->Data[i].Type);
	}
	
 	
 }
 
 //分块索引查找函数 
 Position BlockIndex(List L,char k[])
 {	
 	//初始化索引数组 
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
 
 //随机生产数据的界面 
 BOOL ShowGenrnd(void)
 {
 	BOOL bRet = TRUE;
 	printf("请输入你想生成随机数据的个数：\n");
 	int i=0,j;
 	scanf("%d",&i);
 	getchar();
 	printf("%-17s%-10s%-16s%-14s\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
 	for(j=0;j<i;j++)
 	{
	 	Genrnd();
	 	Sleep(1000);
 	}
 	freedom();
    LoadData();
 	return bRet;
 }
 
//随机结构体的生成 
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
 
 //随机车牌的生成
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

//随机号码的生成

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


// 随机车主名字的生成
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

//随机汽车类型的生成
void GenrndType(char * buff)
{
	char* p;
	char *First[] = {"大众","丰田","本田","日产","奥迪","雪佛兰" ,"现代","宝马","别克" ,"奇瑞" ,"比亚迪", "福特", "马自达" };
	int i = rand() % 13;
	strcpy(buff, wholename(i));
	buff[strlen(buff)] = '\0';
}

char *wholename(int a){
    if(a==0) return "大众";
    else if(a==1) return "丰田";
    else if(a==2) return "本田";
    else if(a==3) return "日产";
    else if(a==4) return "奥迪";
    else if(a==5) return "现代";
    else if(a==6) return "宝马";
    else if(a==7) return "别克";
    else if(a==8) return "奇瑞";
    else if(a==9) return "比亚";
    else if(a==10) return "福特";
    else if(a==11) return "马自达";
    else return "无";
}

//显示模糊查找界面 
BOOL ShowFuzzySearch(void)
{
	BOOL bRet = TRUE;
	//printf("hello world");
	List L;
 	L=MakeEmpty();
 	char ToFind[15];
 	printf("%s", "请输入你想查找的含有关键字的内容：");
	scanf("%15s",ToFind);
	getchar();
	printf("%-17s%-10s%-16s%-14s\n", "车牌号码", "车主姓名", "车主联系方式", "车辆类型");
	FuzzySearch(L,ToFind);
	
	
	
	
	
	return bRet;
}
//模糊查找函数 
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
		printf("没有相互匹配的内容。\n\n");
	}
}

//判断a是不是b的子串 
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
 
 


