/*
* MemoryLeakCheck.cpp
*
* Trace���г�Ա�����Ķ���,����new,delete�Ķ���
*
* Author:����Ȼ
*
* Date:2017/5/25
*/

#include "MemoryLeakCheck.h"
#include <fstream>								
//�����Ľ�����뵽���ļ�log.txt��
#include <iostream>


Trace m_trace;									
//��m_trace��ȫ��ʵ������

Trace::~Trace()									
//��Trace����������
{
	checkMap();					                
//����������ִ�е�ʱ�����checkMap()����������Ƿ����ڴ�й©
	if (on==true)				                
//����������ִ�е�ʱ�򣬼�������������ʱ�򣬽�on��Ϊfalse
		on=false;
}

void Trace::lock ()								
/*��Lock�Ĺ��캯��ִ�еĺ���lock��������Lock�ཨ��ʱ,
m_locktimes���1*/
{
	m_locktimes++;
}

void Trace::unlock ()				          
/*��Lock����������ִ�еĺ���unlock��������Lock�౻����ʱ
,m_locktimes���1*/
{
	m_locktimes--;
}


void Trace::addMap(void *p,char *m_file,int m_line)	
//�����ݽṹmap������µ�����
{  
	    if(m_locktimes>0)						
//��������£�һ��ʼ�ɹ��캯�� ��ʼ��ʹ��m_locktimes==0
			return;
		Trace::Lock locker(*this);			   
/*��������˵���ѭ��,ÿ�ε�����ʹm_locktimes++����ô�����
���(m-locktimes>0)Ϊ�棬����������˵�����Ϊ������������
ֱ��return*/
		m_map[p] = Entry (m_file,m_line);
}

void Trace::deleteMap(void *p)
{
	if (m_locktimes>0)							
/*������Ĩȥm_map�е�����ʱ��erase���������delete��ɾ�����ݣ�
��ô�Ϳ��ܲ���һ����ѭ���ĵ�����*/
		return ;
	Trace:: Lock locker(*this);			        
/*���û�ʹ��delete����ʱ��ͨ��if�ж�(û��ֱ��return)ִ��
deleteMap��������ʱm_locktimes==0,���ǵ��˴�ʹ��m_locktimes==1��
����һ�ε������˳�*/
	cout<<"delete memory,"<<m_map[p].File<<",line "
		<<m_map[p].Line<<endl;
	m_iterator temp= m_map.find(p);
	m_map.erase(temp);						    
/*Ȼ�����erase������erase�������ٴε���delete�����������
�ٴν���deleteMap����������m_locktimes==1��ֱ�ӷ���,Ȼ��
��һ��ִ��free������Ȼ�����erase������Ȼ���ٷ���ԭ����
delete����������ڴ�ռ���ͷ�*/
}

void Trace::checkMap()
{
	void *temp=NULL;
	ofstream out("log.txt",ofstream::app);		
//�����ļ����������ÿ���ڴ�й©�ļ������¼���ļ�log.txt��
	out<<__DATE__<<"\t"<<__TIME__<<endl;		
//����������еľ���ʱ��
	if (m_map.size())						    
/*�ڳ������ʱ�����m_mapû����գ���ô˵�����û������
�ڴ�ռ�û�б��ͷţ����������ڴ�й©*/
	{
		cout<<"Memory Leak Detected!"<<endl;
		out<<"Memory Leak Detected!"<<endl;
		for(m_iterator it =m_map.begin();it!=m_map.end();
			it=m_map.begin())	
/*��⵽���������ڴ�й©�����Զ��ͷ��û������ͷŵ��ڴ�*/
		{
			cout<<"file:"<<it->second.File<<",line:"
				<<it->second.Line<<endl;
			out<<"file:"<<it->second.File<<",line:"
				<<it->second.Line<<endl;
			temp=it->first;															
			deleteMap(temp);					
			//ɾ��m_map������
			free(temp);			                
			//ɾ���û�����Ŀռ�
		}
		cout<<"Leak memory released!"<<endl;
		out<<"Leak memory released!"<<endl;
	}
	else								        
		//���m_mapΪ�գ���ô˵��û�з����ڴ�й©
		{
			cout<<"Memory Leak Not Detected!"<<endl;
			out<<"Memory Leak Not Detected!"<<endl;
		}
	out<<endl;
}

int Trace::getMapSize()
{
	return m_map.size();					   
	/*Ϊ�����ݰ�ȫ����m_map������Ϊprivate��
	��public��getMapSize��������ȡm_map�ĳ���*/
}

void* operator new (size_t size,char *m_file,int m_line)
{
	void* p = malloc (size);
	if (m_trace.on==true)				       
	/*on==true��ʱ�򣬱�ʾ�������������У�
	���on��Ϊfalse��������if�е���䡣
	�����ܳ�����main֮�󻹻����new����ʱ��
	Ҫʹnew�ָ�ԭ�������ӣ�*/
		m_trace.addMap(p,m_file,m_line);		
	//��m_map�в�������
	return p;
}

void* operator new[] (size_t size,char *m_file,int m_line)		 
	//������new
{
	void* p = malloc (size);
	if (m_trace.on==true)
		m_trace.addMap(p,m_file,m_line);
	return p;
}

void operator delete (void *p)																
{
	if (m_trace.on==true&&m_trace.getMapSize())		
		/*��m_map�ĳ���Ϊ0ʱ�����õ��������Լ�
		��deleteMap������������ܻ���main������
		�������delete,�������m_map��û�д���
		��ص���Ϣ����˲���ִ��deleteMap������
		ͬʱ�������������ж��ֿ��Ա�֤�û��������
		�涼���Ա�ִ�е�*/
		m_trace.deleteMap (p);
	free(p);
}
 
void operator delete[] (void *p)				  
	//delete����
{
	if (m_trace.on==true&&m_trace.getMapSize())
		m_trace.deleteMap(p);
	free(p);
}

void* operator new (size_t size)
//һЩ������������new����ʹnew����Ҳ����������ذ汾��new��
{
	void* p = malloc (size);
	if (m_trace.on==true)			
		m_trace.addMap(p,"?",0);		
	return p;
}

