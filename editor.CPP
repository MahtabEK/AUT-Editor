#include <conio.h>
#include <iostream.h>
#include <stdio.h>
int far* scr = (int far*) 0xb8000000;
int endOfProgram = 0;
int cs=0, e, pushed, b[100];
char c;
int x, lang=1;
// ****************** Automata Matrix
int pt[11][10]={
{1,-1,4,-1,6,-1,-1,-1,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
{1,2,4,3,6,7,5,8,9,10},
};
// ******************* Persian Characters
int chars[5][43]={
{48,49,50,51,52,53,54,55,56,57,72,104,102,92,106,101,91,93,112,111,110,98,118,99,67,115,97,119,113,120,122,116,114,39,59,103,108,107,44,105,100,117,121},
{128,129,130,131,132,133,134,135,136,137,141,144,147,149,151,153,155,157,159,161,162,163,164,165,166,168,170,172,174,175,224,234,236,240,238,243,245,247,248,251,254,228,232} ,
{128,129,130,131,132,133,134,135,136,137,141,145,147,149,151,153,155,157,159,161,162,163,164,165,166,168,170,172,174,175,224,234,236,240,238,243,245,247,248,250,254,227,231} ,
{128,129,130,131,132,133,134,135,136,137,141,145,146,148,150,152,154,156,158,160,162,163,164,165,166,167,169,171,173,175,224,233,235,239,237,241,244,246,248,249,252,226,230},
{128,129,130,131,132,133,134,135,136,137,141,144,146,148,150,152,154,156,158,160,162,163,164,165,166,167,169,171,173,175,224,233,235,239,237,241,244,246,248,249,253,225,229},
};

char funkey(int &i){
	char a = getch();
	if(a==0){
		i=1;
		a=getch();
		}
	else
		i=0;
	return a;
}

void init (void){
	 for(int k=0; k<2000; k++)
		*(scr + k) = 0xF0 * 256 + 32;
	if(lang== 1){
		x=35;
		*(scr + 3*80+x) = 0xF1 * 256 + '*';
		*(scr + 6*80 + 15) = 0x70*256 + 'F';
		*(scr + 6*80 + 16) = 0x70*256 + 'a';}
	if(lang== 0){
		x=5;
		*(scr + 3*80+x) = 0xF1 * 256 + '*';
		*(scr + 6*80 + 15) = 0x70*256 + 'E';
		*(scr + 6*80 + 16) = 0x70*256 + 'n';}
}

struct stack{
	char a[100];
	int topOfStack;
	void push(char c);
	char pop();
	int isEmpty();
	int isFull();
	void display();
	void display2();
	void display3();
	void display4();
	void gotoend();
	void gotofirst();
	void save();
	void load();
	int findtop();
	void replacetop(int x);

}left,right;

void stack::save (){

	FILE *ptr;
	ptr = fopen( "editor.txt" ,"wb");
	if( ptr != NULL)
	fwrite(a, sizeof(int) , 100 , ptr);
	fclose(ptr);
}
void stack::load(){
	FILE *ptr;
	ptr = fopen("editor.txt","rb");
	if( ptr != NULL)
	fread(a, sizeof(int) , 2000 , ptr);
	for( int m = 0 ; m<100; m++){
			*( scr + 3*80 + 6+m) = 0x70*256+ a[m];
		}
	x=5;
	fclose(ptr);

}
int stack::isFull(){
	if(topOfStack == 100)
		return 1;
	else
		return 0;
}

int stack::isEmpty(){
	if(topOfStack == 0)
		return 1;
	else
		return 0;
}

void stack::push(char character){
	if(!isFull()){
		a[topOfStack] = character;
		topOfStack++;
	}
}

char stack::pop(){
	if(!isEmpty()){
		topOfStack--;
		return a[topOfStack];
	}
}
void stack::display(){
	if( a[topOfStack - 1] == 1)
		a[topOfStack - 1] =32;
	//if( x<26){
		*(scr + 3*80 + x-1) = 0x70*256 + a[topOfStack-1];
		*(scr + 3*80+x) = 0xF1 * 256 + '*';
	//}
	/*if(x==26){
		x--;
		*(scr + 3*80 + x-1) = 0x70*256 + a[topOfStack-1];
		for(int i=topOfStack - 20 ; i<topOfStack; i++)
			*(scr + 3*80 + 4 + i) = 0x70 *256 + a[i];
		*(scr + 3*80 + x) = 0xF1 *256 + '*';} */

}
void stack::display2(){
	if( a[topOfStack - 1] == 1)
		a[topOfStack - 1] =32;
	*(scr + 3*80+x+1) = 0x70*256 + a[topOfStack-1];
	*(scr + 3*80+x) = 0xF1 * 256 + '*';
}
void stack::display3(){
	for(int i=0; i<topOfStack; i++)
		*(scr + 3*80 + x + topOfStack - i) = 0x70*256+ a[i];
	*(scr + 3*80 + x+ topOfStack+1) = 0x70*256+ 32;
}
void stack::display4(){
	for(int i=1; i<=topOfStack; i++)
		*(scr + 3*80 + x - i) = 0x70*256+ a[topOfStack-i];
	*(scr + 3*80 + x - topOfStack-1) = 0x70*256+ 32;
}

void docommand(int i, char c){
	if( (i==0) &&  ( ((c>='A') && (c<='Z')) || ((c>='a') && (c<='z')) || ((c>='0') && (c<='9')) || c==32 ) )
		e=0;              // A to Z a to z 0 to 9
	else
		e=-1;
	if( i==0 && c ==  8 )     //Backspace
		e=1;
	if( i==1 && c == 'M' )     //Right
		e=2;
	if( i==1 && c == 'K' )    //Left
		e=3;
	if( i==0 && c == 27)     // Escape
		e=4;
	if( i==1 && c =='O')    // End
		e=5;
	if( i==1 && c =='S')    // Delete
		e=6;
	if( i==1 && c =='G') // First(Home Key)
		e=7;
	if(i==1 && c == 'H')
		e=8;
	if( i==1 && c =='P')      // code ascii entero badan mizaram
		e=9;
	cs = pt[cs][e];

}
void changelang(){

		lang = !lang;
		if(lang == 1){
			*(scr + 6*80 + 15) = 0x70*256 + 'F';
			*(scr + 6*80 + 16) = 0x70*256 + 'a'; }
		if(lang == 0){
			*(scr + 6*80 + 15) = 0x70*256 + 'E';
			*(scr + 6*80 + 16) = 0x70*256 + 'n';
			}

}
void home(){
	left.save();
	endOfProgram = 1;
}
void loading(){
	left.load();
}

int stack::findtop(){
return a[topOfStack-1];
}

void stack::replacetop(int x){
       a[topOfStack-1]=x;
}

void detectFaChars(){
	if((c>='0' && c<='9' )|| (c==32)){
		for(int i=0;i<10;i++){
			if (c==chars[0][i])
				pushed=chars[1][i];
		 }
		 if (c==32)
			 pushed=32;

		for(i=0;i<43;i++){
			if ((right.findtop()==chars[1][i])||(right.findtop()==chars[2][i]))
			       right.replacetop(chars[4][i]);
			 }
		for(i=0;i<43;i++){
			if (left.findtop()==chars[2][i])
				pushed=chars[1][i];
			if(left.findtop()==chars[3][i])
				pushed=chars[4][i];}
		 }
	else{
		for(int i=10;i<43;i++){
			if(c==chars[0][i])
				pushed=chars[4][i];
		}

		for(i=10;i<43;i++){
			if(right.findtop()==chars[4][i])
				right.replacetop(chars[1][i]);
			if(right.findtop()==chars[3][i])
				right.replacetop(chars[2][i]);
		}


	}
 }
void characters(){
	if(lang==0){
		x++;
		left.push(c);
		left.display();
		right.display3();
		}
	if(lang == 1){
		detectFaChars();
		right.push(pushed);
		x--;
		right.display2();
		left.display4();

	}
}
void backspace(){   // shart vase kharej nashodane cursor az kadr!!
	if(lang == 0){
		left.pop();
		*(scr + 3*80 + x) = 0x70*256 + 32;
		x--;
		right.display3();
		left.display();
		}
	if(lang == 1){
		right.pop();
		*(scr + 3*80 + x) = 0x70*256 + 32;
		x++;
		right.display2();
		left.display4();
		}
}
void arrowleft(){
		right.push(left.pop());
		x--;
		right.display2();
}

void arrowright(){
		left.push(right.pop());
		x++;
		left.display();
}

void stack::gotoend(){               // end of the line for English
	for(int j=topOfStack-1; j>=0; j--)
		arrowright();

}

void stack::gotofirst(){             // first of the line for English
	for(int j=topOfStack-1; j>=0; j--)
		arrowleft();

}

void del(){
	if(lang==0){
		right.pop();
		right.display3();
	}
	if(lang==1){
		left.pop();
		left.display4();
	}
}
void end(){
	if(lang == 0)
		right.gotoend();
	if(lang == 1)
		left.gotofirst();
}
void first(){
	if(lang == 0)
		left.gotofirst();
	if(lang == 1)
		right.gotoend();
}
main(){
	stack left, right;
	init();
	int i;
	cs=0;
	while( ! endOfProgram){
		c = funkey(i);
		docommand(i,c);
		switch(cs){
			case 1: characters(); break;
			case 2: backspace(); break;
			case 3: arrowleft(); break;
			case 4: arrowright(); break;
			case 5: del(); break;
			case 6: home(); break;
			case 7: end(); break;
			case 8: first(); break;
			case 9: loading(); break;
			case 10: changelang(); break;
		}

	}

}
