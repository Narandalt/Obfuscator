#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

char **prefName;
char **lastName;
int count = 0;

char symbolGenerate();

void obfCode(int user)
{
    srand(time(NULL));

    FILE *op = fopen("code.txt", "r");
    FILE *wr = fopen("obf_code.txt", "w");

    char c;
    char var[20];
    int index = 0, size = 0;
    char string[20];
    bool t_count = false, was = false;

    prefName = (char**)malloc(sizeof(char*) * 1);
    prefName[0] = (char*)malloc(sizeof(char) * 20);

    lastName = (char**)malloc(sizeof(char*) * 1);
    lastName[0] = (char*)malloc(sizeof(char) * 20);

    while((c = (char)fgetc(op)) == '#') // ����� ��� include � ����
    {
        do
        {
            fwrite(&c, 1, 1, wr);
            c = (char)fgetc(op);
        } while(c != '\n');

        fwrite(&c, 1, 1, wr);
        t_count = 1;
    }

    if(user == 1 || user == 5)  /// ���������� ������ - �������
    {
        char name[10];
        char v1[10], v2[10], f[10];

        for(int i = 0; i < 10; i++)
        {
            name[i] = symbolGenerate();
            v1[i] = symbolGenerate();
            v2[i] = symbolGenerate();
            f[i] = symbolGenerate();
        }

        fprintf(wr, "void %s()\n{\nint %s = %d, %s = %d;\nfor(int %s = 0; %s < %s; %s++)\n%s *= %d;\n}\n", name, v1, rand() % 100, v2, rand() % 100, f, f, v2, f, v1, rand() % 10);
    }

    while(c != EOF) // pizdec
    {
        c = (char)fgetc(op);

        if(c == '\n') // �������� �������� �� ����� ������
        {
            if(user == 1 || user == 2)
            {
                if(t_count == false)
                {
                    fwrite(&c, 1, 1, wr);
                    t_count = true;
                }
            }
            else
            {
                fwrite(&c, 1, 1, wr);
            }
        }
        else if(c == '\t')
        {
            if(user == 1 || user == 2)
            {
                continue;
            }
            else
            {
                fwrite(&c, 1, 1, wr);
            }
        }
        else if(c == '/') // �������� ������������
        {
            if(user == 1 || user == 3)
            {
                c = (char)fgetc(op);

                if(c == '/')
                {
                    while(c != '\n')
                    {
                        c = (char)fgetc(op);
                    }

                    fwrite(&c, 1, 1, wr);
                }
                else
                {
                    fwrite("/", 1, 1, wr);
                    fwrite(&c, 1, 1, wr);
                }
            }
            else
            {
                fwrite(&c, 1, 1, wr);
            }
        }

        else /// ������ ����������� � �������������� ����������
        {
            if(c >= 97 && c <= 122)
            {
                var[index] = c;
                index++;
            }
            else
            {
                var[index] = '\0';
                size = index;
                index = 0;

                if(user == 1 || user == 4)
                {
                    if(strcmp(var, "int") == 0 || strcmp(var, "char") == 0 || strcmp(var, "bool") == 0)
                    {
                        //c = (char)fgetc(op);

                        if(c == ' ')
                        {
                            fwrite(&c, 1, 1, wr);
                            c = (char)fgetc(op);

                            while(c != ' ' && c != ';' && c != ',' && c != ')' && c != '(') // �������� ��� ����������
                            {
                                string[index] = c;
                                index++;
                                c = (char)fgetc(op);
                            }

                            string[index] = '\0'; /// ��� ���������� ������ � �����������

                            if(count != 0)
                            {
                                for(int i = 0; i < count; i++) // ��������� ��� ���������� �� ����� ��� �������
                                {
                                    if(strcmp(prefName[i], string) == 0) // ���� ������ ���������� ��� ����
                                    {
                                        for(int j = 0; j < index + 6; j++)
                                        {
                                            //lastName[count][j] = symbolGenerate(); ///
                                            fwrite(&lastName[i][j], 1, 1, wr);
                                        }

                                        was = true;

                                        break;
                                    }
                                }

                                if(was == false)
                                {
                                    strcpy(prefName[count], string);

                                    for(int j = 0; j < index + 6; j++)
                                    {
                                        lastName[count][j] = symbolGenerate();
                                        fwrite(&lastName[count][j], 1, 1, wr);
                                    }
                                }
                            }
                            else
                            {
                                strcpy(prefName[count], string);

                                for(int j = 0; j < index + 6; j++)
                                {
                                    lastName[count][j] = symbolGenerate();
                                    fwrite(&lastName[count][j], 1, 1, wr);
                                }
                            }

                            count++;
                            index = 0;
                            was = false;

                            prefName = (char**)realloc(prefName, sizeof(char*) * (count + 1));
                            prefName[count] = (char*)malloc(sizeof(char) * 20);

                            lastName = (char**)realloc(lastName, sizeof(char*) * (count + 1));
                            lastName[count] = (char*)malloc(sizeof(char) * 20);

                            memset(string, 0, 20); // ������� ������
                        }
                    }

                    /// ��� ������ ���� ������� �� ������ ����� ���������� � ������ ����� ����, � �� ������ ��� �������������

                }

                was = false;
                memset(var, 0, 20);
            }

            fwrite(&c, 1, 1, wr);
            t_count = 0;
        }
    }

    fseek(wr, -1, SEEK_END);
    fwrite(" ", 1, 1, wr);

    fclose(op);
    fclose(wr);
}

int main()
{
    system("chcp 1251");

    int user = 0;

    printf("\n1. ������������ ��� �����������\n2. ������� ��� ������ ������ � ���������\n3. ������� �����������\n4. ������������� ��� ���������� � �������\n5. �������� �������� ���������� � ���������\n\n-> ");
    scanf("%d", &user);

    obfCode(user);
    system("pause");
}

char symbolGenerate()
{
    char s;
    int s_int = 97 + rand() % (123 - 97);

    return s_int;
}