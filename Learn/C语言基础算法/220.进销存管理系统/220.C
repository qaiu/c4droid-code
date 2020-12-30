#include    <stdio.h>
#include    <conio.h>
#include    <ctype.h>
#include    <string.h>
//#include    <dos.h>
#include    <math.h>
#include    <time.h>
#include<stdlib.h>
//#include <bios.h>
void show_mouse();
int t();
void edit();
void infor();
void entry();
void display();
void search();
void search_id();
void search_name();
void del();
void deletion();
void deletion_id();
void print();
void help();
void eat();
void general();
void exit();
void seteefect(int, int);
void graph();
void bar();
void graph2();
void bar2();
void report();
void report2();
void report3();
void pur();
void graph1();
void design();
void bbox();
void ventry(char t[],int);

struct shop
{
	char desc[20];
	int rack;
	char ch;
	int cabnit;
	int quantity;
	float sale;
	char id[6];
	float total;
	float unit;
	float cost;
	float profit;
	float bye;
	int qty;
	int ost;
	int b;
    };

struct shop temp;
struct shop x[20];
FILE *ptr;

char a[10];

void infor()
{
      int sher;
      clrscr();
    //  //textcolor(WHITE);
      gotoxy(13,3);
      printf("---->>>  INFORMATION ABOUT PRODUCTS  \--------");
      gotoxy(2,5);
      printf("THIS SECTION IS TO TELL WHAT KIND OF PRODUCTS ARE THERE IN STORE");
      printf("\n\nENTER THE OPTION WHICH SUITS YOU.\n");
      printf("\n\n1.EDIBLES.\n\n");
      printf("\n2.GENERAL ITEMS.\n\n");
      printf("\n\n\n\n\nOPTION:");
      sher=toupper(getch());

  switch(sher)
  {
      case '1':
      eat();
      break;

      case '2':
      general();
      break;

      default:
      gotoxy(8,21);
      printf("\awrong entry press any key to go main menu..");
    }
   getche();

   }

 void eat()
 {
	clrscr();
	printf("*******************************************************\n");
	printf("  ID : 123.  JAM      COMPANY : MITUCHELS  RACK NO 24 \n");
	printf("  ID : 124.  HONEY    COMPANY : PAK PURE   RACK NO 25\n");
	printf("  ID : 125.  MILK     COMPANY : HALEEB     RACK NO 26\n");
	printf("  ID : 126.  BREAD    COMPANY : WONDER     RACK NO 27\n");
	printf("  ID : 127.  CHEES    COMPANY : JUNAID     RACK NO 28\n");
	printf("  ID : 128.  JUICE    COMPANY : SONS       RACK NO 29\n");
	printf("  ID : 129.  PEPSI    COMPANY : MAJEED     RACK NO 30 \n");
	printf("  ID : 130.  BUTTER   COMPANY : JAFFAR     RACK NO 31 \n");
	printf("  ID : 131.  ICECREAM COMPANY : POLKA      FREEZER 32 \n");
	printf("*******************************************************\n\n");
	printf("Press any key to continue......");
	getche();
	clrscr();
	printf("  ID : 133.  MILO     COMPANY : MITUCHELS  RACK NO 24 \n");
	printf("  ID : 134.  KEY BRAN COMPANY : PAK PURE   RACK NO 25\n");
	printf("  ID : 135.  TANG     COMPANY : HALEEB     RACK NO 26\n");
	printf("  ID : 136.  ALWAYS   COMPANY : WONDER     RACK NO 27\n");
	printf("  ID : 327.  PEPPARS  COMPANY : JUNAID     RACK NO 28\n");
	printf("  ID : 138.  JHERBAL  COMPANY : SONS       RACK NO 29\n");
	printf("  ID : 139.  PEPSI    COMPANY : MAJEED     RACK NO 30 \n");
	printf("  ID : 130.  ORAGD    COMPANY : JAFFAR     RACK NO 31 \n");
	printf("  ID : 131.  ICECREAM COMPANY : POLKA      FREEZER 32 \n");
	printf("*******************************************************\n");
	printf("  ID : 143.  SHAMPO   COMPANY : MITUCHELS  RACK NO 24 \n");
	printf("  ID : 144.  AGRI     COMPANY : PAK PURE   RACK NO 25\n");
	printf("  ID : 145.  TIMO     COMPANY : HALEEB     RACK NO 26\n");
	printf("  ID : 146.  WASHSN   COMPANY : WONDER     RACK NO 27\n");
	printf("  ID : 147.  LACE     COMPANY : JUNAID     RACK NO 28\n");
	printf("  ID : 428.  YARDLY   COMPANY : SONS       RACK NO 29\n");
	printf("  ID : 149.  MUSK     COMPANY : MAJEED     RACK NO 30 \n");
	printf("  ID : 140.  BUTTER   COMPANY : JAFFAR     RACK NO 31 \n");
	printf("  ID : 141.  IMPERIAL COMPANY : POLKA      FREEZER 32 \n");
	printf("*******************************************************\n");
	printf("\n\nPress enter to return to main menu");
   }

void general()
{
	clrscr();
	printf("*******************************************************\n");
	printf("  ID : 153.  DETOL    COMPANY : MITUCHELS  RACK NO 24 \n");
	printf("  ID : 154.  SHAMPO   COMPANY : PAK PURE   RACK NO 25\n");
	printf("  ID : 155.  FINERRE  COMPANY : HALEEB     RACK NO 26\n");
	printf("  ID : 156.  CURTIS   COMPANY : WONDER     RACK NO 27\n");
	printf("  ID : 157.  NIVKA    COMPANY : JUNAID     RACK NO 28\n");
	printf("  ID : 158.  NIVCA    COMPANY : SONS       RACK NO 29\n");
	printf("  ID : 159.  DETOL    COMPANY : MAJEED     RACK NO 30 \n");
	printf("  ID : 150.  BUTTER   COMPANY : JAFFAR     RACK NO 31 \n");
	printf("  ID : 151.  SHAVE    COMPANY : POLKA      FREEZER 32 \n");
	printf("*******************************************************\n\n");
	printf("Press any key to continue......");
	getche();
	clrscr();
	printf("  ID : 163.  ADDIS    COMPANY : MITUCHELS  RACK NO 24 \n");
	printf("  ID : 164.  SKIN     COMPANY : PAK PURE   RACK NO 25\n");
	printf("  ID : 165.  MAX      COMPANY : HALEEB     RACK NO 26\n");
	printf("  ID : 166.  BRESCO   COMPANY : WONDER     RACK NO 27\n");
	printf("  ID : 167.  ZABIT    COMPANY : JUNAID     RACK NO 28\n");
	printf("  ID : 168.  COPEX    COMPANY : SONS       RACK NO 29\n");
	printf("  ID : 169.  SOAP     COMPANY : MAJEED     RACK NO 30 \n");
	printf("  ID : 160.  PAMOLI   COMPANY : JAFFAR     RACK NO 31 \n");
	printf("  ID : 161.  AVA      COMPANY : POLKA      FREEZER 32 \n");
	printf("********************************************************\n");
	printf("  ID : 163.  CAMY     COMPANY : MITUCHELS  RACK NO 24 \n");
	printf("  ID : 164.  SAFE     COMPANY : PAK PURE   RACK NO 25\n");
	printf("  ID : 165.  GUARD    COMPANY : HALEEB     RACK NO 26\n");
	printf("  ID : 166.  DOVE     COMPANY : WONDER     RACK NO 27\n");
	printf("  ID : 167.  REXONA   COMPANY : JUNAID     RACK NO 28\n");
	printf("  ID : 168.  TOUCNME  COMPANY : SONS       RACK NO 29\n");
	printf("  ID : 169.  FORHANS  COMPANY : MAJEED     RACK NO 30 \n");
	printf("  ID : 160.  SIGNAL2  COMPANY : JAFFAR     RACK NO 31 \n");
	printf("  ID : 161.  CLOSEUP  COMPANY : POLKA      FREEZER 32 \n");
	printf("********************************************************\n");
	printf("\n\nPress enter to return to main menu");

}

int t(void)
{
   time_t t;

   time(&t);
   gotoxy(15,21);
   printf("Date and time: %s\n", ctime(&t));
   return 0;
}

void ventry(char t[],int code)
{
  int i=0;
  if(code==0)
  {

    while((t[i]=getch())!=10 && i<30)
     if((t[i]>=97 && t[i]<=122) || (t[i]>=65 && t[i]<=90) || t[i]==32 || t[i]==8)
      {	printf("%c",t[i]);
	i++;
	}
      }
  else if(code==1)
  {
    while((t[i]=getch())!=10 && i<10)
     if((t[i]>=48 && t[i]<=57) || t[i]==46 || t[i]==8 )
      {	printf("%c",t[i]);
	i++;
	}
      }
  t[i]='\0';
}


void entry()
{
   char ch;
  clrscr();

	   ptr=fopen("shop.dat","a+b");
  ch='y';
  while(ch=='y')
 {
  clrscr();
  design();
  //textcolor(14);
  t();
  gotoxy(12,3);
  printf("-------------->  PURSHASE  <-------------- ");
   {


     gotoxy(7,5);
    printf(">>>  ENTER PRODUCT ID  : ");
     ventry(temp.id,1);
    // //flushall(); 
     gotoxy(7,7);
     printf(">>>  ENTER DESCRIPTION : ");
     ventry(temp.desc,0);
     //flushall();
    gotoxy(7,9);
     printf(">>>  ENTER RACK NO     : ");
     ventry(a,1);
    temp.rack= atoi(a);

     //flushall();
     gotoxy(7,11);
     printf(">>>  ENTER CABNIT NO   : ");
     ventry(a,1);
    temp.cabnit= atoi(a);

    //flushall();
    gotoxy(7,13);
     printf(">>>  ENTER UNIT COST  :$ ");
     ventry(a,1);
    temp.unit= atof(a);
     //flushall();

     gotoxy(7,15);
     printf(">>>  ENTER SALE PRICE :$ ");
     ventry(a,1);
    temp.sale= atof(a);
     //flushall();

     gotoxy(7,17);
     printf(">>>  ENTER  QUANTITY  :  ");
     ventry(a,1);
    temp.quantity= atoi(a);
     //flushall();

     gotoxy(7,18);
     printf("=====================================");
     temp.total=temp.quantity*temp.sale;

     //textcolor(10);
     gotoxy(10,20);
     printf("TOTAL PRICE = $ %.2f",temp.total);
     temp.cost=(temp.unit*temp.quantity);
     gotoxy(35,20);
     printf("TOTAL COST = $ %.2f",temp.cost);

     }
  fwrite(&temp,sizeof(temp),1,ptr);
  //textcolor(10);
  gotoxy(10,23);
  printf("More entries  [y/n]");
  ch=getche();
 }
fclose(ptr);

}



void display()
{

  clrscr();
  ptr=fopen("shop.dat","rb");
  while ((fread(&temp,sizeof(temp),1,ptr))==1)

     {
      gotoxy(5,3);
      printf("---------------->  RETRIEVE RECORD  <----------------");
      gotoxy(18,5);
      printf(">>>  DESCRIOTION :  %s",temp.desc);
      gotoxy(18,7);
      printf(">>>  ID NUMBER   :  %s",temp.id);
      gotoxy(18,9);
      printf(">>>  RACK NO     :  %d",temp.rack);
      gotoxy(18,11);
      printf(">>>  CABNIT NO   :  %d",temp.cabnit);
      gotoxy(18,13);
      printf("\xDb--\  UNIT COST   :$ %.2f",temp.unit);
      gotoxy(18,15);
      printf(">>>  SALE PRICE  : $ %.2f",temp.sale);
      gotoxy(18,17);
      printf(">>>  QUANTITY    :  %d",temp.quantity);
      gotoxy(5,18);
      printf("======================================================");
      gotoxy(6,20);
      printf(">>>  TOTAL PRICE : $ %.2f",temp.total);
      gotoxy(30,20);
      printf("  TOTAL COST  : $ %.2f",temp.cost);
      gotoxy(10,23);
      printf("Press Enter to see next if any or exit ...");

      getche();
      clrscr();

   }

  fclose(ptr);
}



void search()
{
  int sh;
  clrscr();
	  design();
	  gotoxy(17,6);
	  printf(">>>  Two options available to search \----");
	  gotoxy(15,9);
	  printf("[ 1 ]  -  Search by ID number   .");
	  gotoxy(15,11);
	  printf("[ 2 ]  -  Search by Description .");
	  gotoxy(17,15);
	  printf("Enter your choice: ");
	  sh=toupper(getch());

  switch(sh)
  {
    case '1':
    search_id();
    break;

    case '2':
    search_name();
    break;

    default:
    gotoxy(18,18);
    printf("\a---- wrong entry try again");
    getch();
   }
  getche();

}



void search_id()
{
  int flag=0;
  char lkf[20];
	   clrscr();
	   ptr=fopen("shop.dat","rb");
	   design();
	   gotoxy(13,4);
	   printf(">>>  Enter id to be searched:");
	   scanf("%s",&lkf);
  while((!flag)&&(fread(&temp,sizeof(temp),1,ptr))==1)
  {
      if(strcmp(temp.id,lkf)==0)
      flag=1;
  }
     if(flag)
  {


	 gotoxy(18,6);
	 printf("Product ID   :  %s",temp.id);
	 gotoxy(18,8);
	 printf("Description  :  %s",temp.desc);
	 gotoxy(18,10);
	 printf("Rack no      :  %d",temp.rack);
	 gotoxy(18,12);
	 printf("Cabnit       :  %d",temp.cabnit);
	 gotoxy(18,14);
	 printf("Sale price   :$ %.2f",temp.sale);
	 gotoxy(18,16);
	 printf("Quantity     :  %d",temp.quantity);
	 gotoxy(16,17);
	 printf("============================");
	 gotoxy(18,19);
	 printf(" Total       :$ %.2f",temp.total);


  }
  else
       printf("Record not found!");
 fclose(ptr);
}


void search_name()
{
  int flag=0;
  char lkf[20];
	   clrscr();
	   ptr=fopen("shop.dat","rb");
	   design();
	   gotoxy(13,4);
	   printf(">>>  Enter description to be searched:");
	   scanf("%s",&lkf);
  while((!flag)&&(fread(&temp,sizeof(temp),1,ptr))==1)
  {
      if(strcmp(temp.desc,lkf)==0)
      flag=1;
  }
     if(flag)
  {


	 gotoxy(18,6);
	 printf("Product ID   :  %s",temp.id);
	 gotoxy(18,8);
	 printf("Description  :  %s",temp.desc);
	 gotoxy(18,10);
	 printf("Rack no      :  %d",temp.rack);
	 gotoxy(18,12);
	 printf("Cabnit       :  %d",temp.cabnit);
	 gotoxy(18,14);
	 printf("Sale price   :$ %.2f",temp.sale);
	 gotoxy(18,16);
	 printf("Quantity     :  %d",temp.quantity);
	 gotoxy(16,17);
	 printf("============================");
	 gotoxy(18,19);
	 printf(" Total       :$ %.2f",temp.total);


  }
  else
       printf("Record not found!");
 fclose(ptr);
}


void deletion()
{
 int j,n,i;
 char tar[30];
 int count=0;

		ptr = fopen("shop.dat","rb");
		clrscr();
		design();
		gotoxy(17,10);
		printf("----  Enter Description to be deleted:");
		scanf("%s",&tar);
		j=0;

		while((fread(&temp,sizeof(temp),1,ptr))==1)
		{
		   if((strcmp(temp.desc,tar)<0) || (strcmp(temp.desc,tar)>0))
		   {
		      x[j] = temp;
		      j++;
		    }
		  else
		  {
		    count++;
		  }
		}
		if (count==0)
		{
		 clrscr();
		 design();
		 gotoxy(30,10);
		 printf("- Record Not Found");
		 gotoxy(17,23);
		 printf("Press Enter to go to MAIN MENU .....");
		 getch();
		 return;
		}

		fclose(ptr);
		n = j;
		clrscr();
		ptr=fopen("shop.dat","wb");
		for(i=0; i<n; i++)
		fwrite(&x[i],sizeof(x[i]),1,ptr);
		fclose(ptr);
		clrscr();
		design();
		gotoxy(20,10);
		printf("Record has been deleted");
		gotoxy(20,12);
		printf("Check it by going to the displayed record");
		gotoxy(17,23);
		printf("Press Enter to go to MAIN MENU ......");
		getch();


}
  void print()
  {


   clrscr();
  putc('\n',stdout);
  fputs("\n\t\t\tREPRT OF SALES",stdout);
  putc('\n',stdout);
  fputs("===========================",stdout);
  putc('\n',stdout);
  putc('\n',stdout);
  fputs("PRODUCT ID.    NAME.     SALE PRICE.     QUANTITY.    TOTAL  PRICE",stdout);
  fputs("===================================================================",stdout);
  putc('\n',stdout);
  putc('\n',stdout);

  ptr=fopen("shop.dat","rb");
  while( (fread(&temp,sizeof(temp),1,stdout))==1)

  {
     fprintf(stdout," %s",temp.id);
     fprintf(stdout,"\t%s",temp.desc);
     fprintf(stdout,"\t%.2f",temp.sale);
     fprintf(stdout,"\t%ld\n",temp.quantity);
     fprintf(stdout,"\t%.2f",temp.total);
  }
  fclose(ptr);
 }


void help()
{
  clrscr();
     design();
     //textcolor(WHITE);
     gotoxy(20,4);
     printf("---->>>  HELP ABOUT THE PROGRAM  \--------");
     gotoxy(4,6);
     printf(" [ 2 ] - ENTER  RECORDS : THIS OPTION ALLOWS THE USER TO ENTER NEW ");
     gotoxy(30,7);
     printf("RECORDS. USER ENTERS PRODUCT ID, DESCRIPTION,");
     gotoxy(30,8);
     printf("QUANTITY AND UNIT PRICE. THE PROGRAMM CALCULATES");
     gotoxy(30,9);
     printf("TOTAL PRICE AND DISPLAYS ON THE SCREEN.");
     gotoxy(4,11);
     printf(" [ 3 ] - ENTER SALES    :  THIS OPTION ALLOWS THE USER TO SEE PREVIOUS ");
     gotoxy(30,12);
     printf("SALE RECORDS.");
     gotoxy(4,14);
     printf(" [ 4 ] - SEARCH PURCHASE : THIS OPTION ALLOWS THE USER TO SEARCH SPECIFIC");
     gotoxy(30,15);
     printf("RECORD BY ENTERING ID OR 'DESCRIPTION'");
     gotoxy(4,17);
     printf(" [ 5 ] - DELETE PURCHASE : THIS OPTION ALLOWS THE USER TO DELETE A SPECIFIC");
     gotoxy(30,18);
     printf("RECORD BY ENTERING ITS 'DESCRIPTION' OR ID");
     gotoxy(6,23);
     printf("Press any key to continue .............");

     getche();
     clrscr();
      design();
      //textcolor(WHITE);
      gotoxy(4,5);
     printf(" [ 6 ] - VIEW REPORT  :   THIS OPTION ALLOWS THE USER TO PRINT PREVIOUS");
     gotoxy(30,6);
     printf("SALE RECORDS ON PRINTER");
     gotoxy(4,8);
     printf(" [ 7 ] - PRINT RECORDS :  THIS OPTION ALLOWS THE USER TO PRINT RECORDS");
     gotoxy(30,9);
     printf("OM PRINTER.");
     gotoxy(4,11);
     printf(" [ 8 ] - VIEW GRAPH     :  THIS OPTION ALLOWS THE USER TO VIEW GRAPH ");
     gotoxy(30,12);
     printf("OF QUANTITY.");
     gotoxy(10,23);
     printf("Press any key to go to the MAIN MENU ........");

 getche();
}

int menu()
{
 int x;
  do{
       {
	clrscr();
	design();
	t();
	//textcolor(WHITE);
	gotoxy(24,3);
//	printf("\xBD-->>>  SAM'S DEPARTMANTAL STORE  \--------");
	gotoxy(3,4);
	printf("---------------------------------------------------------");
	gotoxy(20,5);
	printf("MAIN MENU");
	gotoxy(13,8);
	printf(" 1  -   INFORMATION ABOUT PRODUCTS            ");
	gotoxy(13,9);
	printf(" 2  -   ENTER  PURCHASE  RECORDS            ");
	gotoxy(13,10);
	printf(" 3  -   ENTER PRODUCTS TO BE SALE           ");
	gotoxy(13,11);
	printf(" 4  -   SEARCH FOR RECORD                     ");
	gotoxy(13,12);
	printf(" 5  -   DELETE RECORD FROM STORE DATABASE     ");
	gotoxy(13,13);
	printf(" 6  -   VIEW SALES , PURCHASE & PROFIT REPORT ");
	gotoxy(13,14);
	printf(" 7  -   PRINT RECORDS                         ");
	gotoxy(13,15);
	printf(" 8  -   BAR  GRAPH OF QUANTITY / PROFIT       ");
	gotoxy(13,16);
	printf(" 9  -   RETRIEVE INFORMATION         ");
	gotoxy(13,17);
	printf(" H  -   HELP                                  ");
	gotoxy(13,18);
	printf(" E  -   EXIT                                  ");
	gotoxy(13,23);
	printf("ENTER YOUR CHOICE : ");
	gotoxy(47,23);
	x=toupper(getch());
	switch(x)
	{
	case '1':
	infor();
	break;

	case '2':
	entry();
	break;

	case '3':
	edit();
	break;

	case '4':
	search();
	break;

	case '5':
	del();
	break;

	case '6':
	report2();
	break;

	case '7':
	print();
	break;

	case 'h': case'H':
	help();
	break;

	case'8':
	graph1();
	break;

	case '9':
	display();
	break;

	case 'e': case 'E':
	exit(0);
	break;

	default:
	clrscr();
	design();
	gotoxy(10,12);
	printf("\a--\  WRONG ENTRY : PRESS ANY KEY AND TRY AGAIN");
	getche();
	}
      }
    }while((x!='e')||(x!='E'));
return x;
}


int main()
{
//textbackground(14);
//show_mouse();
menu();
}

void graph()
{
  int inscor=1;
  char ch;
  clrscr();
  design();
  gotoxy(15,23);
  printf("Press enter to go to MAIN MENU .....");
  //textcolor(10);
  gotoxy(9,4);
  printf("********* GRAPH OF PRODUCT QUANTITY ********");
  //textcolor(14);
  gotoxy(3,5);
  printf(" PRODUCT NAME");
  ptr=fopen("shop.dat","rb");
  while((fread(&temp,sizeof(temp),1,ptr))==1)
     {

     printf("\n | %s  \t:",temp.desc);
     bar();

   }

  fclose(ptr);
getche();
}
void bar()
{
int j;

printf(" %d",temp.quantity);
printf("\n");
}
void report()
{
  char ch;
  clrscr();
  design();
  gotoxy(20,23);
  printf("Press Enter to go to MAIN MENU ........");
  //textcolor(GREEN);
  gotoxy(25,4);
  printf("------>>> VIEW OF SALES  ----------");
  //textcolor(RED);
  gotoxy(4,7);
  printf("ID.  NAME.  SALE PRICE.  QUANTITY.    TOTAL  PRICE");
  gotoxy(3,9);
  printf("=================================================");

  ptr=fopen("shop.dat","rb");
  while((fread(&temp,sizeof(temp),1,ptr))==1)

     {
      printf("\n\t%4s    ",temp.id);
      printf("%4s    ",temp.desc);
      printf("$%.2f    ",temp.sale);
      printf("%8d   ",temp.quantity);
      printf("$%.2f    ",temp.total);

       }

getche();
}
void seteffect(int b, int f)
{
   clrscr();
  // textbackground(b);
   //textcolor(f);
}


void pur()
{
 char ch;
  clrscr();
  design();
//  show_mouse();
  gotoxy(20,23);
  printf("Press Enter to go to MAIN MENU ...........");
  //textcolor(GREEN);
  gotoxy(25,4);
  printf("------>>> VIEW OF PURCHASES ----------");
  //textcolor(RED);
  gotoxy(4,7);
  printf("PRODUCT ID. NAME. UNIT PRICE.  QUANTITY.       TOTAL  COST");
  gotoxy(4,9);
  printf("=============================================");

  ptr=fopen("shop.dat","rb");
  while((fread(&temp,sizeof(temp),1,ptr))==1)
     {
      printf("\n | %s\t",temp.id);
      printf(" %s",temp.desc);

      printf("\t$%.2f",temp.unit);
      printf("\t%d",temp.quantity);
      printf("\t $%.2f",temp.cost);
       }
getche();
}


void graph2()
{
  int inscor2=1;
  char ch;
  clrscr();
  design();
  gotoxy(20,23);
  printf("Pres Enter to go to MAIN MENU ...........");
  //textcolor(10);
  gotoxy(15,4);
  printf("************* GRAPH OF PROFIT *************");
  //textcolor(14);
  gotoxy(4,5);
  printf("PRODUCT NAME");
  ptr=fopen("shop.dat","rb");
  while((fread(&temp,sizeof(temp),1,ptr))==1)
     {

     printf("\n | %s  \t:",temp.desc);
     bar2();

   }

  fclose(ptr);
getche();
}
void bar2()
{
int j;
temp.profit=(temp.total-temp.cost);
for(j=1;j<=temp.profit;j++)
printf("--");
printf(" %.1f$",temp.profit);
printf("\n");
}



void graph1()
{
      int sher;
      clrscr();
      //textcolor(WHITE);
      gotoxy(9,3);
      printf("--------------->  BAR GRAPH <-----------------");
      gotoxy(3,5);
      printf("THIS SECTION IS TO show the graph of quantity and profit ");
      printf("\n\n\t*******  ENTER THE OPTION WHICH SUITS YOU. *******\n");
      printf("\n\n\t>>>  1.QUANTITY.\n\n");
      printf("\n\t>>>  2.PROFIT.\n");
      printf("\n\n\n\t>>>  OPTION:");
      sher=toupper(getch());

  switch(sher)
  {
      case '1':
      graph();
      break;

      case '2':
      graph2();
      break;

      default:
      gotoxy(9,20);
      //textcolor(RED);
      printf("\a--\ WRONG ENTRY : PRESS ENTER TO GO TO MAIN MENU... ");
      getche();
      }

 }

void report3()
{
 char ch;
  clrscr();
  design();
  gotoxy(20,23);
  printf("Press Enter to go to MAIN MENU .... ");
  //textcolor(GREEN);
  gotoxy(25,4);
  printf("------>>> VIEW OF PROFIT  \----------");
  //textcolor(RED);
  gotoxy(4,7);
  printf("PRODUCT ID.    DESCRIPTION.    QUANTITY.         PROFIT.");
  gotoxy(4,9);
  printf("================================================");

  ptr=fopen("shop.dat","rb");
  while((fread(&temp,sizeof(temp),1,ptr))==1)
     {

      printf("\n |   %s.",temp.id);
      printf("\t\t%s",temp.desc);
      printf("\t\t%d",temp.quantity);
      temp.profit=(temp.total-temp.cost);
      printf("\t\t $%.2f",temp.profit);

       }
getche();
}


void report2()
{

int sher;
      clrscr();
      bbox();
      //textcolor(WHITE);
      gotoxy(10,3);
      printf("------->  REPORT OF SALES / PURCHASE  \<--------");
      gotoxy(15,5);
      printf("THIS SECTION IS VIEWING SALES, ");
      gotoxy(15,6);
           printf(  "PURCHASE and PROFIT REPORTS ");
      gotoxy(10,8);
      printf("******  ENTER THE OPTION WHICH SUITS YOU. *****\n");
      gotoxy(12,10);
      printf(">>>  1.SALES.");
      gotoxy(12,12);
      printf(">>>  2.PURCHASE.");
      gotoxy(12,14);
      printf(">>>  3.PROFIT.");
      gotoxy(12,17);
      printf(">>>  OPTION:");
      sher=toupper(getch());


  switch(sher)
  {
      case '1':
      report();
      break;

      case '2':
      pur();
      break;

      case '3':
      report3();
      break;

      default:
	gotoxy(2,20);
	//textcolor(RED);
	printf("-- WRONG ENTRY : PRESS ENTER TO GO TO MAIN MENU..--");
	getche();

    }

 }



void design()
{
int i;
clrscr();
	//textcolor(14);
	gotoxy(2,2);
	printf("+");
	gotoxy(3,2);
       for(i=1;i<59;i++)
       printf("-");
	gotoxy(60,2);
	printf("+");
	gotoxy(2,3);
	printf("|");gotoxy(2,4);printf("|");gotoxy(2,5);printf("|");
	gotoxy(2,6);printf("|");gotoxy(2,7);printf("|");gotoxy(2,8);printf("|");
	gotoxy(2,9);printf("|");gotoxy(2,10);printf("|");gotoxy(2,11);printf("|");gotoxy(2,12);printf("|");
	gotoxy(2,13);printf("|");gotoxy(2,14);printf("|");gotoxy(2,15);printf("|");gotoxy(2,16);printf("|");
	gotoxy(2,17);printf("|");gotoxy(2,18);printf("|");gotoxy(2,22);printf("+");

	gotoxy(2,19);printf("|");gotoxy(2,20);printf("|");gotoxy(2,21);printf("|");gotoxy(2,24);printf("+");
	gotoxy(2,23);printf("|");
	gotoxy(3,24);
	for(i=1;i<59;i++)
	printf("-");
	gotoxy(77,18);printf("|");gotoxy(77,19);printf("|");gotoxy(77,20);printf("|");gotoxy(77,21);printf("|");gotoxy(77,24);printf("+");
	gotoxy(77,23);
	printf("|");
	gotoxy(3,22);
	for(i=1;i<59;i++)
	printf("-");
	gotoxy(77,22);
	printf("+");
	gotoxy(77,3);
	printf("|");gotoxy(77,4);printf("|");gotoxy(77,5);printf("|");
	gotoxy(77,6);printf("|");gotoxy(77,7);printf("|");gotoxy(77,8);printf("|");
	gotoxy(77,9);printf("|");gotoxy(77,10);printf("|");gotoxy(77,11);printf("|");gotoxy(77,12);printf("|");
	gotoxy(77,13);printf("|");gotoxy(77,14);printf("|");gotoxy(77,15);printf("|");gotoxy(77,16);printf("|");
	gotoxy(77,17);printf("|");
       //textcolor(RED);

}

void bbox()
{
	int c,r;
	for(c=1;c<=79;c++)
	{
		gotoxy(c,1);
		printf("#");
		gotoxy(c,25);
		printf("*");
	}
	for(r=1;r<=24;r++)
	{
		gotoxy(1,r);
		printf("|");
		gotoxy(79,r);
		printf("|");
	}
}



void deletion_id()
{
 int j,n,i;
 char tar[30];
 int count=0;

		ptr = fopen("shop.dat","rb");
		clrscr();
		design();
		gotoxy(15,8);
		printf("---- Enter ID be deleted:");
		scanf("%s",&tar);
		j=0;

		while((fread(&temp,sizeof(temp),1,ptr))==1)
		{
		   if((strcmp(temp.id,tar)<0) || (strcmp(temp.id,tar)>0))
		   {
		      x[j] = temp;
		      j++;
		    }
		  else
		  {
		    count++;
		  }
		}
		if (count==0)
		{
		 clrscr();
		 design();
		 gotoxy(30,10);
		 printf("---- Record Not Found");
		 gotoxy(17,23);
		 printf("Press Enter to go to MAIN MENU ......");
		 getch();
		 return;
		}

		fclose(ptr);
		n = j;
		clrscr();
		ptr=fopen("shop.dat","wb");
		for(i=0; i<n; i++)
		fwrite(&x[i],sizeof(x[i]),1,ptr);
		fclose(ptr);
		clrscr();
		design();
		gotoxy(15,10);
		printf("Record has been deleted");
		gotoxy(15,12);
		printf("Check it by going to the displayed record");
		gotoxy(17,23);
		printf("Press Enter to go to MAIN MENU ......");
		getch();

}


void del()
{
int sh;
  clrscr();
	  design();
	  gotoxy(17,6);
	  printf(">>> Two options available to delete \----");
	  gotoxy(16,9);
	  printf("[ 1 ] -  Delete by ID number   ");
	  gotoxy(16,11);
	  printf("[ 2 ] -  Delete by Description ");
	  gotoxy(18,15);
	  printf("Enter your choice : ");
	  sh=toupper(getch());

  switch(sh)
  {
    case '1':
    deletion_id();
    break;

    case '2':
    deletion();
    break;

    default:
    gotoxy(18,18);
    printf("\a----  Wrong entry ");
    gotoxy(17,23);
    printf("Press Enter to go to MAIN MENU .....");
    getch();
   }
  getche();

}


void edit()
{
 int j,n,i,a;
 float b;
 char tar[30],ch;
 int count=0;
		ch='y';
		while(ch=='y')
		{
		ptr = fopen("shop.dat","rb");
		clrscr();
		design();
		gotoxy(15,9);
		printf("---- ENTER ID OF THE PRODUCT TO BE SOLD:");
		scanf("%s",&tar);
		j=0;

		while((fread(&temp,sizeof(temp),1,ptr))==1)
		{
		   if((strcmp(temp.id,tar)<0) || (strcmp(temp.id,tar)>0))
		   {
		      x[j] = temp;
		      j++;
		    }
		  else
		  {
		  design();
		  gotoxy(8,7);
		  printf("----  Description       = %s",temp.desc);
		  gotoxy(8,9);
		  printf("----  Quantity in stock = %d",temp.quantity);
		  gotoxy(8,11);
		  printf("----  Sales price       = %.2f",temp.sale);
		  gotoxy(8,13);
		  printf("----  Quantity want to sale : ");
		  scanf("%d",&a);

		  x[j]=temp;
		  x[j].quantity=(x[j].quantity-a);
		  x[j].total=(x[j].quantity*temp.sale);
		  x[j].cost=(x[j].quantity*temp.unit);
		  x[j].bye=(x[j].sale*a);
		  b=x[j].bye;
		  x[j].qty=a;
		  j++;
		  count++;
		  }
		}
		if (count==0)
		{
		 clrscr();
		 design();
		 gotoxy(33,10);
		 printf("\a>>>  Not in stock");
		 getch();
		 return;
		}

		fclose(ptr);
		n = j;
		clrscr();
		ptr=fopen("shop.dat","wb");
		for(i=0; i<n; i++)
		fwrite(&x[i],sizeof(x[i]),1,ptr);
		fclose(ptr);
		clrscr();
		design();
		gotoxy(8,9);
		printf("----  Price paid by customer = %.2f",b);
		gotoxy(8,11);
		printf("----  Quantity sold          = %d",a);
		getch();
		printf("more enteries=(y/n)");
		ch=getche();
		}
}
/*
void show_mouse(void)
{
	union REGS in, out;

	in.x.ax = 0x1;
	int86(0x33, &in, &out);
}
*/