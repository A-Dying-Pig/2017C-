/*
* define.h
*
* new�ĺ궨��
*
* Author:����Ȼ
*
* Date:2017/5/25
*/

#define new new(__FILE__, __LINE__)	
/*�궨��,��new�滻�����Ƕ����
operator new (size_t size,char *m_file,int m_line)����
�õ���__FILE__,__LINE__������ȡ��ǰ�ļ����͵�ǰ����*/
									