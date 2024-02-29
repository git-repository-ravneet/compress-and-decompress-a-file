/*IMPLEMENTATION OF HUFFMAN COMPRESSION TECHNIQUE USING HEAP AND PRIORITY QUEUE*/  

/*Huffman coding involves building a Huffman tree where characters with lower frequencies have higher priorities. A priority queue is used to efficiently select and merge nodes with the lowest frequency during the construction of the Huffman tree. Further the priority queue is implemented using heaps due to their efficient insertion and removal of elements with the highest (or lowest) priority */

#include<string.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#define SIZE 1000   //Maximum size of heap

typedef struct node
{
        char ch;
        int freq;
        struct node *left;
        struct node *right;
} *NODE;

NODE heap[SIZE];
int heapPtr=0;
int p=0;

struct codes {
     char c,code[20];
};
struct codes c_code[128];

NODE createNode(char c,int freq,NODE left,NODE right);
void create_pq(NODE t);
void insertHeap(NODE ptr);
int minimum(int l,int r);
NODE deleteRoot();
void gen_code(NODE temp,char code[],int c);
void set_right(NODE p,char x );
void set_left(NODE p, char x);
void post_order(NODE t);
void compress(FILE *read,FILE *write,codes c_code[]);
void decompress(FILE *read,FILE *write,NODE tree);
	
//Function to create new node
NODE createNode(char c,int freq,NODE left,NODE right)
{
	NODE NEW= (NODE)malloc(sizeof(struct node));
	NEW->ch = c;
	NEW->freq =freq;
	NEW->left =left;
	NEW->right =right;
	return NEW;
}

//Function to insert data in heap
void insertHeap(NODE ptr)
{
	int parent,child;
	NODE temp;
	if(heapPtr==0)
	{	
		heap[0]=ptr;
		heapPtr++;
		return;
	}
	child=heapPtr;
	heap[heapPtr++] = ptr;
	while(child>0)
	{
		parent=(child-1)/2;
		if(heap[child]->freq < heap[parent]->freq)
		{
			temp=heap[parent];
			heap[parent]=heap[child];
			heap[child]=temp;
			child=parent;
		}
		else
			return;
	}
}

void set_right(NODE p,char x )
{
	NODE tree_node=(NODE) malloc(sizeof(struct node));
	tree_node->ch=x;
	tree_node->freq=1;
	tree_node->left=tree_node->right=NULL;
	p->right=tree_node;
}

void set_left(NODE p, char x)
{
	NODE tree_node=(NODE) malloc(sizeof(struct node));
	tree_node->ch=x;
	tree_node->freq=1;
	tree_node->left=tree_node->right=NULL;
	p->left=tree_node;
}

void post_order(NODE t)
{
	if(t!=NULL)
	{
		post_order(t->left);
		post_order(t->right);
		printf("%c - %d\t\t",t->ch,t->freq);
	}
}

void create_pq(NODE t)
{
	if(t!=NULL)
	{
		create_pq(t->left);
		create_pq(t->right);
		t->left=NULL;
		t->right=NULL;
		insertHeap(t);
	}
}

NODE deleteRoot()
{
	NODE temp,ptr= heap[0];
	int parent=0,lchild=1,rchild=2;
	int minchild;
	heap[0]=heap[--heapPtr];
	if(heapPtr==0)
		return ptr;
	minchild=minimum(lchild,rchild);
	while(minchild!=-1 && (heap[parent]->freq > heap[minchild]->freq) && lchild<heapPtr)
	{
		temp=heap[parent];
		heap[parent]=heap[minchild];
		heap[minchild]=temp;
		parent=minchild;
		lchild=parent*2+1;
		rchild=lchild+1;
		minchild=minimum(lchild,rchild);
	} 
	return ptr;
}
int minimum(int l,int r)
{
	if(l>=heapPtr)
		return -1;
	else if(r>=heapPtr)
		return l;
	else if(heap[l]->freq < heap[r]->freq)
		return l;
	else 
		return r;
}

void gen_code(NODE temp,char code[],int c)
{
	if(temp)
	{
		if(temp->left==NULL && temp->right==NULL)
		{
			c_code[p].c=temp->ch;
			strcpy(c_code[p].code,code);
			p++;
			printf("Char: %c & code: %s\n",temp->ch,code);
 			c--;
			return;
		}
		code[c]='0';
		code[c+1]='\0';
		gen_code(temp->left,code,c+1);
		code[c]='1';
		code[c+1]='\0';
		gen_code(temp->right,code,c+1);
	}
}

//Function to compress the input file
void compress(FILE *read,FILE *write,codes c_code[])
{
	char ch,x=0;
	int count_shift=8,i,bit,f,j,compress_file_count=0;
	while((fscanf(read,"%c",&ch))!=EOF)
	{
		//printf("%c ",ch);
		i=0,f=0;
		while((i<128) && (f==0))
		{
			if(ch==c_code[i].c)
			{
				int l = strlen(c_code[i].code);
				for(j=0;j<l;j++)
				{
					bit=(c_code[i].code[j] - '0');
					x=x|bit;
					//printf("%d,",x);
					count_shift--;
					if(count_shift==0)
					{
						//printf(" File: %d,%c\n",x,x);
						fputc(x,write);
						compress_file_count=compress_file_count+8;
						x=0;
						count_shift=8;
					}
					x=x<<1;
					//printf("%d  ",x);
					f=1;
				}
			}
			i++;
		}
	}
	if(count_shift!=8)
	{
		x=x<<(count_shift-1);
		compress_file_count=compress_file_count+count_shift;
		fputc(x,write);
	}
	printf("\nNUMBER OF BITS STORED IN COMPRESSED FILE - %d",compress_file_count);
}

//Function to decompress the file
void decompress(FILE *read,FILE *write,NODE tree)
{
	NODE current=tree;
	int bit,i,c_f_count=0,d_f_count=0;
	char mask=1<<7,c;
	//printf("\n");
	while((fscanf(read,"%c",&c))!=EOF)
	{
		//printf("\n%d- ",c);
		c_f_count=c_f_count+8;
		for(i=0;i<8;i++)
		{
			bit=c & mask;
			c=c<<1;
			bit=bit>>7;
			if(bit<0)
				bit=-(bit);
			if(bit==0)
				current=current->left;
			else
				current=current->right;
			if(current->ch!=0)
			{
				//printf("%c ",current->ch);
				fputc(current->ch,write);
				d_f_count=d_f_count+1;
				current=tree;
			}
		}
	}
	printf("\nNUMBER OF BITS READ FROM COMPRESSED FILE - %d",c_f_count);
	printf("\nNUMBER OF CHARACTERS STORED IN DECOMPRESSED FILE - %d",d_f_count);
}		

void main()
{
		int i,k,f_count=0;
        		char s,code[20];      
		c_code[128].c=NULL;
		c_code[128].code[20]=NULL;
		NODE temp,Root=NULL,parent,child,f,huff_root;
        		FILE *read,*write;
		read= fopen("text1.txt","r");
		if(read==NULL)
		{
			printf("\n INPUT DATA FILE COULD NOT BE OPENEDned");
			getch();
			exit(0);
		}
		printf("******PROGRAM TO IMPLEMENT HUFFMAN COMPRESSION TECHNIQUE*****\n");
		while((fscanf(read,"%c",&s))!=EOF)
		{
			f_count++;
			if(Root==NULL)
				Root=createNode(s,1,NULL,NULL);
			else
			{
				parent=child=Root;
				while ((s!=parent->ch)&&(child!=NULL))
				{
					parent=child;
					if(s<parent->ch)
			
						child=parent->left;
					else
						child=parent->right;
				}
				if(s==parent->ch)
					parent->freq=parent->freq+1;
				else
					if(s<parent->ch)
						set_left(parent,s );
					else
						set_right(parent,s);
			}
		}
		fclose(read);
		printf("\nTOTAL NUMBER OF CHARACTERS IN INPUT FILE - %d\n",f_count);
		/*printf("\nPost_Order Traversing of BST after counting frequency of each character: \n");
		post_order(Root);		*/
		printf("\n");
		create_pq(Root);
		/*printf("\nPriority Queue Traversing:\n");
		for(i=0;i<heapPtr;i++)
			printf("%c-%d\t",heap[i]->ch,heap[i]->freq); 		*/
		k=heapPtr;
		for(i=0;i<k-1;i++)
        		{
                		NODE left = deleteRoot();
               		NODE right = deleteRoot();
                		temp = createNode(0,left->freq + right->freq,left,right);
                		insertHeap(temp);
		} 
		/*printf("\n\nPost_Order Traversing of HUFFMAN TREE: \n");
		post_order(heap[0]);  		*/
		huff_root = temp;
		printf("FREQUENCY OF ROOT NODE OF HUFFMAN TREE - %d\n",huff_root->freq);
		printf("\nHUFFMAN CODES FOR FILE DATA :\n");
		gen_code(huff_root,code,0);
		read= fopen("text1.txt","r");
		write=fopen("text2.txt","w");
		if(read==NULL)
		{
			printf("\n INPUT DATA FILE COULD NOT BE OPENED");
			getch();
			exit(0);
		}
		if(write==NULL)
		{
			printf("\n FILE TO WRITE INTERMEDIATE COMPRESSED DATA COULD NOT BE OPENED");
			getch();
			exit(0);
		}
		compress(read,write,c_code);
		fclose(read);
		fclose(write);
		read=fopen("text2.txt","r");
		write=fopen("text3.txt","w");
		if(read==NULL)
		{
			printf("\n FILE TO READ INTERMEDIATE COMPRESSED DATA COULD NOT BE OPENED");
			getch();
			exit(0);
		}
		if(write==NULL)
		{
			printf("\n FILE TO WRITE DECOMPRESSED DATA COULD NOT BE OPENED");
			getch();
			exit(0);
		}
		decompress(read,write,huff_root);
		fclose(read);
		fclose(write);
		read=fopen("text3.txt","r");
		if(read==NULL)
		{
			printf("\n FILE TO READ DECOMPRESSED DATA COULD NOT BE OPENED");
			getch();
			exit(0);
		}
		printf("\n\nCONTENTS OF DECOMPRESSED FILE : \n");
		while((fscanf(read,"%c",&s))!=EOF)
		{
			printf("%c",s);
		}		
		fclose(read);
		getch();
}
