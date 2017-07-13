/*
 * inord_list.cpp
 *
 *  Created on: Apr 8, 2017
 *      Author: ranamagdyomran
 */

#include "inorderlist.h"
#include "fileslist.h"
#include "fileSystem.h"
#include <sstream>
#include <string.h>


using namespace std;

inorderlist::inorderlist(){
    head= NULL;
    current= NULL;
    tail= NULL;

}

bool inorderlist::insert( word & x)
{
    link pred;
    link new_node;
    new_node = new node;
    if(new_node ==NULL)
        {return false;}

    else{new_node->node_element=x;
     const char* newfilename=x.retName();
        const char* headfilename=head->node_element.retName();
        if(head==NULL ||strcmp(newfilename, headfilename)<0)
        {  new_node->next_node=head;
            head=new_node;

        }
    else{
        pred = head;
        const char* predfilename=pred->next_node->node_element.retName();
            while (pred->next_node != NULL && strcmp(predfilename , newfilename)<0)
            {
                pred = pred->next_node;
            }
            new_node->next_node = pred->next_node;
            pred->next_node = new_node;
            predfilename=pred->next_node->node_element.retName();



    }
     return true;}
}




bool inorderlist::retrieve_the_first_element(word &x)
{
    if(head ==NULL)
            {return false;}
    else{
        current=head;
        x=current->node_element;

        return true;


    }
}
bool inorderlist::retrieve_the_next_element(word &y){
    if(current==NULL || current->next_node==NULL)
        return false;
    else{
        current=current->next_node;
       y=current->node_element;
        return true;

    }

}






inorderlist::~inorderlist()
{

    link new_node,deleted_node;
    new_node = new node;
    new_node=head;
    while(new_node!=NULL)
    {  deleted_node=new_node;
        new_node=new_node->next_node;

        delete deleted_node ;

    }
    delete new_node;
}

