#ifndef GENE_H
#define GENE_H
#include"maze.h"

class Gene{
private:
    int gene;
public:
    Gene():gene(0){}
    int ReturnGene() {return gene;}
    int GetBit(int n){//取二进制数某一位
        if (n<31&&n>=0){
            return (ReturnGene()>>n)&1;
        }
        else return 0;
    }
    void GeneToChar(char *c){
        for (int i=0;i<16;i++)
            if (GetBit(i)==0) c[i]='0';
            else c[i]='1';
        c[16]='\n';
        for (int i=17;i<33;i++)
            if (GetBit(i)==0) c[i]='0';
            else c[i]='1';
        c[33]='\0';
    }
    void Correct(){
        int zero=0,one=0;
        for (int i=0;i<32;i++){
            if (GetBit(i)==1) one++;
            else zero++;
            if (zero==16) {
                int correctNum=0;
                for (int j=i;j<32;j++){
                    correctNum+=(1<<j);
                }
                gene=gene|correctNum;
                return;
            }
            else if (one==16){
                int correctNum=0;
                for (int j=i;j<32;j++){
                    correctNum+=(1<<j);
                }
                gene=gene&correctNum;
                return;
            }
        }
    }
    void Initial(){
        qsrand((uint)QTime::currentTime().msec());
        for (int i=0;i<32;i++){
            gene+=((qrand()%2)<<i);
        }
        Correct();
    }
    int Length(int * maze){
        int x=1,y=1,sum=0;
        for(int i=0;i<32;i++){
            if (GetBit(i)==1) x++;
            else y++;
            sum+=maze[y*MAXLENGTH+x];
        }
        return sum;
    }
};




#endif // GENE_H
