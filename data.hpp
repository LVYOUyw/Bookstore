#include<cstring>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
#define next nex
const int N = 100;

class data
{
    private:
        fstream ioA, ioB;
        int st[4] = {-1, -1, -1, -1}, ST[4] = {0, 0, 0, 0};
        int cz[4] = {0, 0, 0, 0};
        string nts[4] = {"0", "1", "2", "3"};
        bool debug = 0;
    public:
        data ()
        {
            fstream io;
            io.open("fileA0");
            if (!io)
            {
                ofstream out[4];
                for (int i = 0; i < 4; i++)
                {
                    out[i].open("fileA" + nts[i], ios::binary);
                    out[i].close();
                }
                int Next = -1;
                int Ptr = 0;
                int siz = 0;
                for (int i = 0; i < 4; i++)
                {
                    out[i].open("fileB" + nts[i], ios::binary);
                    out[i].write(reinterpret_cast<char *> (&Next), 4);
                    out[i].write(reinterpret_cast<char *> (&Ptr), 4);
                    out[i].write(reinterpret_cast<char *> (&siz), 4);
                    out[i].close();
                }
            }
            else
            {
                io.close();
                io.open("KZLB", ios::binary | ios::in | ios::out);
                for (int i = 0; i < 4; i++)
                    io.read(reinterpret_cast<char *> (&st[i]), 4);
                for (int i = 0; i < 4; i++)
                    io.read(reinterpret_cast<char *> (&ST[i]), 4);
                for (int i = 0; i < 4; i++)
                    io.read(reinterpret_cast<char *> (&cz[i]), 4);
                io.close();
            }
        }

        ~ data()
        {
            ofstream out;
            out.open("KZLB", ios::binary);
            for (int i = 0; i < 4; i++)
                out.write(reinterpret_cast<char *> (&st[i]), 4);
            for (int i = 0; i < 4; i++)
                out.write(reinterpret_cast<char *> (&ST[i]), 4);
            for (int i = 0; i < 4; i++)
                out.write(reinterpret_cast<char *> (&cz[i]), 4);
            out.close();
        }

        void Insert(string t, char *key, char s[4][45], double v, int num)
        {
            int o = t[0] - '0';
            int next, siz;
            ioA.open("fileA" + t, ios::binary | ios::in | ios::out);
            ioB.open("fileB" + t, ios::binary | ios::in | ios::out);
            ioA.seekg(0, ios::end);
            int pos = ioA.tellg();
            cz[o] ++;
            int posA = st[o], posB = ST[o];
            find(s[0], key, posA, posB);
            if (posA == -1)
                next = st[o], st[o] = pos;
            else
            {
                ioA.seekg(posA);
                ioA.read(reinterpret_cast<char *> (&next), 4);
                ioA.seekp(posA);
                ioA.write(reinterpret_cast<char *> (&pos), 4);
            }
            ioA.seekp(0,ios::end);
            ioA.write(reinterpret_cast<char *> (&next), 4);
            ioA.write(reinterpret_cast<char *> (&v), 8);
            ioA.write(reinterpret_cast<char *> (&num), 4);
            ioA.write(reinterpret_cast<char *> (key),40);
            for (int i = 0; i < 4; i++)
                ioA.write(reinterpret_cast<char *> (s[i]), 40);
            if (posA == -1)
            {
                ioB.seekp(ST[o] + 4);
                ioB.write(reinterpret_cast<char *> (&pos), 4);
            }
            if (posB == -1) posB = ST[o];
            ioB.seekg(posB + 8);
            ioB.read(reinterpret_cast<char *> (&siz), 4);
            siz++;
            ioB.seekp(posB + 8);
            ioB.write(reinterpret_cast<char *> (&siz), 4);
            ioA.close();
            ioB.close();
            if (cz[o] > N) maintain(t);
        }

        bool cmp(char* A1, char* A2, char* B1, char* B2)
        {
            return strcmp(A1, A2) < 0 || (strcmp(A1, A2) == 0 && strcmp(B1, B2) <= 0);
        }

        void Getnext(int& posA, int& posB)
        {
            if (posB == -1) return;
            ioB.seekg(posB);
            ioB.read(reinterpret_cast<char *> (&posB), 4);
            if (posB == -1) {posA = -1; return;}
            ioB.seekg(posB + 4);
            ioB.read(reinterpret_cast<char *> (&posA), 4);
        }

        void getnext(int &posA)
        {
            if (posA == -1) return;
            ioA.seekg(posA);
            ioA.read(reinterpret_cast<char *> (&posA), 4);
        }

        void find(char* ISBN, char* KEY, int& posA, int& posB)
        //????key??????????isbn??????????????????
        {
            char isbn[45], key[45];
            int lasta = -1, lastb = -1;
            while (posA != -1)
            {
                ioA.seekg(posA + 16);
                ioA.read(reinterpret_cast<char *> (key), 40);
                ioA.read(reinterpret_cast<char *> (isbn), 40);
            //    printf("%s\n%s\n", KEY, key);
                if (cmp(KEY, key, ISBN, isbn)) break;
                lasta = posA;
                lastb = posB;
                Getnext(posA, posB);
            }
            posA = lasta; posB = lastb;
            if (posA < 0) return;
            while (posA != -1)
            {
                getnext(posA);
                if (posA == -1)
                {
                    posA = lasta;
                    posB = lastb;
                    return;
                }
                ioA.seekg(posA + 16);
                ioA.read(reinterpret_cast<char *> (key), 40);
                ioA.read(reinterpret_cast<char *> (isbn), 40);
                if (cmp(KEY, key, ISBN, isbn)) break;
                lasta = posA;
                lastb = posB;
            }
            posA = lasta; posB = lastb;
        }

        void select(string t, char* isbn, double& v, int& num, char s[4][45])
        {
            ioA.open("fileA" + t, ios::binary | ios::in | ios::out);
            ioB.open("fileB" + t, ios::binary | ios::in | ios::out);
            int o = t[0] - '0';
            int posA = st[o], posB = ST[o];
            find(isbn, isbn, posA, posB);
            if (posA == -1) posA = st[o];else getnext(posA);
            if (posA == -1)
            {
                s[0][0] = '^';
                ioA.close();
                ioB.close();
                return;
            }
            ioA.seekg(posA + 4);
            ioA.read(reinterpret_cast<char *> (&v), 8);
            ioA.read(reinterpret_cast<char *> (&num), 4);
            ioA.seekg(posA + 16 + 40);
            for (int i = 0; i < 4; i++)
                ioA.read(reinterpret_cast<char *> (s[i]), 40);
            if (strcmp(isbn, s[0])) {s[0][0] = '^'; s[0][1] = '\0';}
            ioA.close();
            ioB.close();
        }

        void Delete(string t, char* isbn, char* key)
        {
            ioA.open("fileA" + t, ios::binary | ios::in | ios::out);
            ioB.open("fileB" + t, ios::binary | ios::in | ios::out);
            int o = t[0] - '0';
            int posA = st[o], posB = ST[o], siz;
            find(isbn, key, posA, posB);
            cz[o]++;
            if (posA == -1)
            {
                posA = st[o];
                ioA.seekg(posA);
                ioA.read(reinterpret_cast<char *> (&st[o]), 4);
                ioB.seekp(ST[o] + 4);
                ioB.write(reinterpret_cast<char *> (&st[o]), 4);
            }
            else
            {
                int tmp = posA;
                getnext(posA);
                getnext(posA);
                ioA.seekp(tmp);
                ioA.write(reinterpret_cast<char *> (&posA), 4);
            }
            posB = posB < 0 ? ST[o] : posB;
            ioB.seekg(posB + 8);
            ioB.read(reinterpret_cast<char *> (&siz), 4);
            siz--;
            ioB.seekp(posB + 8);
            ioB.write(reinterpret_cast<char *> (&siz), 4);
            ioA.close();
            ioB.close();
            if (cz[o] > N) maintain(t);
        }

        void split(int posB,int siz)
        {
            ioB.seekg(posB + 4);
            int posA, cnt = 1, tmp;
            int Next, Ptr, Siz = siz - N;
            ioB.read(reinterpret_cast<char *> (&posA), 4);
            while (cnt < N)
            {
                getnext(posA);
                cnt++;
            }
            getnext(posA);
            siz = N;
            ioB.seekp(posB + 8);
            ioB.write(reinterpret_cast<char *> (&siz), 4);
            ioB.seekg(0, ios::end);
            int pos = ioB.tellg();
            ioB.seekg(posB);
            ioB.read(reinterpret_cast<char *> (&tmp), 4);
            ioB.seekp(posB);
            ioB.write(reinterpret_cast<char *> (&pos), 4);
            Next = tmp; Ptr = posA;
            ioB.seekp(0, ios::end);
            ioB.write(reinterpret_cast<char *> (&Next), 4);
            ioB.write(reinterpret_cast<char *> (&Ptr), 4);
            ioB.write(reinterpret_cast<char *> (&Siz), 4);
        }

        void merge(int posB)
        {
            int tmp, siz, Siz;
            ioB.seekg(posB + 8);
            ioB.read(reinterpret_cast<char *> (&siz), 4);
            ioB.seekg(posB);
            ioB.read(reinterpret_cast<char *> (&tmp), 4);
            if (tmp == -1) return;
            ioB.seekg(tmp + 8);
            ioB.read(reinterpret_cast<char *> (&Siz), 4);
            siz += Siz;
            ioB.seekg(tmp);
            ioB.read(reinterpret_cast<char *> (&tmp), 4);
            ioB.seekp(posB);
            ioB.write(reinterpret_cast<char *> (&tmp), 4);
            ioB.seekp(posB + 8);
            ioB.write(reinterpret_cast<char *> (&siz), 4);
        }

        void maintain(string t)
        {
            int o = t[0] - '0';
            if (cz[o] < N) return;
            ioA.open("fileA" + t, ios::binary | ios::in | ios::out);
            ioB.open("fileB" + t, ios::binary | ios::in | ios::out);
            cz[o] = 0;
            int posB = ST[o], siz;
            while (posB != -1)
            {
                ioB.seekg(posB + 8);
                ioB.read(reinterpret_cast<char *> (&siz), 4);
                if (siz >= 2 * N) split(posB, siz);
                if (siz <= N / 2) merge(posB);
                ioB.seekg(posB);
                ioB.read(reinterpret_cast<char *> (&posB), 4);
            }
            ioA.close();
            ioB.close();
        }

        void show(string t,char* ISBN, char* KEY, char s[4][45])
        {
            ioA.open("fileA" + t, ios::binary | ios::in | ios::out);
            ioB.open("fileB" + t, ios::binary | ios::in | ios::out);
            int o = t[0] - '0';
            int posA = st[o], posB = ST[o];
            char tmp[4][45], key[45];
            double v; int num;
            if (KEY[0] == '^')
            {
                while (posA != -1)
                {
                    ioA.seekg(posA + 4);
                    ioA.read(reinterpret_cast<char *> (&v), 8);
                    ioA.read(reinterpret_cast<char *> (&num), 4);
                    ioA.seekg(posA + 16 + 40);
                    for (int i = 0; i < 4; i++)
                        ioA.read(reinterpret_cast<char *> (tmp[i]), 40);
		            for (int i = 0; i < 4; i++)
                        printf("%s\t", tmp[i]);
                    printf("%.2lf\t%d本¾\n",v,num);
                    getnext(posA);
                }
                ioA.close();
                ioB.close();
                return;
            }
            find(ISBN, KEY, posA, posB);
            //printf("%d\n",st[o]);
            if (posA == -1) posA = st[o];else getnext(posA);
            while (posA != -1)
            {
                bool flag = 1;
                ioA.seekg(posA + 16);
                ioA.read(reinterpret_cast<char *> (key), 40);
                //printf("%d:\n",posA);
                //printf("%s\n%s\n",KEY,key);
                if (strcmp(KEY, key))
                {
                    ioA.close();
                    ioB.close();
                    return;
                }
                for (int i = 0; i < 4; i++)
                {
                    ioA.seekg(posA + 16 + 40 + i * 40);
                    ioA.read(reinterpret_cast<char *> (tmp[i]), 40);
                    if (i == 3) break;
                    if (s[i][0] != '^' && strcmp(tmp[i], s[i]))
                    {
                        flag = 0;
                        break;
                    }
                }
                if (s[3][0] != '^' && flag)
                {
                    bool ok = 0;
                    int len = strlen(tmp[3]), ll = 0;
                    for (int i = 0; i < len; i++)
                    {
                        if (tmp[3][i] == '|')
                        {
                            key[ll] = '\0';
                            if (!strcmp(key, KEY)) {ok = 1; break;}
                            ll = 0;
                            continue;
                        }
                        key[ll++] = tmp[3][i];
                    }
                    key[ll] = '\0';
                    if (!ok && !strcmp(key, KEY)) ok = 1;
                    if (!ok) flag = 0;
                }
                if (flag)
                {
                    ioA.seekg(posA + 4);
                    ioA.read(reinterpret_cast<char *> (&v), 8);
                    ioA.read(reinterpret_cast<char *> (&num), 4);
                    for (int i = 0; i < 4; i++)
                        printf("%s\t", tmp[i]);
                    printf("%.2lf\t%d本¾\n",v,num);
                }
                getnext(posA);
            }
            ioA.close();
            ioB.close();
        }
};
