#include <iostream>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <string>
using namespace std;
string CommandLine();
void InfoFileReading(char Array[],int &BitsPerChar,int &FileSize);
int BytesToBeRead(int FileSize,int BitsPerChar);
void BinaryFileReading(string filename,unsigned char *&cstream,int CmpBits);
void Unpack(unsigned char *cstream,int *&stream,int CmpBits,int TotalChars);
void store(string filename,char Array[],int *stream,int BitsPerChar,int FileSize);
void main()
{
	const int size=256;
	char Array[size]={'\0'};
	int BitsPerChar=0;
	int FileSize=0;
	unsigned char *cstream='\0';
	int *stream=0;
	string filename;
	filename=CommandLine();
	InfoFileReading(Array,BitsPerChar,FileSize);
	int TotalChars=FileSize*BitsPerChar;
	int CmpBits=BytesToBeRead(FileSize,BitsPerChar);
	BinaryFileReading(filename,cstream,CmpBits);
	Unpack(cstream,stream,CmpBits,TotalChars);
	store(filename,Array,stream,BitsPerChar,FileSize);
	cout<<"Congratulations! Decompression Successful";		
	getch();
}
string CommandLine()
{
	string command="\0";
	string filename="\0";
	int count=0;
	while(count<2)
	{
		cout<<endl<<"Enter Command:- ";
		cin>>command;
		if(command=="decompress")
			count++;
		else
			cout<<"Please Enter Valid Command."<<endl;		
		cin>>filename;
		ifstream fin;
		fin.open(filename);
		if(fin)
			count++;
		else
			cout<<"Could Not Open File."<<endl;
	}
	return filename;
}
void InfoFileReading(char Array[], int &BitsPerChar, int &FileSize)
{
	int Index=0;
	string Extra="\0";
	ifstream fin;
	fin.open("info.cmp",ios::in);
	if(!fin)
		cout<<"Could Not Open File.";
	else
	{
		fin>>Extra;
		fin>>BitsPerChar;
		fin>>Extra;
		fin>>FileSize;
		fin>>Extra;
		while(!fin.eof())
		{
			fin>>Index;
			fin.ignore(1);
			fin.get(Array[Index]);
		}
	}
	fin.close();
}
int BytesToBeRead(int FileSize, int BitsPerChar)
{
	int ReadBytes=(FileSize*BitsPerChar)/8;
	if(((FileSize*BitsPerChar)%8)!=0)
		ReadBytes++;
	return ReadBytes;
}
void BinaryFileReading(string filename,unsigned char *&cstream,int CmpBits)
{	
	cstream=new unsigned char[CmpBits];
	char *Temp='\0';
	ifstream fin;
	fin.open(filename,ios::in|ios::binary);		
	Temp=new char[CmpBits];
	fin.read(Temp,CmpBits);
	fin.close();	
	int i=0; 
	for(i=0;i<CmpBits;i++)
		cstream[i]=Temp[i];
	delete []Temp;
}
void Unpack(unsigned char *cstream,int *&stream,int CmpBits,int TotalChars)
{
	unsigned char mask=0x80;
	unsigned char ans=0x00;
	unsigned char temp=0;
	int i=0,k=0;
	stream=new int[TotalChars];
	for(i=0,k=0;i<TotalChars && k<CmpBits;k++)
	{
		temp=cstream[k];
		for(int j=0;j<8;j++)
		{
			ans=temp&mask;
			if(ans==mask)
				stream[i++]=1;
			else
				stream[i++]=0;			
			temp=temp<<1;
		}
	}
	cout<<endl;
}
void store(string filename,char Array[],int *stream,int BitsPerChar,int FileSize)
{
	int Decimal=0;
	int Power=0;
	ofstream fout;
	int position = filename.find ('.');
	filename.erase (position,4);
	filename=filename+".txt";
	fout.open(filename,ios::out);
	int j=0;
	for(int i=0;i<FileSize;i++)
	{
		Decimal=0;
		Power=BitsPerChar-1;
		for(int k=j;Power>=0; k++,Power--)
			Decimal=Decimal+(stream[k]*pow(2,Power));	
		j=j+BitsPerChar;
		fout<<Array[Decimal];
	}	
	fout.close();
}