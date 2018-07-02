 /*
* MemoryLeakCheck.h
*
* Trace���г�Ա����������,����new,delete������
*
* Author:����Ȼ
*
* Date:2017/5/25
*/
#ifndef MEMORYLEAKCHECK_H_
#define MEMORYLEAKCHECK_H_

#include <map>
using namespace std;


class Trace
{
public:
	class Entry														
		//Entry��������¼�����ڴ����Ϣ
	{
	public:
		char *File;													
		//������¼�����ڴ�й©��Դ�ļ�����
		int Line;													
		//������¼δ����delete�ͷ��ڴ��new��������
		Entry():Line(0){}											
		//Ĭ�Ϲ��캯��
		Entry(char m_file[],int m_line):Line(m_line),File(m_file){}	
		//���ع��캯��
		~Entry(){}													
		//��������
	};

	class Lock														
	/*Lock����������delete��map��
	�������ʱʹ��delete��ɸ���*/
	{
	public:
		Lock(Trace &tracer):_trace(tracer){_trace.lock();}
		~Lock(){_trace.unlock();}
	private:
		Trace& _trace;												
		//��Ա��������Trace������
	};

private:
	int m_locktimes;												
	//�����ж��ǲ���map���õ�delete
	map<void*,Entry > m_map;										
	/*��¼�ڴ�����map����ֵΪnew�����
	�ڴ��ַ����Ӧ��ֵΪ��Entry������new
	���ڵ��ļ�������*/
	typedef map<void*,Entry>::iterator m_iterator;				   
	//������map<void*,Entry>������������
public:
	int getMapSize();												
	//������ȡm_map�ĳ���
	bool on;														
	/*������ʾTrace�Ķ����Ƿ����,��
	main�����Ƿ����,true��ʾδ����,
	false��ʾ��������ִ�У�������������*/
	void lock();													
	//����ʹm_locktimes++
	void unlock();													
	//����ʹm_locktimes--
	void addMap (void *p,char* m_file,int m_line);					
	//�������map�е���Ϣ
	void deleteMap (void *p);										
	//����ɾ��map�е���Ϣ	
	void checkMap ();												
	/*�������m_map�е��Ƿ����Ԫ��, 
	�����л㱨,��������ڴ�й©��
	�ͷ�ռ���ڴ�*/
	Trace():on(true),m_locktimes(0){}						
	/*����ʼ�󣬻Ὣon��Ϊtrue,
	m_locktimes��ֵ��Ϊ0������on==true��
	��������δ����*/
    ~Trace();
};


void* operator new (size_t size,char *m_file,int m_line);			
/*����new�����,char* m_file���ļ���,int m_line������*/
void* operator new[] (size_t size,char *m_file,int m_line);
void* operator new (size_t size);
void operator delete (void *p);
void operator delete[] (void *p);

#endif
