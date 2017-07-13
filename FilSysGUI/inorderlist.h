/*
 * inorderlist.h
 *
 *  Created on: Jun 22, 2017
 *      Author: rana
 */

#ifndef INORDERLIST_H_
#define INORDERLIST_H_
#include <iostream>
#include <fstream>
#include <string>
#include "fileslist.h"
using namespace std;
typedef fileslist word;
class inorderlist {
public:
    inorderlist();
    bool insert(word & x);
    bool retrieve_the_first_element(word & x);
    bool retrieve_the_next_element(word & x);
    virtual ~inorderlist();


private:
    struct node;
    typedef node* link;
    struct node{
        word node_element;
        link next_node;

    };
link head;
link current;
link tail;





};

#endif /* INORDERLIST_H_ */

