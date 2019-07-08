#ifndef BTREE_H
#define BTREE_H

#include "node.h"
#include<iostream>
#include<stack>
#include<cmath>

class BTree {
    private:
        Node* root;
        unsigned int degree;
        unsigned int minDegree;
        int ** ref;

        /*
        //Auxiliar methods
        */
        void printChilds(Node* node){
          int i;
          cout<<"\nEs hoja::"<<node->isLeaf<<" ";
          for(i=0;i<node->numKeys;i++){
            if(!node->isLeaf)
              printChilds(node->childs[i]);
            cout<<node->keys[i]<<" ";
          }
          cout<<endl;
          if(!node->isLeaf)
              printChilds(node->childs[i]);
        }

        void splitChild(int i,Node *tmp,Node *father){

          Node * child=new Node(degree,tmp->isLeaf);
          child->numKeys=minDegree-1;
          int j;
          for(j=0;j<minDegree-1;j++){
            cout<<"\n Elemento a hijo derecho::"<<tmp->keys[j+minDegree];
            child->keys[j]=tmp->keys[j+minDegree];
          }


          tmp->numKeys=minDegree-1;

          if(!tmp->isLeaf)
            for(j=0;j<minDegree;j++)
              child->childs[j]=tmp->childs[j+minDegree];


          cout<<"\nKeys de father "<<father->numKeys<<" --"<<endl;
          for(j=father->numKeys;j>=i+1;j--)
            father->childs[j+1]=father->childs[j];

          father->childs[i+1]=child;

          for(j=father->numKeys-1;j>=i;j--)
            father->keys[j+1]=father->keys[j];

          father->keys[i]=tmp->keys[minDegree-1];

          father->numKeys++;
        }
        void insertNonFull(int data,Node * node){
          int i=node->numKeys-1;

          if(node->isLeaf){
            while(i>=0 && (node->keys[i]>data || node->keys[i]==0)){
           // while(i>=0 && node->keys[i]>data){
              if(node->keys[i]==0)
                node->numKeys--;
              node->keys[i+1]=node->keys[i];
              i--;
            }

            node->keys[i+1]=data;
            node->numKeys++;
          }
          else{
            while(i>=0 && node->keys[i]>data)
              i--;

            if(node->childs[i+1]->numKeys==degree-1){
              splitChild(i+1,node->childs[i+1],node);

              if(node->keys[i+1] < data)
                i++;
            }

            insertNonFull(data,node->childs[i+1]);
          }
        }

    public:
        BTree(unsigned int degree) : degree(degree), root(nullptr) {
          std::cout<<"Hola"<<std::endl;
          minDegree=ceil(degree*1.0/2);
        }

        bool search(int value){
          ref=nullptr;
          if(!root)
            return false;

          int i=0;
          Node* tmp = root;

          while(1){
            while(i<tmp->numKeys && tmp->keys[i]>value)
              i++;
            if(tmp->keys[i]==value){
              return true;
            }
            if(tmp->isLeaf)
              return false;
          }
        }

        bool insert(int data) {

          if(!root){
            root=new Node(degree,true);
            root->keys[0]=data;
            root->numKeys=1;
            return true;
          }

          if(root->numKeys==degree-1){
            Node* node=new Node(degree,false);
            node->childs[0]=root;
            splitChild(0,root,node);
            int i=0;
            if(node->keys[0]<data)
              i++;
            insertNonFull(data,node->childs[i]);
            root=node;
          }
          else
            insertNonFull(data,root);

          return true;
        }

        bool remove(int data) {
          if(!root)
            throw printf("No hay elementos\n");

          root->remove(data);

          if(root->numKeys==0){
            Node *tmp=root;
            if(root->isLeaf)
              root=nullptr;
            else
              root=root->childs[0];
            delete tmp;
          }
          return true;
        }

        void print(){
          Node *tmp=root;
          if(!root)
            throw printf("No hay elementos en btree\n");

          int i;
          cout<<"\nEs hoja::"<<tmp->isLeaf<<" ";
          for(i=0;i<tmp->numKeys;i++){
            if(!tmp->isLeaf)
              printChilds(tmp->childs[i]);
            cout<<tmp->keys[i]<<" ";
          }
          if(!tmp->isLeaf)
              printChilds(tmp->childs[i]);

          cout<<endl;
        }

        ~BTree(){}
};

#endif
