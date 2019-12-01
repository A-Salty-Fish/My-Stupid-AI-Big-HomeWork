#ifndef GENE_H
#define GENE_H
#include"maze.h"
const int GeneLength=32;//基因长度
static const int GeneNum=32;//个体数量
const int VarRate=0.02;//变异概率
const int GeneBlue=300/GeneNum;//用来绘图的常数 蓝色越深 代表个体越多
const int RouNum=10;//轮盘赌比例常数
class Gene{
private:
    bool gene[GeneLength];//保存基因型的数组
    int length;//路径求和值
    double f;//适应值
public:
    Gene(int l=0) {for (int i=0;i<GeneLength;i++) gene[i]=false;length=l;f=0;}
    Gene(const Gene & g){for (int i=0;i<GeneLength;i++) gene[i]=g.gene[i];length=g.length;f=g.f;}
    void SetBit(int n,bool g){gene[n]=g;}
    int ReturnLength(){return length;}
    int GetBit(int n){//取某一位基因
        if (n<GeneLength-1&&n>=0){
            return gene[n];
        }
        else return 0;
    }

    void GeneToChar(char *c){//将bool数组转换成char数组来显示
        for (int i=0;i<16;i++)
            if (gene[i]==0) c[i]='0';
            else c[i]='1';
        c[16]='\n';
        for (int i=17;i<33;i++)
            if (gene[i-1]==0) c[i]='0';
            else c[i]='1';
        c[33]='\0';
    }

    void Correct(){//矫正不正确的基因 基因中只能有16个1和16个0
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
    void Initial(){//随机生成基因
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<GeneLength;i++){
            gene[i]=qrand()%2;
            //            Sleep(qrand()%2);
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
        f/=RouNum;
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
                if (WhoFirst) NewOne.gene[j+i*num]=gene[j+i*num];
                else NewOne.gene[j+i*num]=couple.gene[j+i*num];
                WhoFirst=~WhoFirst;
            }
        }
        NewOne.Correct();
        length=0;f=0;
        return NewOne;
    }
    static void UpdateQColor(Gene* group,int* GeneColor){//根据个体密度计算颜色
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
    static void RouSelect(Gene* &group){//轮盘赌算法
        qsrand((uint)QTime::currentTime().msec());
        double sum=0; for (int i=0;i<GeneNum;i++) sum+=group[i].f;
        Gene* newgroup=new Gene[GeneNum];
        for (int i=0;i<GeneNum;i++){
            bool find1=false,find2=false;
            double select1=(qrand()%(int)(RouNum*sum))/RouNum;
            Sleep(qrand()%3+1);
            double select2=(qrand()%(int)(RouNum*sum))/RouNum;
            int selectNum1=0,selectNum2=0;
            double s=0;
            for (int j=0;j<GeneNum;j++){
                s+=group[j].f;
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
            int CrossNum=qrand()%3;
            switch (CrossNum) {
            case 0:CrossNum=4;break;
            case 1:CrossNum=8;break;
            case 2:CrossNum=16;break;
            default:CrossNum=33;break;
            }
            newgroup[i]=group[selectNum1].Mate(group[selectNum2],CrossNum);
            newgroup[i].Initial();
        }
        delete [] group;
        group=newgroup;
    }
    static void NewTurn(Gene* &group,int * maze){//每一轮操作
        RouSelect(group);
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<GeneNum;i++){//评估与变异
            double var=((double)(qrand()%1000))/1000;
            if (var<VarRate){
                group[i].Variation(qrand()%3+1);//变异2位或4位或6位
            }
            group[i].Length(maze);
        }
    }
};

#endif // GENE_H
