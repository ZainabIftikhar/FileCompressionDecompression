#include <iostream>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <string>
using namespace std;
string CommandLine(int argc2, char *argv2[]); 
int UniqueChars(char Array[],int &TotalChars,string filename);	
int BitsRequired(int Uniques);	
void StoreStream (char Array[], int Bits, unsigned int* & Stream, string filename);  
int BinaryConversion (int index, int &power); 
int BinaryLength(int Num);  
int CompressedBitsRequired (int TotalBits);
void PackBits(int &TotalBits,unsigned int* & Stream,char *&Cstream,int &CmpBits);
void BinaryFiling(int CmpBits,char *Cstream, string filename); 
void InfoStorage(char Array[],int Bits,int TotalChars);  
void CompressRatio(int TotalChars, int CmpBits);  
void main(int argc,char *argv[])
{
	char Array[256]={'\0'};
	int Uniques=0;
	int TotalBits=0;
	int Bits=0;
	int TotalChars=0;
	int CmpBits=0;
	unsigned int * Stream;
	char * Cstream;	
	argc=2;
	argv[0]="compress";
	string filename;	
	filename=CommandLine(argc, argv); 
	Uniques=UniqueChars(Array,TotalChars,filename);     
	Bits=BitsRequired(Uniques);    
	TotalBits=TotalChars*Bits;
	Stream=new unsigned int[TotalBits];
	StoreStream (Array,Bits,Stream,filename);       
	CmpBits=CompressedBitsRequired (TotalBits);
	Cstream=new char[CmpBits];
	PackBits(TotalBits,Stream,Cstream,CmpBits);
	BinaryFiling(CmpBits,Cstream,filename);
	InfoStorage(Array,Bits,TotalChars);
	CompressRatio(TotalChars,CmpBits);	
	cout<<endl<<"Congratulations! Compression Successful"<<endl;
	getch();
}
string CommandLine(int argc2, char *argv2[])
{
	string command="\0";
	string filename="\0";
	int count=0;
	while(count<argc2)
	{
		cout<<endl<<"Enter Command:- ";
		cin>>command;
		if(command==argv2[0])
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
int UniqueChars(char Array[],int &TotalChars,string filename)  
{
	char ch='\0';	
	int Uniques=0;
	int count=0;
	ifstream fin;
	fin.open(filename,ios::in);
	while(!fin.eof())
	{
		count=0;
		ch=fin.get();	
		for(int i=0;Array[i]!='\0';i++)
		{
			if(Array[i]==ch)
					count++;
		}
		if(count==0)
		{
			Array[Uniques]=ch;
			Uniques++;
		}			
		TotalChars++;
	}
	TotalChars--;
	fin.close();
	return Uniques-1;
}
int BitsRequired(int Uniques)    
{
	int Ceiling=(log(Uniques))/(log(2));
	float Bits=(log(Uniques))/(log(2));
	if(Ceiling<Bits)
		Ceiling++;
	return Ceiling;
}
void StoreStream (char Array[], int Bits, unsigned int* & Stream, string filename) 
{
	char ch;
	int binary=0;
	int length=0;
	int power=0;	
	int index=0;
	ifstream fin;
	fin.open(filename,ios::in);	
	ch=fin.get();
	while(!fin.eof())
	{
		power=0;
		for(int i=0; Array[i]!='\0'; i++)
		{
			if(Array[i]==ch)
			{
				binary=BinaryConversion(i,power);					
				length=BinaryLength (binary);
				for(int i=0; i<(Bits-length); i++)
				{
					Stream[index]=0;
					index++;
				}							
				for(int i=binary; power>=0; )
				{
					int p=(pow(10,power));
					int divident=i/p;
					i=i%p;
					power--;
					Stream[index]=divident;
					index++;
				}
			}
		}
		ch=fin.get();			
	}			
	fin.close();
}
int BinaryConversion (int index, int &power)
{
	int binary=0;	
	int remainder=0;
	while(index>0)
	{
		remainder=index%2;		
		binary=binary+ (remainder* (pow (10,power)));
		power++;
		index=index/2;
	}
	power=power-1;
	return binary;
}
int BinaryLength (int Binary)
{
	int count=0;
	while(Binary>0)
	{
		Binary=Binary/10;
		count++;
	}
	return count;
}
void PackBits(int &TotalBits,unsigned int* & Stream,char *&Cstream,int &CmpBits)
{
	int count=0;	
	for(int k=0;k<CmpBits;k++)
	{
		Cstream[k]=0x00;
	}
	for(int i=0;i<CmpBits;i++)
	{
		unsigned char mask=0x80;
		unsigned char ans=0x00;
		for(int j=0;j<8&&count<TotalBits;j++,mask=mask>>1,count++)
		{
			if(Stream[count]==1)
				ans=ans|mask;
		}
		Cstream[i]=ans;
	}	
}
int CompressedBitsRequired (int TotalBits)
{
	int CmpBits=TotalBits/8;
	if((TotalBits%8)!=0)
	{
		CmpBits++;
	}
	return CmpBits;
}
void BinaryFiling(int CmpBits,char *Cstream,string filename)
{
	int position = filename.find ('.');
	filename.erase (position,4);
	ofstream fout;
	fout.open (filename+".cmp", ios::out | ios::binary);
	if(fout)
		for(int i=0; i<CmpBits; i++)
		{
		char byte=Cstream[i];
		fout.write(&byte,1);
		}	
	else
	{
		cout<<"ERROR";
	}
	fout.close();
}
void InfoStorage(char Array[],int Bits,int TotalChars)
{
	remove("info.cmp");
	ofstream fout;
	fout.open("info.cmp",ios::out);
	fout<<"TotalBits: "<<Bits<<endl;
	fout<<"FileSize: "<<TotalChars<<endl;
	fout<<"Table:"<<endl;
	for(int i=0;Array[i]!='\0';i++)
	{
		fout<<i<<"\t"<<Array[i]<<endl;
	}
	system("attrib +h info.cmp");
	fout.close();
}
void CompressRatio(int TotalChars, int CmpBits)
{
	float Ratio=0;
	float m=CmpBits;
	float n=TotalChars;
	Ratio=(1-(m/n))*100;
	cout<<endl<<"Compression Ratio: "<<Ratio<<"%"<<endl;
}
