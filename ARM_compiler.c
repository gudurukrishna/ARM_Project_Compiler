#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define register_reference 10000
//---------------------------------------STACK-------------------------------------
struct stack{
int data;
struct stack* next;
};
//--------------------------------NODE STRUCTURE---------------------------
struct node{
int number;
int is_operand;
struct node *next;
};
//------------------------PUSHES VALUE INTO THE STACK---------------------------------------
void push(int data,struct stack **st1)
{
struct stack *temp=(struct stack *) malloc(sizeof(struct stack));
temp->data=data;
temp->next=*st1;
*st1=temp;
}
//---------------------------------------POPS VALUE FROM THE STACK-------------------------------------
int pop(struct stack **st1)
{
if(*st1==NULL)
	return -1;
struct stack *temp=*st1;
*st1=(*st1)->next;
int data=temp->data;
free(temp);
return data;
}
//-------------------------CREATES LINKED LIST TO EVALUATE THE POSTFIX FOR THE input_expression----------------------------------------
void append(int data,int is_operand,struct node **start,struct node **end)
{
struct node *temp=(struct node *) malloc(sizeof(struct node));
temp->number=data;
temp->is_operand=is_operand;
temp->next=NULL;
if((*end)==NULL)
{
(*end)=temp;
(*start)=temp;
}
else
{
(*end)->next=temp;
(*end)=(*end)->next;
}
}

//-------------------------------------------RETURNS THE top OF STACK--------------------------------------
char top_of_stack(struct stack **st1)
{
return (*st1)->data;
}

//---------------------------------------------CHECKS IF STACK IS EMPTY--------------------------------------------
int is_stack_empty(struct stack **st1)
{
	if((*st1)==NULL)
	return 1;
	else
	return 0;
}

//----------------------------------------------------RETURNS THE PRECEDENCE OF OPERATORS-------------------------------------
int precedence_table(char ch)
{
if(ch=='(')
{
return 1;}
if(ch=='+'||ch=='-')
return 2;
if(ch=='*'||ch=='/')
return 3;
}

//-------------------------------------------------CREATES ASSEMBLY CODE FOR GIVEN ARITHMETIC input_expression---------------------------------
void assembly_code(struct node **start)
{
struct node *temp=*start;
struct stack **operand_stack=(struct stack **) malloc(sizeof(struct stack *));

char c;
int operand1,operand2,reg=0;
FILE *fpointer=fopen("assembly_code.s","w");
if(fpointer==NULL)
{
printf("Error in opening the file\n");
exit(1);
}
fprintf(fpointer,"\tAREA appcode, CODE, READONLY\n\tEXPORT __main\n\tENTRY\n__main FUNCTION\n");
while((temp)!=NULL)
{
if(temp->is_operand==0)
push(temp->number,operand_stack);
else
{
c=temp->number;
switch(c)
{ 
	case '+':
	
// less than register_reference -> number, if it is >register_reference it is a register. reg variable is used to know the current 	   	registers under use. This is mainly used to preserve the number of registers. 
		if((operand1=pop(operand_stack))<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand1);
		if((operand2=pop(operand_stack))<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand2);
		//printf("operand1=%d,operand2=%d\n",operand1,operand2);
		if(operand1<register_reference && operand2<register_reference)
		{
		fprintf(fpointer,"\t\tADD R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(register_reference+(reg-1),operand_stack);
		}
		if(operand1<register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tADD R%d,R%d\n",operand2-register_reference,reg-1);
		reg--;
		push(operand2,operand_stack);
		}
                if(operand1>=register_reference && operand2<register_reference)
                {
                fprintf(fpointer,"\t\tADD R%d,R%d\n",operand1-register_reference,reg-1);
                reg--;
                push(operand1,operand_stack);
                }
		if(operand1>=register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tADD R%d,R%d\n",operand2-register_reference,operand1-register_reference);
		reg--;
		push(operand2,operand_stack);
		}
		break;
		
	case '-':
		operand1=pop(operand_stack);
		operand2=pop(operand_stack);
		if(operand2<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand2);
		if(operand1<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand1);
		if(operand1<register_reference && operand2<register_reference)
		{
		fprintf(fpointer,"\t\tSUB R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(register_reference+(reg-1),operand_stack);
		}
		if(operand1<register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tSUB R%d,R%d\n",reg-1,operand2-register_reference);
		reg--;
		push(operand2,operand_stack);
		}
                if(operand1>=register_reference && operand2<register_reference)
                {
                fprintf(fpointer,"\t\tSUB R%d,R%d\n",reg-1,operand1-register_reference);
                reg--;
                push(operand1,operand_stack);
                }
		if(operand1>=register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tSUB R%d,R%d\n",operand2-register_reference,operand1-register_reference);
		reg--;
		push(operand2,operand_stack);
		}
		break;
		
	case '*':
		if((operand1=pop(operand_stack))<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand1);
		if((operand2=pop(operand_stack))<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand2);
		if(operand1<register_reference && operand2<register_reference)
		{
		fprintf(fpointer,"\t\tMUL R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(register_reference+(reg-1),operand_stack);
		}
		if(operand1<register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tMUL R%d,R%d\n",operand2-register_reference,reg-1);
		reg--;
		push(operand2,operand_stack);
		}
                if(operand1>=register_reference && operand2<register_reference)
                {
                fprintf(fpointer,"\t\tMUL R%d,R%d\n",operand1-register_reference,reg-1);
                reg--;
                push(operand1,operand_stack);
                }
		if(operand1>=register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tMUL R%d,R%d\n",operand2-register_reference,operand1-register_reference);
		reg--;
		push(operand2,operand_stack);
		}
		break;
		
	case '/':
		operand1=pop(operand_stack);
		operand2=pop(operand_stack);
		if(operand2<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand2);
		if(operand1<register_reference)
		fprintf(fpointer,"\t\tMOV R%d,#%d\n",reg++,operand1);
		if(operand1<register_reference && operand2<register_reference)
		{
		fprintf(fpointer,"\t\tSDIV R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(register_reference+(reg-1),operand_stack);
		}
		if(operand1<register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tSDIV R%d,R%d\n",reg-1,operand2-register_reference);
		reg--;
		push(operand2,operand_stack);
		}
                if(operand1>=register_reference && operand2<register_reference)
                {
                fprintf(fpointer,"\t\tSDIV R%d,R%d\n",reg-1,operand1-register_reference);
                reg--;
                push(operand1,operand_stack);
                }
		if(operand1>=register_reference && operand2>=register_reference)
		{
		fprintf(fpointer,"\t\tSDIV R%d,R%d\n",operand2-register_reference,operand1-register_reference);
		reg--;
		push(operand2,operand_stack);
		}
		break;
}
}
temp=temp->next;
}
fprintf(fpointer,"stop B stop\n\tENDFUNC\n\tEND\n");
fclose(fpointer);
}


int main()
{

struct node **list_end=(struct node **)malloc(sizeof(struct node*));
struct node **list_start=(struct node **)malloc(sizeof(struct node*));
struct stack **operator_stack=(struct stack **)malloc(sizeof(struct stack*));
printf("Enter the expression:\n");
char input_exp[100],c;
int i,number=0,j;
scanf("%s",input_exp);

for(i=0;i<strlen(input_exp);i++)
{
	if((input_exp[i]>='A'&& input_exp[i]<='Z')||(input_exp[i]>='a'&&input_exp[i]<='z'))
	{
		printf("%c",input_exp[i]);
		append(input_exp[i],0,list_start,list_end);
	}

	if((input_exp[i]>='0'&&input_exp[i]<='9'))
	{
		j=i;
		while((input_exp[j]>='0'&&input_exp[j]<='9'))
		{
			number=(number*10)+(input_exp[j]-48);
			j++;
		}
		i=j-1;
		printf("%d",number);
		append(number,0,list_start,list_end);		
		number=0;
		//continue;
	}
	if(input_exp[i]=='(')
		push('(',operator_stack);
	if(input_exp[i]==')')
	{
		while((c=pop(operator_stack))!='(')
			{
			printf("%c",c);
			append(c,1,list_start,list_end);
			}
	}
	if((input_exp[i]=='+')||(input_exp[i]=='-')||(input_exp[i]=='*')||(input_exp[i]=='/'))
	{
		if(!is_stack_empty(operator_stack))
		while(precedence_table(top_of_stack(operator_stack))>=precedence_table(input_exp[i]))
		{
		c=pop(operator_stack);
		printf("%c",c);
		append(c,1,list_start,list_end);
		if(is_stack_empty(operator_stack))
		break;
		}
		push(input_exp[i],operator_stack);
	}
   
}
while((c=pop(operator_stack))!=-1)
{
printf("%c",c);
append(c,1,list_start,list_end);
}
printf("\n");
assembly_code(list_start);
return 0;
}
