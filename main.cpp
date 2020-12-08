#include <iostream>
#include <string>
#include <cstring>
#include <fstream>


using namespace std;

short avail_list;

struct Book///max size of record
{
    char ISBN[5];///5
    string title,author;///32 total -> (5 < title < 16) && author < 32 - size of title;
    float price;///4
    short year,pages_num;///2 + 2 = 4
};
istream& operator>>(istream&,Book&);
ostream& operator<<(ostream&,const Book&);
void write_book(fstream&,Book&);
void read_book(fstream&,Book&);
void add_book(fstream&);
void print_book(fstream&,char[5]);
void print_All(fstream&);
void update_book(fstream&,char[5]);
void delete_book(fstream&,char[5]);
void compact_file(fstream&,string);

int main()
{
    short intialize = -1;
    fstream file("new try - Copy.txt",ios::in|ios::out);
    //file.write((char*)&intialize,2);
    //add_book(file);
    //add_book(file);
 //delete_book(file,"111");
 print_All(file);
    return 0;
}
void write_book(fstream& file,Book& b)
{
    file.write(b.ISBN,5);
    char rec_buff[34];/// 32 + 2
    strcpy(rec_buff,b.title.c_str());strcat(rec_buff,"$");
    strcat(rec_buff,b.author.c_str());strcat(rec_buff,"$");
    file.write(rec_buff,strlen(rec_buff));
    file.write((char*)&b.price,sizeof(b.price));
    file.write((char*)&b.year,sizeof(b.year));
    file.write((char*)&b.pages_num,sizeof(b.pages_num));
    short rec_buff_len = strlen(rec_buff);
    if(rec_buff_len <  34)
    {
        strcpy(rec_buff," ");
        for(short i = 0;i < (33 - rec_buff_len);i++)
            strcat(rec_buff," ");
    }
    file.write(rec_buff,strlen(rec_buff));
}
void read_book(fstream& file,Book& b)
{
    file.read(b.ISBN,sizeof(b.ISBN));
    while(b.ISBN[0] == '*' && !file.fail())
    {
        file.seekg(42,ios::cur);
        file.read(b.ISBN,sizeof(b.ISBN));
    }
    getline(file,b.title,'$');
    getline(file,b.author,'$');
    file.read((char*)&b.price,sizeof(b.price));
    file.read((char*)&b.year,sizeof(b.year));
    file.read((char*)&b.pages_num,sizeof(b.pages_num));
    short padding = 32 - (b.title.size() + b.author.size());
    file.seekg(padding,ios::cur);
}
void add_book(fstream& file)
{
    Book added_book;
    cin >> added_book;
    if(file.fail())
        file.clear();
    file.seekg(0,ios::beg);
    file.read((char*)&avail_list,sizeof(avail_list));
    if(avail_list != -1)
    {
        file.seekg(avail_list*47 + 1,ios::cur);///as it pass first char of id that might be *
        file.read((char*)&avail_list,sizeof(avail_list));
        file.seekp(-3,ios::cur);
        write_book(file,added_book);
        file.seekp(0,ios::beg);
        file.write((char*)&avail_list,sizeof(avail_list));
        return;
    }
    file.seekp(0,ios::end);
    write_book(file,added_book);
}
void print_book(fstream& file,char ISBN[5])
{
    if(file.fail())
        file.clear();
    file.seekg(0,ios::beg);
    file.read((char*)&avail_list,sizeof(avail_list));
    Book temp_book;
    while(true)
    {
        read_book(file,temp_book);
        if(file.fail())
            break;
        else if(!strcmp(temp_book.ISBN,ISBN))
        {
            cout << temp_book;
            return;
        }
    }
    cout << "Book of ISBN : " << ISBN << " is not found!!!" << endl;
}
void print_All(fstream& file)
{
    if(file.fail())
        file.clear();
    file.seekg(2,ios::beg);
    Book temp_book;
    while(true)
    {
        read_book(file,temp_book);
        if(file.fail())
            break;
        cout << temp_book;
    }
}
void update_book(fstream& file,char ISBN[5])
{
    Book update;
    cin >> update;
    if(file.fail())
        file.clear();
    file.seekg(0,ios::beg);
    file.read((char*)&avail_list,sizeof(avail_list));
    Book temp_book;
    while(true)
    {
        read_book(file,temp_book);
        if(file.fail())
            break;
        else if(!strcmp(temp_book.ISBN,ISBN))
        {
            file.seekp(-47,ios::cur);
            write_book(file,update);
            return;
        }
    }
    cout << "Book of ISBN : " << ISBN << " is not found!!!" << endl;
}
void delete_book(fstream& file,char ISBN[5])
{
    if(file.fail())
        file.clear();
    file.seekg(0,ios::beg);
    file.read((char*)&avail_list,sizeof(avail_list));
    short RRN = -1;
    Book temp_book;
    while(true)
    {
        read_book(file,temp_book);
        if(file.fail())
            break;
        RRN++;
        if(!strcmp(temp_book.ISBN,ISBN))
        {
            file.seekp(-47,ios::cur);
            file.put('*');
            file.write((char*)&avail_list,sizeof(avail_list));
            file.seekg(0,ios::beg);
            file.write((char*)&RRN,sizeof(RRN));
            return;
        }
    }
    cout << "Book of ISBN : " << ISBN << " is not found!!!" << endl;
}
void compact_file(fstream& file,string ph_file_name)
{
    fstream temp("temp.txt",ios::in|ios::out|ios::trunc);
    short intialize = -1;
    temp.write((char*)&intialize,2);
    if(file.fail())
        file.clear();
    file.seekg(2,ios::beg);
    Book temp_book;
    while(true)
    {
        read_book(file,temp_book);
        if(file.fail())
            break;
        write_book(temp,temp_book);
    }
    file.close();temp.close();
    remove(ph_file_name.c_str());rename("temp.txt",ph_file_name.c_str());
    file.open(ph_file_name.c_str(),ios::in|ios::out);
}
istream& operator>>(istream& in,Book& b)
{
    cout << "Enter Book Data" << endl;
    cout << "Enter ISBN : " ;
    in >> b.ISBN ;
    cout << "Enter Title : " ;
    in.ignore();
    getline(in,b.title);//in >> b.title ;
    if(b.title.size() > 16)
        b.title.erase(16);
    cout << "Enter Author : " ;
    getline(in,b.author);//in >> b.author ;
    if(b.author.size() > 16)
        b.author.erase(16);
    cout << "Enter Price : " ;
    in >> b.price ;
    cout << "Enter Year : " ;
    in >> b.year ;
    cout << "Enter Number of pages : " ;
    in >> b.pages_num;
    return in;
}
ostream& operator<<(ostream& out,const Book& b)
{
    out << "ISBN : " << b.ISBN << "\nTitle : " << b.title << "\nAuthor : " << b.author << "\nPrice : " << b.price
    << "\nYear : " << b.year << "\nNumber of Pages : " << b.pages_num << endl;
    return out;
}
