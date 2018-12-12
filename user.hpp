#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;

class user
{
    private:
        fstream io;
        int cnt = 0;
    public:
        user()
        {
            io.open("user_list");
            if (!io)
            {
                ofstream out("user_list", ios::binary);
                char s[3][35];
                int pow = 7;
                strcpy(s[0], "root");
                strcpy(s[1], "sjtu");
                strcpy(s[2], "root");
                for (int i = 0; i < 3; i++)
                    out.write(reinterpret_cast<char *> (s[i]), 30);
                out.write(reinterpret_cast<char *> (&pow), 4);
                out.close();
            }
            else io.close();
        }

        void adduser(char s[3][45], int pow)
        {
            char tmp[45];
            io.open("user_list", ios::binary | ios::in | ios::out);
            io.seekg(0, ios::end);
            int End = io.tellg();
            int pos = 0;
            while (pos < End)
            {
                io.seekg(pos);
                io.read(reinterpret_cast<char *> (tmp), 30);
                if (!strcmp(tmp, s[0]))
                {
                    puts("Invalid");
                    io.close();
                    return;
                }
                pos += 94;
            }
            io.seekp(0,ios::end);
            for (int i = 0; i < 3; i++)
                io.write(reinterpret_cast<char *> (s[i]), 30);
            io.write(reinterpret_cast<char *> (&pow), 4);
            io.close();
        }

        void Delete(char* user)
        {
            io.open("user_list", ios::in | ios::out);
            io.seekg(0, ios::end);
            char tmp[45];
            int pos = 0;
            int End = io.tellg();
            while (pos < End)
            {
                io.seekg(pos);
                io.read(reinterpret_cast<char *> (tmp), 30);
                if (!strcmp(user, tmp))
                {
                    io.seekp(pos);
                    strcpy(tmp, "^^$$%%^^^");
                    io.write(reinterpret_cast<char *> (tmp), 30);
                    io.close();
                    return;
                }
                pos += 94;
            }
            puts("Invalid");
            io.close();
        }

        void passwd(char* user, char* old_key, char* new_key, int pow)
        {
            io.open("user_list", ios::in | ios::out);
            io.seekg(0, ios::end);
            char tmp[45];
            int pos = 0;
            int End = io.tellg();
            while (pos < End)
            {
                io.seekg(pos);
                io.read(reinterpret_cast<char *> (tmp), 30);
                if (!strcmp(user, tmp))
                {
                    io.read(reinterpret_cast<char *> (tmp), 30);
                    if (pow != 7 && strcmp(tmp, old_key))
                        puts("Invalid");
                    else
                    {
                        io.seekp(pos + 30);
                        io.write(reinterpret_cast<char *> (new_key), 30);
                    }
                    io.close();
                    return;
                }
                pos += 94;
            }
            puts("Invalid");
            io.close();
        }

        int login(char* user, char* key, int pow)
        {
            io.open("user_list",ios::binary | ios::in | ios::out);
            io.seekg(0, ios::end);
            char tmp[45];
            int pos = 0;
            int End = io.tellg();
            while (pos < End)
            {
                io.seekg(pos);
                io.read(reinterpret_cast<char *> (tmp), 30);
                if (!strcmp(user, tmp))
                {
                    io.read(reinterpret_cast<char *> (tmp), 30);
                    io.seekg(pos + 90);
                    int pw;
                    io.read(reinterpret_cast<char *> (&pw), 4);
                    //printf("%s\n%s\n%s\n%d\n",user,key,tmp,pw);
                    if (pw > pow && strcmp(key, tmp))
                    {
                        io.close();
                        return 0;
                    }
                    io.close();
                    return pw;
                }
                pos += 94;
            }
            puts("TAT");
            io.close();
            return 0;
        }

};
