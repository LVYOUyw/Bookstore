#include"data.hpp"
#include"user.hpp"
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
using namespace std;

data Data;
user User;
char s[4][45], u[4][45], last[45], key_word[24][45];
string line, t[4];
int pow_state = 7, pos, cnt, tot, kcnt;
pair<double, double> list[10005];
bool flag1, flag2;

bool isin(string tmp)
{
    return line.find(tmp) != line.npos;
}

int getint(char* s)
{
    int x = 0;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        x = x * 10 + s[i] - '0';
    return x;
}


void process(string obj, char* s, bool pd)
{
    s[0] = '^';
    s[1] = '\0';
    int pos = line.find(obj);
    int len = line.length();
    if (pos != line.npos)
    {
        pos += obj.length();
        int i;
        for (i = pos; i < len; i++)
        {
            if ((pd == 0 && line[i] == ' ' )|| (pd == 1 && line[i] == '"')) break;
            s[i - pos] = line[i];
        }
        s[i - pos] = '\0';
    }
}


double getdouble(char* s)
{

    double x = 0;
    int i;
    double base = 0.1;
    int len = strlen(s);
    for (i = 0; i < len; i++)
    {
        if (s[i] == '.') break;
        if (s[i] > '9' || s[i] < '0') break;
        x = x * 10 + s[i] - '0';
    }
    i++;
    for (; i < len; i++)
    {
        if (s[i] > '9' || s[i] < '0') break;
        x += (s[i] - '0') * base;
        base *= 0.1;
    }
    return x;
}

void trans(string line, int st, char u[4][45])
{
    int len = line.length();
    cnt = 0;
    int n = 0;
    while (line[len - 1] == ' ' || line[len - 1] == '\r') len--;
    for (int i = st; i < len; i++)
        if (line[i] == ' ')
        {
            u[cnt][n] = '\0';
            cnt++;
            n = 0;
            continue;
        }
        else u[cnt][n++] = line[i];
    u[cnt][n] = '\0';
}

void deal_with_key_word(char *s)
{
    kcnt = 0;
    if (s[0] == '^')
    {
        key_word[0][0] = '^';
        key_word[0][1] = '\0';
        return;
    }
    int len = strlen(s);
    int ll = 0;
    for (int i = 0; i < len; i++)
        if (s[i] == '|') key_word[kcnt][ll] = '\0', kcnt++, ll = 0;
        else key_word[kcnt][ll++] = s[i];
/*    printf("%d: \n", kcnt);
    for (int i = 0; i <= kcnt; i++)
        printf("%s\n", key_word[i]);*/
}

void init(void)
{
    s[0][0] = s[1][0] = s[2][0] = s[3][0] = '^';
    s[0][1] = s[1][1] = s[2][1] = s[3][1] = '\0';
}

void Insert(char s[4][45], double &v, int &num)
{
    for (int i = 0; i < 3; i++)
        Data.Insert(t[i], s[i], s, v, num);
    deal_with_key_word(s[3]);
    char key[45];
//    printf("Insert: %d\n", kcnt);
    for (int i = 0; i <= kcnt; i++)
    {
        memcpy(key, key_word[i], sizeof(key_word[i]));
//        printf("%s\n",key);
        Data.Insert(t[3], key, s, v, num);
    }
}


void Delete(char s[4][45])
{
    for (int i = 0; i < 3; i++)
        Data.Delete(t[i], s[0], s[i]);
    deal_with_key_word(s[3]);
    char key[45];
//    printf("Delete: %d\n",kcnt);
    for (int i = 0; i <= kcnt; i++)
    {
        memcpy(key, key_word[i], sizeof(key_word[i]));
//        printf("%s\n",key);
        Data.Delete(t[3], s[0], key);
    }
}

void user_judge(void)
{

    if (isin("logout"))
    {
        if (pow_state == 0) puts("Invalid");
        pow_state = 0;
        return;
    }
    if (isin("su ")) //some bug here
    {
       trans(line, 3, u);
       if (cnt > 1)
       {
           puts("Invalid");
           return;
       }
       pow_state = User.login(u[0], u[1], pow_state);

       if (pow_state == 0) puts("Invalid");
       return;
    }

    if (isin("useradd "))
    {
        if (pow_state < 3)
        {
            puts("Invalid");
            return;
        }

        trans(line, 8, u);
        if (cnt != 3)
        {
            puts("Invalid");
            return;
        }
        int pow = u[2][0] - '0';
        memcpy(u[2],u[3],sizeof (u[3]));
        if (pow >=  pow_state)
        {
            puts("Invalid");
            return;
        }

        User.adduser(u, pow);
        return;
    }
    if (isin("register "))
    {
        trans(line, 9, u);
        if (cnt != 2)
        {
            puts("Invalid");
            return;
        }
        int pow = 1;
        User.adduser(u, pow);
        return;
    }
    if (isin("delete "))
    {

        if (pow_state != 7)
        {
            puts("Invalid");
            return;
        }

        trans(line, 7, u);
        if (cnt != 0)
        {
            puts("Invalid");
            return;
        }
        User.Delete(u[0]);
        return;
    }
    if (isin("passwd "))
    {
        if (pow_state == 0)
        {
            puts("Invalid");
            return;
        }
        trans(line, 7, u);
        if (!(cnt == 1 && pow_state == 7 || cnt == 2) )
        {
            puts("Invalid");
            return;
        }
        if (pow_state == 7) memcpy(u[2], u[1], sizeof (u[1]));
        User.passwd(u[0], u[1], u[2], pow_state);
        return;
    }
    flag1 = 0;
}

void data_judge(void)
{

    if (isin("select "))
    {
        if (pow_state < 3) {puts("Invalid");return;}
        trans(line, 7, u);
        if (cnt != 0) {puts("Invalid");return;}
        double v; int num;
        memcpy(last, u[0], sizeof (u[0]));
        Data.select("0", u[0], v, num, s);
        if (s[0][0] == '^')
        {
            init();
            memcpy(s[0], u[0], sizeof (u[0]));
            v = 0; num = 0;
            for (int i = 0; i < 4; i++)
                Data.Insert(t[i], s[i], s, v, num);
        }
        return;
    }
    if (isin("modify "))
    {
        if (pow_state < 3 || last[0] == '^') {puts("Invalid");return;}
        double v; int num;
        process("-ISBN=", u[0], 0);
        process("-name=\"", u[1], 1);
        process("-author=\"", u[2], 1);
        process("-keyword=\"", u[3], 1);
        char tmp[45];
        process("-price=", tmp, 0);
        if (u[0][0] != '^')
        {
            Data.select("0", u[0], v, num, s);
            if (s[0][0] != '^')
            {
                puts("Invalid");
                return;
            }
        }
        Data.select("0", last, v, num ,s);
        Delete(s);
        if (tmp[0] != '^') v = getdouble(tmp);
        for (int i = 0; i < 4; i++)
            if (u[i][0] != '^') memcpy(s[i], u[i], sizeof (u[i]));
    /*    for (int i = 0; i < 4; i++)
            if (s[i][0] != '^') printf("%s\n",s[i]);
        puts("");*/
        Insert(s, v, num);
        if (s[0][0] != '^') memcpy(last, s[0], sizeof (s[0]));
        return;
    }

    if (isin("import "))
    {
        if (pow_state < 3 || last[0] == '^') {puts("Invalid");return;}
        trans(line, 7, u);
        double v; int num;
        Data.select("0", last, v, num, s);
        Delete(s);
        num += getint(u[0]);
        tot++;
        list[tot] = make_pair(list[tot-1].first, list[tot-1].second + getdouble(u[1]));
        Insert(s, v, num);
        return;
    }
    if (isin("show finance"))
    {

        if (pow_state < 7) {puts("Invalid");return;}
        int len = line.length();
        if (len == 12) printf("+ %.2lf - %.2lf\n", list[tot].first, list[tot].second);
        else
        {
            trans(line, 13, u);
            int tmp = max(0, tot - getint(u[0]));
            printf("+ %.2lf - %.2lf\n", list[tot].first - list[tmp].first, list[tot].second - list[tmp].second);
        }
        return;
    }
    if (isin("show"))
    {
        if (pow_state < 1)
        {
            puts("Invalid");
            return;
        }
        process("-ISBN=", s[0], 0);
        process("-name=\"", s[1], 1);
        process("-author=\"", s[2], 1);
        process("-keyword=\"", s[3], 1);
        int o = 0;
        char tmp[45];
        tmp[0] = 0;
        tmp[1] = '\0';
        for (int i = 0; i < 4; i++)
            if (s[i][0] != '^') {o = i; break;}
        if (o == 0)  Data.show("0", s[0], s[o], s);
        else Data.show(t[o], tmp, s[o], s);
        return;
    }

    if (isin("buy "))
    {
        if (pow_state < 1) {puts("Invalid");return;}
        trans(line, 4, u);
        double v; int num;
        Data.select("0", u[0], v, num, s);
        if (s[0][0] == '^') {puts("Invalid");return;}
        int tmp = getint(u[1]);
        if (num < tmp) {puts("Invalid");return;}
        tot++;
        list[tot] = make_pair(list[tot-1].first + v * tmp, list[tot-1].second);
        num -= tmp;
        Delete(s);
        Insert(s, v, num);
        return;
    }
    flag2 = 0;
}

int main()
{
    ifstream in;
    in.open("command.txt");
    t[0]="0";t[1]="1";
    t[2]="2";t[3]="3";
    last[0] = '^';
    int C = 0;
    if (in)
    {
        while (1)
        {
            getline(in, line);
            flag1 = flag2 = 1;
            if (line == "exit") break;
            user_judge();
            data_judge();
            if (!flag1 && !flag2) puts("Invalid");
        }
    }
    in.close();
    return 0;
}
