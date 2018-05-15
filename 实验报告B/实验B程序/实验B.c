#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int NUM;
typedef struct State{
	int x, y;					//position
	float Num_people;			//weight
}State[100];

void Input(State S){
	int x, y, i = 0;
	printf("您要输入的小区数：\n");
	scanf("%d",&NUM);
	if(NUM < 0){
		printf("您输入的数据有误");
		exit(1);
	}
	while(i < NUM){
		printf("请输入小区的位置X(x>0)：\n");
		scanf("%d",&S[i].x);
		if(S[i].x < 0){
			printf("您输入的数据有误");
			printf("请输入小区的位置X(x>0)：\n");
			scanf("%d",&S[i].x);
		}
		printf("Y(y>0):\n");
		scanf("%d",&S[i].y);
		if(S[i].x < 0){
			printf("您输入的数据有误");
			printf("请输入该小区的人口：\n");
			scanf("%f",&S[i].Num_people); 
		}
		printf("请输入该小区的人口：\n");
		scanf("%f",&S[i].Num_people); 
		i++;
	}	 
}

int Compare_max(int *a){
	int i,max = a[0];
	for(i = 0;i < NUM;i++){
		if(max <= a[i]){
			max = a[i];
		}
	}
	return max; 
}

int Compare_min(int *a){
	int i,min = a[0];
	for(i = 0;i < NUM;i++){
		if(min >= a[i]){
			min = a[i];
		}
	}
	return min; 
}
float Distance_L(int x1,int x2,int y1, int y2){
	float a;
	a = sqrt(((x1 - x2)*(x1 - x2))+((y1 - y2)*(y1 - y2)));
	return a;
} 

float Distance(int x,int y,State S){
	float L = 0;
	int i;
	for(i = 0;i<NUM;i++){
		L = L + Distance_L(x, S[i].x, y, S[i].y)/S[i].Num_people;
	}
	return L; 
}


void Locate(State S){
	int X[NUM], Y[NUM];
	int max_x, max_y, min_x, min_y, x, y, i,x0, y0;
	for(i = 0;i < NUM;i++){
		X[i] = S[i].x;
		Y[i] = S[i].y;
	}
	max_x = Compare_max(X);
	max_y = Compare_max(Y);
	min_x = Compare_min(X);
	min_y = Compare_min(Y);
	//printf("%d%d",max_x,max_y);
	float  min_distance = 1.797693E+308, z;
	for(x = min_x;x < max_x;x++)
		for(y = min_y;y < max_y;y++){
			z = Distance(x,y,S); 
	 		if( min_distance > z ){
		 		min_distance = z;							
		 		x0 = x;
		 		y0 = y;
		 	}
		}
	printf("邮局合适的地址为：\n");
	printf("X=%d Y=%d\n",x0,y0);
	printf("他到各个小区的加权距离是:\n%f",min_distance);
}

int main()
{
	State S;
	Input(S);
	Locate(S);
	return 0; 
}