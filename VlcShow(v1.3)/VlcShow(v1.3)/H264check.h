#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct ParseContext
{   
    unsigned int FrameStartFound;    
    unsigned int iFrameLength;    

} ParseContext; 
signed int DecLoadAU(unsigned char* pStream, unsigned int iStreamLen,int fpsNum, ParseContext *pc)
    {   
        unsigned int i;   
        unsigned int state = 0xffffffff;   
        if( NULL == pStream )   
        {   
            return -1;   
        }   
		int j=0;
        for( i = 0; i < iStreamLen; i++)   
        {   
            /* ����nal����Ϊ1��5��nalͷ */ 
            if( (state & 0xFFFFFF1F) == 0x101 || (state & 0xFFFFFF1F) == 0x105 )   /*�ٶȲ������գ�101*/
			{   
                if (i >= iStreamLen) /* ����Bufferβ�����˳�����ѭ�� */ 
                {   
                    break;   
                }if( pStream[i] & 0x80) /* ����first_mb_in_sliceΪ0��sliceͷȷ��һ��ͼ��Ŀ�ʼ */ 
                {   
                    if(pc->FrameStartFound) /* ���ҵ���һ��ͼ��Ŀ�ʼ�Ϳ���ȷ��ͼ�����ʼ�ͽ��� */ 
                    {   
                        if (j<fpsNum)
						{
							j++;
							continue;
						}
						else
						{
						    pc->iFrameLength = i - 3;   
                            pc->FrameStartFound = 0;   
                            state = 0xffffffff;  
							
                            return 0; /* �ҵ�һ��ͼ��ı߽緵��0 */
						}
                    }   
                    else 
                    {   
                        pc->FrameStartFound = 1;   
                    }   
                }   
            }   
            if (i < iStreamLen)   
            {   
                state = (state << 8) | pStream[i];  
            }   
        }   
        pc->FrameStartFound = 0;   
        return -1; /* û���ҵ�AU�߽緵��-1 */ 
    }

int dingwei(FILE *fp,int k)			//!��λNAL����ʼλ�õĺ���
{   char buf[2];
    int i=0;
	fseek(fp,k,SEEK_SET);		//!ָ�붨λ���ӿ�ͷ��ʼ
	for(;feof(fp)==0;)
		{
			fread(buf,1,1,fp) ;
			k++;
			if(i<4)
			{
				if(buf[0]==0)   {i=i+1;}
				else		{i=0;}
			}
			if(i==4&&buf[0]==1)
			{
		    	return k;					//!��ʱ��λ�ڿ�ʼ��0x000001��01��
			}
	}

exit(-1);
}

char NALU(FILE *fp,int dis,unsigned char nal_unit_type)	//!�ж�NALU�����͵ĺ���
{	unsigned char buf1[2];
	fseek(fp,dis,SEEK_SET);
	fread(buf1,1,1,fp);
	nal_unit_type=(buf1[0]&0x1f);					//!ȡbuf1[0]�еĵ���λbit
	//!���Ƭ����
	return nal_unit_type;
}

char slice(FILE *fp,int k)		      //!�ж�Ƭ���͵ĺ���
{  // FILE *fp;
	int j=0;
	unsigned char buf1[2];
	int count=0;
	char slice_type=0;
	//fp=fopen(FILE_PATH,"rb+");
	fseek(fp,k,SEEK_SET);
	fread(buf1,1,1,fp);
	buf1[1]=buf1[0];			             //!���º���������ɵ���Golomb�ر�����Ϣ��ȡ
	for(count=0;count<2;count++)
	{	buf1[0]=buf1[1];
		buf1[0]<<=j;
		buf1[1]=buf1[0];
		for(j=1;(buf1[j]&0x80)!=0x80;j++)    //!jΪ0�ļ���
			{buf1[j]<<=1;}
		if(count==1)   
		{	
			slice_type=(buf1[1]>>=(9-2*j));
			slice_type=slice_type-1;
		}
	}
  return slice_type;
}
char sliceEx(FILE *fp,int k)
{
	int j=0;
	unsigned char buf1[2];
	int count=0;
	char slice_type=0;
	//fp=fopen(FILE_PATH,"rb+");
	fseek(fp,k,SEEK_SET);
	fread(buf1,1,2,fp);
	if (buf1[1]&0x80==0x80)
	{return 1;}
	else
	{return 0;}

}
int count_long(int dis,FILE *fp)	//!�����֡�ĳ��ȵĺ��������ֽ�Ϊ��λ����
{	char buf[2];int i=0;int count=0;
	fseek(fp,dis,SEEK_SET);

	for(;feof(fp)==0;)						//!��������Ϊ�⵽�ļ��������ʱ
	{count++;
	fread(buf,1,1,fp) ;
	if(i<3)
	{
		if(buf[0]==0)   {i=i+1;}
		else{i=0;}
		
	}
	if(i==3&&buf[0]==1)  return count;						//!����ֵcountΪ��֡�����ֽ���
	}	
return count ;
}
int GetHeadBuf(char* buf,FILE *fp)   //��264ͷ����buf
{
    int k=0;
	k=dingwei(fp,k);
	k=k+count_long(k,fp);
	fseek(fp,0,0);
	fread(buf,1,k-4,fp);
	return k-4;
}
int setbeginTime(char *filename,int beginTime)
{
	FILE *fp;
	char nut=0;char buf[350000];		
	int i=0,k=0,j=0,times,min,ms,framerate=25,num1,tp,count=0,m=0,n=0,p=0;
	char st=0;
	times=beginTime;
	num1=beginTime;		               //!ʱ�任��֡��   +2�ǰ�PPS��SPS������
    fp=fopen(filename,"rb+");
	 for(;num1>0;num1--)	  
    {	k=dingwei(fp,k);              //��λ
	    //char st=slice(fp,k);
	    k=k+count_long(k,fp);
			
	 }
	fclose(fp);
	return k-4;
}

int setbeginPoint(char *filename,int beginTime)
{
	FILE *fp;
	char nut=0;char buf[350000];		
	int i=0,k=0,j=0,times,min,ms,framerate=25,num1,tp,count=0,m=0,n=0,p=0;
	char st=0;
	times=beginTime;
	num1=beginTime;		               //!ʱ�任��֡��   +2�ǰ�PPS��SPS������
    fp=fopen(filename,"rb+");
	 for(;num1>0;num1--)	  
    {	k=dingwei(fp,k);               //��λ�����
	    char st=sliceEx(fp,k);           //��λ֡�߽�
	    if(st==1)
		{
		   k=k+count_long(k,fp)-4;     //��ͷ��4�ֽ�
           
		}	
	 }
	fclose(fp);
	return k-4;
}

