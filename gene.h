#ifndef GENE_H
#define GENE_H
#include"maze.h"
const int GeneLength=32;//基因长度
static const int GeneNum=16;//个体数量
const int VarRate=0.01;//变异概率

class Gene{
private:
    bool gene[GeneLength];
    int length;
public:
    Gene() {for (int i=0;i<GeneLength;i++) gene[i]=false;length=0;}
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
            sum+=maze[y*MAXLENGTH+x];
        }
        return sum;
    }
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
    static void NewTurn(Gene* group,int * maze){//每一轮操作
        for (int i=0;i<GeneNum;i++){//评估
            group[i].length=group[i].Length(maze);
        }
        for (int i=1;i<GeneNum;i++){//选择排序
            int mini=i;
            for (int j=i;j<GeneNum;j++){
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
        //留下前二分之一个体 交叉产生四分之一新个体 随机新生成四分之一个体
        for (int i=GeneNum/2;i<GeneNum*3/4;i++){
            group[i]=group[i-GeneNum/2].Mate(group[i-GeneNum/2+1],4);
            double rate= (qrand()%1000)/1000;
            if (rate<VarRate){//变异
                int num=qrand()%2+1;
                group[i].Variation(num);//变异2或4位
            }
        }
        for (int i=GeneNum*3/4;i<GeneNum;i++){
            group[i].Initial();
        }
    }


};




#endif // GENE_H
