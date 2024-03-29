#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

#define OR -1
#define AND 1
#define NOT 0


struct node {
    int data;
    struct node *left;
    struct node *right;
};


char stack[MAX];
int top;
int notCnt = 0;

//Stack Functions
void init_stack();
char push(char t);
char pop();
char s_top();
int isEmpty();


void init_stack(){      
    top=-1;
}

char push(char t){
    if(top>=MAX -1){
        printf("Stack overflow.\n");
        return -1;
    }

    stack[++top]=t;
    return t;
}

char pop(){
    if(top<0){
        printf("Stack is empty\n");
        return -1;
    }

    return stack[top--];
}

char s_top(){
    return stack[top];
}

int isEmpty(){
    if(top<=0) return 1;
    else return 0;
}

char* parse(struct node* root, char* exp);
struct node* pushTree(struct node* root, int data);
struct node* negateNode(struct node* root, int data);
struct node* deleteNode(struct node* root, int data);
void distributive(struct node* root);
void printResult(struct node* root);
void negate(struct node* root);
void preOrderPrint(struct node *root);
int check(char* expr);

char* parse(struct node* root, char* exp){

    char* pch;
    pch= strtok(exp, " ");
    root=pushTree(root, check(pch));

     while (pch != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
     {
              // 자른 문자열 출력
        pch = strtok(NULL, " ");
        
        if(pch!=NULL){
            pushTree(root, check(pch));
        }       // 다음 문자열을 잘라서 포인터를 반환
            
    }

    root = negateNode(root, 0);
    for(int i=0; i<notCnt; i++){
        root = deleteNode(root, 0);
    }

    distributive(root);
     preOrderPrint(root); printf("\n");

    return pch;  
}

int check (char* expr){
    if(strcmp(expr, "(not") == 0){
        //printf("expr : not\n");
        notCnt++;
        return 0;
    }else if(strcmp(expr, "(or") == 0){
        //printf("expr : or\n");
        return -1;
    }else if(strcmp(expr, "(and") == 0){
        //printf("expr : and\n");
        return 1;

    //The case of an (n is an integer)
    }else{
        int num = expr[1]-'0';
        return num+1;
    }
}

struct node *newNode(int data){
    struct node *temp = (struct node*) malloc (sizeof(struct node));
    temp->data = data;
    temp->left = temp->right = NULL;
    return temp;
}

int hasBothChild(struct node* node){
    return node!= NULL && node->left != NULL && node->right !=NULL;
}

int hasNoChild(struct node* node){
    return node!=NULL && node->left == NULL && node->right ==NULL;
}

int hasLeftChild(struct node* node){
    return node!=NULL && node->left !=NULL;
}

struct node* pushTree(struct node* node, int data){
    
    //If tree is empty, create root node
  
    if(node==NULL){
       return newNode(data);
    } else {
            //if(node->left==NULL)
        
            //If data is 0 (= NOT), go to left.
            if(node->data==0){
                
                if(node->left==NULL){
                //printf("pushed left : %d\n\n", data);
               node->left = pushTree(node->left, data);
                }else{
                    pushTree(node->left, data);
                }
            //If data is -1 (= OR or AND), go to the left.
            } else if(node->data==-1 || node->data==1){
                
                if(node->left==NULL){
                    //("pushed left : %d\n\n", data);
                    node->left = pushTree(node->left, data);
                }
                else if(node->right==NULL){

                    if(node->left->data==0){
                        if(!hasBothChild(node->left->left))
                            pushTree(node->left, data);
                        else{
                            //printf("pushed right : %d\n\n", data);
                            node->right= pushTree(node->right, data);
                        }
                    }
                    else{
                    //printf("pushed right : %d\n\n", data);
                    node->right= pushTree(node->right, data);
                    }
                }else {
                    pushTree(node->right, data);
                }
            //Else, go to the right of the tree
            }else
                return node;
        
           
    }

    return node;

}

struct node* negateNode(struct node* root, int data){

    if(root==NULL) {
        printf("Tree is empty\n");
        return NULL;
    }else{
        //If value is found in the given binary tree then, set the flag to true
        if(root->data == data){
            negate(root);

        }//If node has left, go to left
         if(root->left != NULL){
            negateNode(root->left, data);
        }//If node has right, go to right
         if(root->right != NULL){
            negateNode(root->right, data);
        }

   
    }
    return root;
}

struct node* deleteNode(struct node* root, int data){
   

    if(root==NULL){
        return NULL;
    }else{
        if(root->data == data){
            struct node* temp = (struct node*)malloc(sizeof(struct node));
            temp = root->left;
            //printf("node deleted : %d\n", temp->data);
            free(root);
            return temp;
        }else{
           
            root->left=deleteNode(root->left, data);
        
            root->right=deleteNode(root->right, data);
        
        }
         

        return root;

    }

}

void negate(struct node* root){
    if(root==NULL) return;
    else{
        root->data = root->data*-1;

        if(root->left!=NULL){
            negate(root->left);
        }

        if(root->right!=NULL){
            negate(root->right);
        }
    }
}

void distributive(struct node* root){
    //Copy the Root's OP

    if(root==NULL) return;

    int center = root->data;

    //LC's OP Will be the Root's OP.

    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data = root->data;
    temp->left = root->left;
    root->left = temp;

    //Exchange Root's OP And Right's child OP

    int tmp = root->data;
    root->data = root->right->data;
    root->right->data = tmp;

    //Move RC's LC to LC's RC
    struct node* t2 = (struct node*)malloc(sizeof(struct node));
    t2->data = root->right->left->data;
    t2->left = root->right->left->left;
    t2->right = root->right->left->right;
    root->left->right = t2;

    //Move LC to RC's LC.
    struct node* t3 = (struct node*)malloc(sizeof(struct node));
    t3->data = root->left->left->data;
    t3->left = root->left->left->left;
    t3->right = root->left->left->right;
    root->right->left = t3;
    //root->right->left = root->left->left;
}

void preOrderPrint(struct node *root){
 
    if(root==NULL)
        return;


      preOrderPrint(root->left);
    if(root->data == 1){
        printf("\n");
    }
    if(root->data != 0 && root->data != 1 && root->data != -1){
        int value = root->data;
        if(value<0) value+=1;
        else value-=1;
        printf("%d ", value);
    }
  
    preOrderPrint(root->right);
    
}

int main(){

    char exp[MAX];


    printf("Enter the expression : ");
    gets(exp);
    //char exp[]="(or a1 (not (or (not (or a2 a3)) a4)))";
    //char exp[]="(or (not (or (not (or a2 a3)) a4)) a1)";

    for(int i=0; i<strlen(exp); i++){
       
        if(exp[i]=='(') push(exp[i]);
        else if(exp[i]==')'){
            if(s_top()=='(') pop();
            else push(exp[i]);
        }
    }

    if(!isEmpty()){
        printf("Wrong input.. Exit program!\n");
        return -1;
    }

    struct node* root = NULL;
    parse(root, exp);

}

