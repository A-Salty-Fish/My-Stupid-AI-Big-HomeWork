#ifndef GENE_H
#define GENE_H
#include"maze.h"
const int GeneLength=32;//基因长度
static const int GeneNum=64;//个体数量
const int VarRate=0.01;//变异概率
const int GeneBlue=300/GeneNum;//用来绘图的常数 蓝色越深 代表个体越多

class Gene{
private:
    bool gene[GeneLength];
    int length;
public:
    Gene(int l=0) {for (int i=0;i<GeneLength;i++) gene[i]=false;length=l;}
    Gene(const Gene & g){for (int i=0;i<GeneLength;i++) gene[i]=g.gene[i];length=g.length;}
    int GetBit(int n){//取某一位
        if (n<GeneLength-1&&n>=0){
            return gene[n];
        }
        else return 0;
    }
    void GeneToChar(char *c){
        for (int i=0;i<16;i++)
            if (gene[i]==0) c[i]='0';
            else c[i]='1';
        c[16]='\n';
        for (int i=17;i<33;i++)
            if (gene[i-1]==0) c[i]='0';
            else c[i]='1';
        c[33]='\0';
    }
    void Correct(){
        int zero=0,one=0;
        for (int i=0;i<GeneLength;i++){
            if (gene[i]==1) one++;
            else zero++;
            if (zero==GeneLength/2) {
                for (int j=i+1;j<GeneLength;j++){
                    gene[j]=1;
                }
                return;
            }
            else if (one==GeneLength/2){
                for (int j=i+1;j<GeneLength;j++){
                    gene[j]=0;
                }
                return;
            }
        }
    }
    int onenum(){
        int oneN=0;
        for (int i=0;i<GeneLength;i++){
            if (gene[i]==1) oneN++;
        }
        return oneN;
    }
    void Initial(){
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<GeneLength;i++){
            gene[i]=qrand()%2;
        }
        Correct();
    }
    int Length(int * maze){
        int x=1,y=1,sum=0;
        for(int i=0;i<GeneLength;i++){
            if (gene[i]==1) x++;
            else y++;
            sum+=maze[x*MAXLENGTH+y];
        }
        length=sum;
        return sum;
    }
    int ReturnLength(){return length;}
    void Variation(int num){//变异函数 变异num位
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<num;i++){
            int n=qrand()%GeneLength;
            gene[n]=~gene[n];
        }
        Correct();
    }
    Gene Mate(Gene& couple,int n){//每n个交叉基因
        Gene NewOne;
        qsrand((uint)QTime::currentTime().msec());
        bool WhoFirst=qrand()%2;//决定哪个个体正在复制这一段
        int num=GeneLength/n;//分成了num段
        for (int i=0;i<num;i++){//对于每一段
            for (int j=0;j<n;j++){
                if (WhoFirst==0) NewOne.gene[j+i*num]=gene[j+i*num];
                else NewOne.gene[j+i*num]=couple.gene[j+i*num];
                WhoFirst=~WhoFirst;
            }
        }
        NewOne.Correct();
        return NewOne;
    }
    static void UpdateQColor(Gene* group,int* GeneColor){
        for (int i=0;i<MAXLENGTH;i++) for (int j=0;j<MAXLENGTH;j++) GeneColor[i*MAXLENGTH+j]=0;
        for (int i=0;i<GeneNum;i++){
            int x=1,y=1;
            for (int j=0;j<GeneLength-1;j++){
                if (group[i].gene[j]==1) x++;
                else y++;
                GeneColor[x*MAXLENGTH+y]+=GeneBlue;
            }
        }
    }
    static void NewTurn(Gene* group,int * maze){//每一轮操作
        for (int i=0;i<GeneNum;i++){//评估
            group[i].length=group[i].Length(maze);
        }
        for (int i=0;i<GeneNum;i++){//选择排序
            int mini=i;
            for (int j=i+1;j<GeneNum;j++){
                if (group[j].length<group[mini].length){
                    mini=j;
                }
                if (mini!=i){
                    Gene tmpg=group[i];
                    group[i]=group[mini];
                    group[mini]=tmpg;
                }
            }
        }
        qsrand((uint)QTime::currentTime().msec());
        //交叉产生1/4新个体 随机新生成1/4个体  然后前1/4个体变异2位 1/4个体变异4位
        for (int i=GeneNum/2;i<GeneNum*3/4;i++){//交叉产生1/4新个体
            group[i]=group[i-GeneNum/2].Mate(group[i-GeneNum/2+1],4);
            //            qsrand((uint)QTime::currentTime().msec());
            //            double rate= (qrand()%1000)/1000;
            //            if (rate<VarRate){//变异
            //                int num=qrand()%2+1;
            //                group[i].Variation(num);//变异2或4位
            //            }
        }
        for (int i=GeneNum*3/4;i<GeneNum;i++){//随机新生成1/4个体
            qsrand((uint)QTime::currentTime().msec());
            Sleep(qrand()%10);
            group[i].Initial();
        }
        for (int i=0;i<GeneNum/4;i++){//1/4个体变异2位
            Sleep(qrand()%10);
            group[i].Variation(1);
        }
        for (int i=GeneNum/4;i<GeneNum/2;i++){//1/4个体变异4位
            group[i].Variation(2);
            Sleep(qrand()%10);
        }
    }
};

#endif // GENE_H
