/*
* MemoryOutOfBounds.cpp
*
* WriteCheck���Ա�����Ķ����Լ�new���ض���
*
* Author:����Ȼ
*
* Date:2017/5/27
*/
#include "MemoryOutOfBounds.h"
#include <iostream>
#include <fstream>

WriteCheck m_writecheck;									
//��WriteCheck��ȫ�ֶ���

WriteCheck::~WriteCheck()				
{
	checkMap();												
	//ִ��m_map�ļ��
	if (on==true)	
		on=false;											
	//��������������Ϊfalse
}

void WriteCheck::addMap(void *p,char* m_file,int m_line,int m_length)
{
	m_map[p] = Entry(m_file,m_line,m_length);				
	//��m_map�������Ϣ
	unsigned char* temp =(unsigned char *)p;				
	//�������ڴ���unsigned char����ʼ��һ�� unsigned char����ռ1�ֽ�//
	int start = m_length / sizeof (unsigned char),			
	//����������ڴ�ת����unsigned char����±귶Χ//
	end = (m_length+EXTRALENGTH) / sizeof (unsigned char);
	for (int i=start;i<end;i++)		  //��ʼ���������ڴ棬ȫ����Ϊ0
		temp[i] = 0;
}


void WriteCheck::checkMap()
{
	ofstream out("LOG.txt",ofstream::app);	  //����ļ���
	unsigned char *temp=NULL;								
	//������void *ת��Ϊunsigned char*
	bool first = false;						  //һ��������ƿ���
	int start=0,end=0,i=0;
	for (m_iterator it = m_map.begin();it!=m_map.end();it++)
	{
		temp = (unsigned char *)it->first;					
		//��ȡm_map��Entry�洢���ڴ��ַ
		start = it->second.Length / sizeof(unsigned char);  
		//�����±귶Χ
		end = ( it->second.Length + EXTRALENGTH )
			   /sizeof (unsigned char);
		for (i = start;i<end;i++ )			  //����ʼֵ�Ƿ�ı�
			if (temp[i]!=0)
				break;
		//�������
		if (i!= end)
		{
			if (first==false)
				{
					cout<<"Write memory out of bounds DETECTED!"<<endl;
					out<<__DATE__<<"\t"<<__TIME__<<endl<<
						"Write memory out of bounds DETECTED!"<<endl;
					first = true;
				}
			cout<<"file:"<<it->second.File<<"\tline:"
				<<it->second.Line<<endl;
			out<<"file:"<<it->second.File<<"\tline:"
				<<it->second.Line<<endl;
		}
	}
	if (first==false)
	{
		cout<<"Write memory out of bounds NOT DETECTED!"<<endl;
		out<<__DATE__<<"\t"<<__TIME__<<endl<<
			"Write memory out of bounds NOT DETECTED!"<<endl;
	}
	out<<endl;
}


void* operator new[] (size_t size,char *m_file,int m_line)
{
	void* p = malloc (size+EXTRALENGTH);				 
	//�����EXTRALENGTH�ֽڵĳ��ȵ��ڴ�
	if (m_writecheck.on==true)							 
	//���main����û�н���
		m_writecheck.addMap(p,m_file,m_line,size);
	return p;
}

void* operator new (size_t size,char* m_file,int m_line) 
	//��ʽ�ϵ�����Ϊ�˷���define�궨��
{
	void *p = malloc(size);
	return p;
}