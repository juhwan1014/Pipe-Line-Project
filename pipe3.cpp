
#include<stdio.h>


int ret;
	int i = 0;
	int inst;
	int SEI; int ZEI; int BA; int JA;
int op ;
int rs;
int rt;
int rd;
int imm;
int shamt;
int func;
int address;
int memory[0x2000]={0,};
int Reg[32]={0,};

int cycle=0;
int pc=0;




enum funct{ sll, srl = 0x02, jr = 0x08, add = 0x20, addu, sub, subu, and, or, nor = 0x27, slt = 0x2a, sltu };
enum opcode_I{ beq = 0x4, bne, addi = 0x8, addiu, slti = 0xa, sltiu, andi, ori, lui = 0xf, lw = 0x23, lbu,
	           lhu, sb = 0x28, sh, ll, sw = 0x2b, sc = 0x38 };
enum opcode_J{ j = 0x2, jal };

struct if_latch{
	unsigned int valid;
	unsigned int pc4;
	unsigned int inst;
};
struct id_latch{
	unsigned int valid;
	unsigned int pc4;
	unsigned int op;
	unsigned int rs;
	unsigned int rd;
	unsigned int rt;
             int SEI;
			 int ZEI;
    unsigned int shamt;
    unsigned int func;
	unsigned int BA;
	unsigned int JA;
	unsigned int imm;
	unsigned int address;
	unsigned int wReg;
	unsigned int Npc;
};
struct ex_latch{
	unsigned int valid;
	unsigned int op_ex;
	unsigned int rs_ex;
	unsigned int rt_ex;
	unsigned int rd_ex;
	unsigned int v1;
	unsigned int v2;
	unsigned int wReg;
	unsigned int BA;
	unsigned int R;
	unsigned int W;
	unsigned int m3;
	unsigned int Npc;
	unsigned int rup;
};
struct mem_latch{
	unsigned int valid;
	unsigned int op_mem;
		unsigned int rs_mem;
	unsigned int rt_mem;
	unsigned int rd_mem;
	unsigned int v3;
	unsigned int v4;
	unsigned int wreg;
	unsigned int m3;
	unsigned int rup;
};
struct wb_latch{
     unsigned int valid;
	 unsigned int op_wb;
	 unsigned int rd_wb;
	unsigned int rt_wb;
		unsigned int rs_wb;
	unsigned int wreg;
	unsigned int m3;
	unsigned int rup;
};

struct if_latch if_latch[2];

struct id_latch id_latch[2];

struct ex_latch ex_latch[2];

struct mem_latch mem_latch[2];

struct wb_latch wb_latch[2];












	
	int write_back(struct mem_latch[2], struct wb_latch[2])
	{

		if(wb_latch[1].valid == 0)
		{	
			if(pc==-1)
        return 5;
			else
				return 3;
		}

wb_latch[0].valid=1;
	wb_latch[0].op_wb=mem_latch[1].op_mem;
wb_latch[0].rs_wb=mem_latch[1].rs_mem;
wb_latch[0].rt_wb=mem_latch[1].rt_mem;
wb_latch[0].rd_wb=mem_latch[1].rd_mem;

		
if(mem_latch[1].op_mem==0x0)
		             { 
						 Reg[wb_latch[0].rd_wb]=mem_latch[1].v3;
printf("WRITE BACK ----> Reg[rd]값 업데이트\n");
}
else if (mem_latch[1].op_mem!=0x0)
	{Reg[wb_latch[0].rt_wb]=mem_latch[1].v3;
	printf("WRITE BACK ----> Reg[rt]값 업데이트\n");
}
/*
		int temp;
		switch (op){
		case 0:Reg[rd] = temp;
			break;
		case j:
			break;
		case jal:
			break;
		default:if(rt)Reg[rt] = temp;
		}
		*/

wb_latch[1]=wb_latch[0];
		return 1;
	}























	int memory_access(struct ex_latch[2], struct mem_latch[2])
	{	
		
		if(mem_latch[1].valid == 0)
			{wb_latch[1].valid=0;
        return 3;
		}
		wb_latch[1].valid=1;
		mem_latch[0].op_mem=ex_latch[1].op_ex;
mem_latch[0].rs_mem=ex_latch[1].rs_ex;
mem_latch[0].rt_mem=ex_latch[1].rt_ex;
mem_latch[0].rd_mem=ex_latch[1].rd_ex;



		if(ex_latch[1].op_ex==lbu)
{Reg[rt]=memory[(Reg[rs]+SEI)/4]&0xFF;
   
   goto p;
}
		else if(ex_latch[1].op_ex==lhu)
{Reg[rt]=memory[(Reg[rs]+SEI)/4]&0xFFFF;
  
   goto p;
}
		else if(ex_latch[1].op_ex==ll)
{Reg[rt]=memory[(Reg[rs]+SEI)/4];
  
   goto p;
}
		else if(ex_latch[1].op_ex==lw)
{ 
		 mem_latch[0].v3=memory[ex_latch[1].v1];
    
 goto p;
}
		else if(ex_latch[1].op_ex==sb)
{memory[(Reg[rs]+SEI)/4]=Reg[rt]&0xFF;

 goto p;
}
		else if(ex_latch[1].op_ex==sh)
{memory[(Reg[rs]+SEI)/4]=Reg[rt]&0xFFFF;
   
   goto p;
}
		else if(ex_latch[1].op_ex==sw)
{
	memory[ex_latch[1].v1]=Reg[ex_latch[1].rt_ex];
 
 
 goto p;
}

 mem_latch[0].v3=ex_latch[1].v1;
 mem_latch[0].v4=ex_latch[1].v2;
 mem_latch[0].op_mem=ex_latch[1].op_ex;
		p: mem_latch[1]=mem_latch[0];
 printf("MEMORY ACCESS ---->  memory[%x]\n",ex_latch[1].v1);
		return 1;
		}









	
int execute_instruction(struct id_latch[2], struct ex_latch[2])
{

	if(ex_latch[1].valid == 0)
	 return 3;
	mem_latch[1].valid=1;
	
ex_latch[0].valid=1;
ex_latch[0].op_ex=id_latch[1].op;
ex_latch[0].rs_ex=id_latch[1].rs;
ex_latch[0].rt_ex=id_latch[1].rt;
ex_latch[0].rd_ex=id_latch[1].rd;

	if(id_latch[1].op==0x0)
{
if(id_latch[1].func==add)
{ex_latch[0].v1=Reg[id_latch[1].rs]+Reg[id_latch[1].rt];
 
   goto p;
}
else if(id_latch[1].func==addu)
{
 ex_latch[0].v1=Reg[id_latch[1].rs]+Reg[id_latch[1].rt];
  
   goto p;
}
else if(id_latch[1].func==and)
{ex_latch[0].v1=Reg[id_latch[1].rs]&Reg[id_latch[1].rt];
   
   goto p;
}
else if(id_latch[1].func==jr)
{pc=Reg[id_latch[1].rs];//Reg[rs];

   goto p;
	}
else if(id_latch[1].func==nor)
{ex_latch[0].v1=~(Reg[id_latch[1].rs]|Reg[id_latch[1].rt]);

  goto p;
}
else if(id_latch[1].func==or)
{ex_latch[0].v1=Reg[id_latch[1].rs]|Reg[id_latch[1].rt];
  
   goto p;
}
else if(id_latch[1].func==slt)
{if(Reg[id_latch[1].rs]<Reg[id_latch[1].rt])
    Reg[id_latch[1].rd]=0x1;
else
    Reg[id_latch[1].rd]=0x0;

  
   goto p;
}
else if(id_latch[1].func==sltu)
{  if(Reg[id_latch[1].rs]<Reg[id_latch[1].rt])
      Reg[id_latch[1].rd]=0x1;
  else
      Reg[id_latch[1].rd]=0x0;

  
  goto p;
}

else if(id_latch[1].func==sub)
{ex_latch[0].v1=Reg[id_latch[1].rs]-Reg[id_latch[1].rt];
  
  goto p;
}
else if(id_latch[1].func==subu)
{ex_latch[0].v1=Reg[id_latch[1].rs]-Reg[id_latch[1].rt];
   
   goto p;
}
}









	else
	{

if (id_latch[1].op==addi)
{ex_latch[0].v1=Reg[id_latch[1].rs]+id_latch[1].SEI;
  
 goto p;
}

else if(id_latch[1].op==addiu)
{
 ex_latch[0].v1=Reg[id_latch[1].rs]+id_latch[1].SEI;
 
  
  goto p;
   
}
else if(id_latch[1].op==andi)
{
  ex_latch[0].v1=Reg[id_latch[1].rs]&id_latch[1].ZEI;
   
   goto p;
}

else if(id_latch[1].op==beq)

{if(Reg[id_latch[1].rt]==Reg[id_latch[1].rs])
id_latch[1].pc4=id_latch[1].pc4+4+id_latch[1].BA;
else 
 
goto p;

}

else if(id_latch[1].op==bne)
{

if(Reg[mem_latch[1].rs_mem] != Reg[mem_latch[1].v3])
{
	pc=pc+id_latch[1].BA-4;
	
goto p;
}
else
 pc=pc+4;

goto p;

}




else if(id_latch[1].op==ori)
{------Reg[id_latch[1].rt]=Reg[id_latch[1].rs] | id_latch[1].ZEI;
  
   goto p;
}
else if(id_latch[1].op==slti)
{
 if(Reg[id_latch[1].rs]<id_latch[1].SEI)
	 ex_latch[0].v1=0x1;
 
else
    ex_latch[0].v1=0x0;
  
   goto p;
}
else if(id_latch[1].op==sltiu)
{ if(Reg[id_latch[1].rs]<id_latch[1].SEI)
    Reg[id_latch[1].rt]=0x1;
else
    Reg[id_latch[1].rt]=0x0;

   
   goto p;
}
else if(id_latch[1].op==sw)
{	ex_latch[0].v1=(Reg[id_latch[1].rs]+id_latch[1].SEI)/4;
	
	
   goto p;
}

else if(id_latch[1].op==lw)
{ex_latch[0].v1=(Reg[id_latch[1].rs]+id_latch[1].SEI)/4;
	

    
 goto p;
}
}
p:ex_latch[1]=ex_latch[0];
printf("EXECUTE ----> value1 is %x\n",ex_latch[0].v1);
return 1;
	}








	


int decode_instruction(struct if_latch[2], struct id_latch[2])
{

	if(id_latch[1].valid == 0)
		{ex_latch[1].valid=0;
        return 3;
	}
	ex_latch[1].valid=1;
	

id_latch[0].op = (if_latch[1].inst &0xFC000000)>>26;



if(id_latch[0].op==0x0)
{printf("DECODE ----> op type is R-type\n");
	id_latch[0].rs = (if_latch[1].inst&0x03E00000)>>21;
id_latch[0].rt = (if_latch[1].inst&0x001F0000)>>16;
id_latch[0].rd = (if_latch[1].inst&0x0000F800)>>11;
id_latch[0].shamt=(if_latch[1].inst&0x000007C0)>>6;
id_latch[0].func = (if_latch[1].inst&0x0000003F);




	if(id_latch[0].func==sll)
	{id_latch[0].wReg=Reg[id_latch[1].rt];
   
 goto p;
}
else if(id_latch[0].func==srl)
{id_latch[0].wReg=Reg[id_latch[1].rt];
   
   goto p;
}

}
else{
if(id_latch[0].op==j || id_latch[0].op==jal)
	{printf("DECODE ----> op type is J-type\n");

id_latch[0].address = (if_latch[1].inst&0x03FFFFFF);
	id_latch[0].JA = (pc & 0xf0000000) | id_latch[0].address << 2;
	
pc=id_latch[0].JA;

	 if(id_latch[0].op==jal){

      Reg[31]=pc+8;
   goto p;
   }

}
else
{printf("DECODE ----> op type is I-type\n");
id_latch[0].rs = (if_latch[1].inst&0x03E00000)>>21;
id_latch[0].rt = (if_latch[1].inst&0x001F0000)>>16;
id_latch[0].imm =(if_latch[1].inst&0x0000FFFF);

if(id_latch[0].imm >= 0x8000)
 {id_latch[0].SEI = (short)id_latch[0].imm;}
else
{id_latch[0].SEI=id_latch[0].imm;}



id_latch[0].ZEI = id_latch[0].imm;
	id_latch[0].BA = id_latch[0].SEI << 2;


 if(id_latch[0].op==lui)
{id_latch[0].wReg=id_latch[0].imm>>16;
    
	goto p;
}

}
}

p: id_latch[1]=id_latch[0];
	
return 1;
}







int fetch_instruction(struct if_latch[2], struct if_latch[2], struct id_latch[2], struct ex_latch[2])
{
	

	if(if_latch[1].valid==0)
		return 1;
	
if(pc==-1)
	{id_latch[1].valid=0;
return 3;
}
id_latch[1].valid=1;
	if_latch[0].inst = memory[pc/4];
	printf("FETCH ----> instruction   %x\n", if_latch[0].inst);
	
	
	if_latch[1]=if_latch[0];
	
	if(id_latch[1].op==j || id_latch[1].op==jal || ex_latch[1].op_ex==bne)
		return 1;
	
	pc=pc+4;
	 return 1;
}







int main(void)
{   
Reg[31]=-1;
Reg[29]=0x8000;

	FILE *fd;
fopen_s(&fd,"simple2.bin","rb");
	
	while(!feof(fd)){
int a;

a=fread(memory+pc,4,1,fd);
   memory[pc]=(*(memory+pc)&0xFF000000)>>24|(*(memory+pc)&0xFF0000)>>8|(*(memory+pc)&0xFF00)<<8|(*(memory+pc)&0xFF)<<24;
    pc++;
}
	pc=0;




do{
	cycle++;
	if_latch[1].valid=1;
	
ret=write_back(&mem_latch[1], &wb_latch[1]);
if(ret==5)break;

	
ret=memory_access(&ex_latch[1], &mem_latch[1]);
if(ret==5)break;

ret=execute_instruction(&id_latch[1], &ex_latch[1]);
if(ret==5)break;

ret=decode_instruction(&if_latch[1], &id_latch[1]);
if(ret==5)break;

ret=fetch_instruction(&if_latch[0],&if_latch[1],&id_latch[1],&ex_latch[1]);
if(ret==5)break;



printf("Now cycle is %d\n",cycle);
printf("Value of this program~~!=%d \n\n\n",Reg[2]);




}while(ret != 5);


		

		
		
	return 0;
	}
	/*switch (opcode)
	{
	case 0: printf ("R[rd]=%d\n", R[rd]);
	break;
	case j:
	case jal:
	break;
	default: printf ("R[rt]=%d\n", R[rt]);
	}*/
