#include <thread>
#include <unistd.h>
#include <iostream>
#include <mylib.h> 
#include <time.h>
using namespace std;
 int stop=0;  // bien dung ca 2 thread
void run(){
	char kt;
	while((kt=getch())!=27 && stop==0){  // an ESC de ket thuc
		gotoxy(0,1);   cout <<kt;
	}
	stop=1;
	if (kt==27) cout<<"\nDung chuong trinh do user ."<<endl;
	else cout<<"\nDung chuong trinh do het thoi gian ."<<endl;
}
void startTimerJob(int time_in_second){
	while(time_in_second>=0 && stop==0){
		gotoxy(70,1); cout <<"Thoi gian con lai : " << time_in_second<<"   ";
        Sleep(1000);		
		time_in_second-=1; 
	}
	if (stop==0) cout<<"\nHet thoi gian"<<endl;  
	stop=1; 

}
void startTimer(){
	thread timer;
	timer=thread(startTimerJob,10); // tao thread cho Ch Trinh con startTimerJob chay trong 10 giay
	
	run();  // luong chuong trinh chinh cua ta
	timer.join();  // After a call to this function, the thread object becomes non-joinable and can be destroyed safely
}
int main(){
	startTimer();
		
}

