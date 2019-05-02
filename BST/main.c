//
//  main.c
//  bst
//
//  Created by 김희주 on 02/05/2019.
//  Copyright © 2019 김희주. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

//메모리 할당 매크로 함수
#define MALLOC(p, s)\
if (!((p) = malloc(s))) {\
fprintf(stderr, "Insufficient memory");\
exit(EXIT_FAILURE);\
}


typedef struct node * treePointer;
typedef struct node{
    int data;
    treePointer leftChild, rightChild;
}; // root 포인터 생성
treePointer root;


void inorder(treePointer ptr)
{ /* 중위 트리 순회 함수*/
    if (ptr) {
        inorder(ptr->leftChild);
        printf("%d ", ptr->data);
        inorder(ptr->rightChild);
    }
}



treePointer search(treePointer root, int key)
{ /* 키값이 key인 노드에 대한 포인터를 반환함.
   그런 노드가 없는 경우에는 NULL을 반환 */
    if (!root) return NULL;
    if (key == root->data) return root;
    if (key < root->data)
        return search(root->leftChild, key);
    return search(root->rightChild, key);
}
treePointer modifiedSearch(treePointer lead, int key)
{ /* key값이 존재하면 NULL, 없으면 뒤 따라온 trail 노드 리턴 */
    treePointer trail=NULL;
    
    while (lead) {
        trail = lead;
        
        if(key<lead->data) lead = lead->leftChild;
        else if(key==lead->data) return NULL;
        else lead = lead->rightChild;
    }
    return trail;
}






void insert (treePointer *node, int key)
{/* 트리내의 노드가 key를 가리키고 있으면 아무 일도 하지 않음;
  그렇지 않은 경우는 data=key인 새 노드를 첨가 */
    
    
    // 루트에서 시작하여 key를 찾아라
    treePointer ptr, temp = modifiedSearch(*node, key);
    
    if(temp || !(*node)) {
        /* key값이 트리에 없다면 */
        MALLOC(ptr, sizeof(*ptr));
        ptr->data = key;
        ptr->leftChild = ptr->rightChild = NULL;
        if(*node) //공백 트리가 아니면
            
            
            // temp가 가리키는 자식 노드에 ptr넣음
            if(key<temp->data) temp->leftChild = ptr;
            else temp->rightChild = ptr;
            else *node = ptr; // 값이 없었다면 첫노드 생성
    }
}


int deleteFull(treePointer* ptr)
{
    int leftMax;
    treePointer temp;
    
    /*기준 노드의 왼쪽 sub트리 중 가장 큰 값의 노드를 찾아 왼쪽 자식 링크를 변경*/
    
    if ((*ptr)->rightChild==NULL) {
        leftMax = (*ptr)->data; //삭제될 노드의 왼쪽 sub트리 중 값이 가장 큰 노드를 저장
        temp = *ptr;//sub트리 가장 오른쪽 단말 노드를 temp에 저장
        *ptr = (*ptr)->leftChild;
        /* *ptr은 삭제될 트리의 왼쪽 sub트리 중 가장 큰 값을 갖고 있는 노드이며
         자신의 원래 값은 leftMax에 저장해 반환시키고 자신의 위치에는 자신의
         왼쪽 자식을 넣는다.*/
        
        free(temp); // 연결이 끊어진 동적메모리를 해제시켜 메모리 낭비를 없앤다.
        return leftMax; // leftMax 노드 값을 리턴해 삭제할 노드에 값을 바꿔준다.
    } else {
        //오른쪽 자식이 있을 경우 deleteFull()에 오른쪽 자식을 넣고 재호출한다.
        return deleteFull(&(*ptr)->rightChild);
    }
}




void deleteNode(treePointer* lead, int key){
    treePointer temp;
    
    if (*lead) {
        //삭제될 key과 트리의 데이터를 비교 작으면 왼쪽 sub트리로 함수 재호출
        if (key < (*lead)->data) {
            deleteNode(&(*lead)->leftChild, key);
        }
        
        //삭제될 key과 트리의 데이터를 비교 크면 오른쪽 sub트리로 함수 재호출
        else if (key > (*lead)->data) {
            deleteNode(&(*lead)->rightChild, key);
        }
        
        //트리 노드의 좌/우 자식이 없으면 *lead를 삭제하고 NULL값으로 초기화
        else if ((*lead)->leftChild==NULL && (*lead)->rightChild==NULL){
            free(*lead);
            *lead = NULL;
        }
        
        
        //트리 노드의 왼쪽 자식이 없으면 *lead에 오른쪽 자식을 넣고 자신은 삭제
        else if ((*lead)->leftChild==NULL){
            temp=*lead;
            *lead = (*lead)->rightChild;
            free(temp);
        }
        
        //트리 노드의 오른쪽 자식이 없으면 *lead에 왼쪽 자식을 넣고 자신은 삭제
        else if ((*lead)->rightChild==NULL) {
            temp=*lead;
            *lead = (*lead)->leftChild;
            free(temp);
        }
        
        
        // 트리에 좌/우 자식이 모두 붙어 있는 경우
        else {
            (*lead)->data = deleteFull(&(*lead)->leftChild);
            //삭제될 위치에 왼쪽 sub트리 중에 가장 큰 값으로 바꾼다.
        }
    }
    
    return;
}











int main()
{
    
    int menuSelect, count, inputKey, i;
    
    while(1){
        printf("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        printf("\n┃ (1) 삽입  (2) 삭제  (3) 출력  (4) 종료   ┃");
        printf("\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        printf("\n 메뉴 선택 : ");
        
        scanf("%d", &menuSelect); // 메뉴선택값 입력받음
        
        switch(menuSelect)
        {
            case 1:// 삽입
                printf(" 삽입하려는 키 갯수 : ");
                scanf("%d", &count);
                printf(" 삽입하려는 키 값 : ");
                for (i=0 ; i < count ; i++){
                    scanf("%d",&inputKey);
                    insert (&root, inputKey);
                }
                break;
                
                
            case 2:// 삭제
                if(root!=NULL) { // 트리가 비어있지 않을경우
                    printf(" 삭제하려는 키 갯수 : ");
                    scanf("%d", &count);
                    printf(" 삭제하려는 키 값 : ");
                    for (i=0 ; i < count ; i++){
                        scanf("%d", &inputKey);
                        deleteNode(&root, inputKey);
                    }
                }else printf(" 트리가 생성되어 있지 않습니다.\n");
                
                break;
                
                
            case 3:// 출력
                if(root!=NULL) { // 트리가 비어있지 않을경우
                    printf(" 삽입된 데이터의 중위순회 탐색 출력 : ");
                    inorder(root);
                    printf("\n");
                }
                else printf(" 트리가 생성되어 있지 않습니다.\n");
                
                break;
                
            case 4:// 종료
                return 0;
                
            default:
                printf(" 다시입력하세요.\n");
                return 1;
        } // switch문 종료
        
    } // while문 종료
}
