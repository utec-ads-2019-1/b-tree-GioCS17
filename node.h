#ifndef NODE_H
#define NODE_H

#include <vector>
#include<cmath>

using namespace std;

class Node {

    unsigned int size;
    vector<unsigned int> keys;
    vector<Node*> childs;
    int numKeys;
    int numChilds;
    bool isLeaf;
    int miniDegree;

    public:
        Node(unsigned int size, bool isLeaf = true) : size(size), isLeaf(isLeaf) {
            keys.resize(size - 1);
            childs.resize(size);
            numKeys=0;
            numChilds=0;
            miniDegree=ceil(size*1.0/2);
        }
        Node* getChild(int pos){
          return childs[pos];
        }
        int findKey(int data){
          int index=0;
          while(index<numKeys && keys[index]<data)
            index++;
          return index;
        }
        void removeFromLeaf(int index){
          for(int i=index+1;i<numKeys;i++)
            keys[i-1]=keys[i];
          numKeys--;
        }
        int getPred(int index){
           Node *curr=childs[index];
           while (!curr->isLeaf)
             curr = curr->childs[curr->numKeys];
           return curr->keys[curr->numKeys-1];
        }
        int getSucc(int index){
          Node *curr = childs[index+1];
          while (!curr->isLeaf)
            curr = curr->childs[0];
          return curr->keys[0];
        }
        void removeFromNonLeaf(int index){
          int aux=keys[index];
          if(childs[index]->numKeys>=miniDegree){
            int pred=getPred(index);
            keys[index]=pred;
            childs[index]->remove(pred);
          }
          else if(childs[index+1]->numKeys>=miniDegree){
            int succ=getSucc(index);
            keys[index]=succ;
            childs[index+1]->remove(succ);
          }
          else{
            merge(index);
            childs[index]->remove(aux);
          }
        }
        void merge(int index){
          Node* child=childs[index];
          Node* sibling=childs[index+1];

          child->keys[miniDegree-1]=keys[index];

          for(int i=0;i<sibling->numKeys;i++)
            child->keys[i+miniDegree]=sibling->keys[i];

          if(!child->isLeaf)
            for(int i=0;i<=sibling->numKeys;i++)
              child->childs[i+miniDegree]=sibling->childs[i];

          for(int i=index+2;i<=numKeys;i++)
            childs[i-1]=childs[i];
          child->numKeys+=sibling->numKeys+1;
          numKeys--;
          delete sibling;
        }
        void fill(int index){
          if(index!=0 && childs[index-1]->numKeys>=miniDegree)
            borrowFromPrev(index);
          if(index!=numKeys && childs[index+1]->numKeys>=miniDegree)
            borrowFromNext(index);
          else{
            if(index!=numKeys)
              merge(index);
            else
              merge(index-1);
          }
        }
        void borrowFromPrev(int index){
          Node* child=childs[index];
          Node* sibling=childs[index-1];

          for(int i=child->numKeys-1;i>=0;i--)
            child->keys[i+1]=child->keys[i];

          if(!child->isLeaf)
            for(int i=child->numKeys;i>=0;i--)
              child->childs[i+i]=child->childs[i];

          child->keys[0]=keys[index-1];
          if(!child->isLeaf)
            child->childs[0]=sibling->childs[sibling->numKeys];

          keys[index-1]=sibling->keys[sibling->numKeys-1];

          child->numKeys++;
          sibling->numKeys--;
        }
        void borrowFromNext(int index){
          Node* child=childs[index];
          Node* sibling=childs[index+1];

          child->keys[child->numKeys]=keys[index];

          if(!child->isLeaf)
            child->childs[child->numKeys+1]=sibling->childs[0];

          keys[index]=sibling->keys[0];

          for(int i=1;i<sibling->numKeys;i++)
            sibling->keys[i-1]=sibling->keys[i];

          if(!sibling->isLeaf)
            for(int i=1;i<=sibling->numKeys;i++)
              sibling->childs[i-1]=sibling->childs[i];

          child->numKeys++;
          sibling->numKeys--;
        }
        bool  remove(int data){
          int index=findKey(data);
          if(index<numKeys && keys[index]==data){
            if(isLeaf)
              removeFromLeaf(index);
            else
              removeFromNonLeaf(index);
            return true;
          }
          else{
            if(isLeaf)
              return false;

            bool flag=false;
            if(index==numKeys)
              flag=true;
            if(childs[index]->numKeys<miniDegree)
              fill(index);
            if(flag && index>numKeys)
              return childs[index-1]->remove(data);
            else
              return childs[index]->remove(data);

          }
        }
        /**
         * An alternative is to create two different nodes (Internal and Leaf) that inherite from Node
         * an implement this function
         */
        //virtual bool isLeaf() = 0;

    friend class BTree;
};

#endif
