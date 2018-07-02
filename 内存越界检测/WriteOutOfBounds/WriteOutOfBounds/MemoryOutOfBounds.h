/*
* MemoryOutOfBounds.h
*
* WriteCheck���Ա�����������Լ�new��������
*
* Author:����Ȼ
*
* Date:2017/5/27
*/
#ifndef MEMORYOUTOFBOUNDS_H_
#define MEMORYOUTOFBOUNDS_H_

#include <map>
using namespace std;

class WriteCheck
{
public:
	class Entry										 
	//Entry��������¼�����ڴ����Ϣ
	{
	public:
		char *File;									 
		//������¼�����ڴ�й©��Դ�ļ�����
		int Line;									  
		//������¼����дԽ���Ӧ��new��������
		int Length;									  
		//������¼new����size_t�Ĵ�С
		Entry():Line(0),Length(0){}					  
		//Ĭ�Ϲ��캯��
		Entry(char m_file[],int m_line,int m_length)
			:Line(m_line),File(m_file),Length(m_length){}		
		//���ع��캯��
		~Entry(){}									  
		//��������
	};
private:											
	map<void*,Entry > m_map;						 
	//��¼�ڴ�����map����ֵΪnew������ڴ��ַ����Ӧ��ֵΪ��Entry//
	typedef map<void*,Entry>::iterator m_iterator;	 
	//������map<void*,Entry>�����������֣�����ʹ��
public:
	bool on;										 
	/*������ʾWriteOut���ȫ�ֶ����Ƿ����,true��ʾδ����,
	false��ʾ��������ִ�У�������������*/
	void addMap (void *p,char* m_file,int m_line,int m_length);	
	//�������map�е���Ϣ
	void checkMap ();								 
	/*�������map�е�EntryԪ��,�����ʼֵ���޸ģ�
	����дԽ�磬����û�У������л㱨*/
	WriteCheck():on(true){}						   
	//����ʼ�󣬻Ὣon��Ϊtrue��
    ~WriteCheck();
};

			
void* operator new[] (size_t size,char *m_file,int m_line);	
 //operator new[]����,char* m_file���ļ���,int m_line������
void* operator new (size_t size,char *m_file,int m_line);
//operator new����ֻ����ʽ�������Է��Ϻ궨��
#define EXTRALENGTH 100				  
//������ڴ�ĳ���


#endif