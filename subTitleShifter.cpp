#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

struct subTitle
{
    int num;
    int hi,hf,mi,mf,si,sf,msi,msf;
    vector<string> lines;
};

string format(int n){
    string s = to_string(n);
    if(s.size()<2){
        s = "0"+s;
    }
    return s;
}

subTitle NextSubTitle(FILE* file,bool* flag);
void printSubTile(subTitle st);
void writeSubTile(ofstream& file,subTitle st);
void delaySubTile(subTitle& st,int s);

int main(int argv,char** argc){ 
    
    if(argv != 4){
        cout<<"error :  not enough arguments\n";
        return 0;
    }
    
    FILE* subsfile;

    subsfile = fopen(argc[1],"r");
    if(subsfile == NULL){
        printf("error openning file\n");
        return 1;
    }
    
    vector<subTitle> parsedData;

    bool flag= true;
    subTitle tmpst;
    while (flag){
        tmpst =  NextSubTitle(subsfile,&flag);
        if(flag)
            parsedData.push_back(tmpst);
    }
        
    
    fclose(subsfile);

    for(subTitle& st : parsedData) printSubTile(st); //for debug
    
    int h,m,s;
    s = stoi(argc[3]);

    for(subTitle& st : parsedData)
        delaySubTile(st,s);

    ofstream output = ofstream(argc[2]);

    for(subTitle& st : parsedData)
        writeSubTile(output,st);
    
    output.close();

    return 0;
}



subTitle NextSubTitle(FILE* file,bool* flag){
    subTitle out;

    int len_line=2,maxlen = 300;
    char line[maxlen];

    *flag = *flag and fscanf(file,"%d\n",&out.num)==1;

    *flag = *flag and fscanf(file,"%d:%d:%d,%d --> %d:%d:%d,%d\n",
           &out.hi,&out.mi,&out.si,&out.msi,
           &out.hf,&out.mf,&out.sf,&out.msf) == 8;
    
    while(len_line>1){
        fgets(line,maxlen,file);
        len_line = strlen(line);
        if(len_line>0)
            out.lines.push_back(string(line));
        strset(line,'\0');
    }
    
    return out;
}

void printSubTile(subTitle st){
    cout<<st.num<<'\n'
        <<st.hi<<':'<<st.mi<<':'<<st.si<<','<<st.msi<<" --> "
        <<st.hf<<':'<<st.mf<<':'<<st.sf<<','<<st.msf<<'\n';

    for(string& s : st.lines)
        cout<<s;
}

void writeSubTile(ofstream& file,subTitle st){
    
    file<<format(st.num)<<'\n'
        <<format(st.hi)<<':'<<format(st.mi)<<':'<<format(st.si)<<','<<format(st.msi)<<" --> "
        <<format(st.hf)<<':'<<format(st.mf)<<':'<<format(st.sf)<<','<<format(st.msf)<<'\n';

    for(string& s : st.lines)
        file<<s;
}

void delaySubTile(subTitle& st,int s){
    int secs = st.si + 60*st.mi + 3600*st.hi;
    secs = max(secs-s,0);
    st.hi = secs/3600; secs%=3600;
    st.mi = secs/60; secs%=60;
    st.si = secs;

    secs = st.sf + 60*st.mf + 3600*st.hf;
    secs = max(secs-s,0);
    st.hf = secs/3600; secs%=3600;
    st.mf = secs/60; secs%=60;
    st.sf = secs;
}