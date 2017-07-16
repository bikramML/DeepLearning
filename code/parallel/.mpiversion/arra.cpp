#include<iostream>
using namespace std;
main()
{
	float **s;
	s=new float*[5];
	for(int i=0;i<5;i++)
	{
		s[i]=new float[6500];
	}
}
