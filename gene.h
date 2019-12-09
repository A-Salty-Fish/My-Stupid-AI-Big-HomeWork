#ifndef GENE_H
#define GENE_H
#include"maze.h"

const int GeneLength=32;//基因长度
static const int GeneNum=64;//个体数量

double VarRate=0.8;//变异概率
const int GeneBlue=300/GeneNum;//用来绘图的常数 蓝色越深 代表个体越多

const double A=1.1,B=0.2;//线性选择常数

class Gene{
private:
    bool gene[GeneLength];//保存基因型的数组
    int length;//路径求和值
    double f;//适应值
public:
    Gene(int l=0) {
        for (int i=0;i<GeneLength;i++)
            gene[i]=false;
        length=l;
        f=0;
    }
    Gene(const Gene & g){
        for (int i=0;i<GeneLength;i++)
            gene[i]=g.gene[i];
        length=g.length;
        f=g.f;
    }
    void SetBit(int n,bool g){gene[n]=g;}
    int ReturnLength(){return length;}

    int GetBit(int n){//取某一位基因
        if (n<GeneLength-1&&n>=0){
            return gene[n];
        }
        else return 0;
    }

    void GeneToChar(char *c){//将bool数组转换成char数组来显示
        for (int i=0;i<GeneLength/2;i++)
            if (gene[i]==0) c[i]='0';
            else c[i]='1';
        c[GeneLength/2]='\n';
        for (int i=GeneLength/2+1;i<GeneLength+1;i++)
            if (gene[i-1]==0) c[i]='0';
            else c[i]='1';
        c[GeneLength+1]='\0';
    }

    void Correct(){//矫正不正确的基因 基因中只能有16个1和16个0
        int zero=0,one=0;
        qsrand((uint)QTime::currentTime().msec());
        bool First=qrand()%2;//决定从哪个方向开始计数
        if (First){
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
        else{
            for (int i=GeneLength-1;i>=0;i--){
                if (gene[i]==1) one++;
                else zero++;
                if (zero==GeneLength/2) {
                    for (int j=i-1;j>=0;j--){
                        gene[j]=1;
                    }
                    return;
                }
                else if (one==GeneLength/2){
                    for (int j=i-1;j>=0;j--){
                        gene[j]=0;
                    }
                    return;
                }
            }
        }
    }

    void Initial(){//随机生成基因
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<GeneLength;i++){
            gene[i]=qrand()%2;
        }
        Correct();
    }

    int Length(int * maze){//求得该基因的适应值
        int x=1,y=1,sum=0;
        for(int i=0;i<GeneLength;i++){
            if (gene[i]==1) x++;
            else y++;
            sum+=maze[x*MAXLENGTH+y];
        }
        length=sum;
        f=MAXLENGTH*MAXNUMBER-length;
        return sum;
    }

    void Variation(int num){//变异函数 变异num*2位
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
                if (WhoFirst) NewOne.gene[j+i*n]=gene[j+i*n];
                else NewOne.gene[j+i*n]=couple.gene[j+i*n];
            }
            WhoFirst=!WhoFirst;
        }
        NewOne.Correct();
        return NewOne;
    }

    static void UpdateQColor(Gene* group,int* GeneColor){//根据个体密度计算颜色
        //清除上轮数据
        for (int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++)
                GeneColor[i*MAXLENGTH+j]=0;
        //生成这一轮的数据
        for (int i=0;i<GeneNum;i++){
            int x=1,y=1;
            for (int j=0;j<GeneLength-1;j++){
                if (group[i].gene[j]==1) x++;
                else y++;
                GeneColor[x*MAXLENGTH+y]+=GeneBlue;
            }
        }
    }

    static void RankSelect(Gene* &group){//基于排名的选择算法 选择交配池
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<GeneNum;i++){//先排名 选择排序
            int maxf=group[i].f;int maxi=i;
            for (int j=i+1;j<GeneNum;j++){
                if (maxf<group[j].f){
                    maxi=j;
                    maxf=group[j].f;
                }
            }
            Gene tmp=group[maxi];
            group[maxi]=group[i];
            group[i]=tmp;
        }
        Gene* newgroup=new Gene[GeneNum];//新群体
        int rankf[GeneNum];
        int total=10*GeneNum*(GeneNum+1);//比例常数

        for (int i=0;i<GeneNum;i++)
            rankf[i]=total*(A-B*i/(GeneNum+1))/(GeneNum);//线性选择函数

        for (int i=0;i<GeneNum;i++){
            int s=0;
            bool find1=false,find2=false;
            double select1=qrand()%total;
            Sleep(qrand()%2+1);
            double select2=qrand()%total;
            int selectNum1=0,selectNum2=0;
            for (int j=0;j<GeneNum;j++){
                s+=rankf[i];
                if (!find1&&s>select1){
                    find1=true;
                    selectNum1=j;
                }
                else if (!find2&&s>select2){
                    find2=true;
                    selectNum2=j;
                }
                if (find1&&find2) break;
            }
            if (selectNum1==selectNum2){
                i--;
                continue;
            }
            int CrossNum=qrand()%3;//交叉位数
            switch (CrossNum) {
            case 0:CrossNum=2;break;
            case 1:CrossNum=4;break;
            case 2:CrossNum=8;break;
            }
            newgroup[i]=group[selectNum1].Mate(group[selectNum2],CrossNum);//交叉
        }
        delete [] group;
        group=newgroup;
    }
    static void NewTurn(Gene* &group,int * maze){//每一轮操作
        RankSelect(group);
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<GeneNum;i++){//评估与变异
            double var=((double)(qrand()%100))/100.0;
            if (var<VarRate){
                group[i].Variation(qrand()%3+1);//变异2位或4位或6位
            }
            group[i].Length(maze);//评估
        }
        //模拟死亡操作 死亡一定的个体 然后随机生成新个体
        Sleep(qrand()%2+1);
        int NewNum=qrand()%(GeneNum/8);
        for (int i=0;i<NewNum;i++){
            int NewI=qrand()%GeneNum;
            group[NewI].Initial();
            group[NewI].Length(maze);
        }
        if (VarRate>0)
            VarRate=VarRate-0.01;//每轮变异概率减少
    }
};

#endif // GENE_H
