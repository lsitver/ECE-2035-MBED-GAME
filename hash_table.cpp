//=================================================================
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

/*
 Student Name: Lilly Sitver
 Date:10-28-2020

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
HashTableEntry* x = (HashTableEntry*)malloc(sizeof(HashTableEntry));//create a hashtable x and this allocates a block of space/memory on heap
x->key=key;//initialize components of x hashtable struct
x->value=value;
x->next=NULL;
return x;//return hash table struct
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/

static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    unsigned int bucketNum = hashTable->hash(key);//get bucket number
    HashTableEntry* temp = hashTable->buckets[bucketNum];//get head of bucket
    
    while(temp!=NULL){//goes through hashtable while temp is not NULL
        if (temp->key ==key){
            return temp;//if key exists return hash table
        }
        temp = temp ->next;//if key is not found continue on to the next node
    }
    return NULL;//if no key is present return NULL
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable){
   HashTableEntry* a;//new hashtable a created
    for(unsigned int i=0; i<hashTable->num_buckets;i++){//for loop to iterate through all buckets in hashtable
        a=hashTable->buckets[i];//accesses all buckets
        while(a){//while their are buckets
            HashTableEntry* b = a->next;//hashtable b is pointing to the next node b should be ahead of a by 1
            free(a->value);//free a's value of entry
            free(a);//free a's entry
            a = b;//gets next node to continue to go through while loop
        }
    }
    free(hashTable->buckets);//free buckets
    free(hashTable);//free hashtable
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
    unsigned int insert = hashTable->hash(key);
    HashTableEntry * dest = findItem(hashTable, key);//checks for keys in dest hashtable
    void*temp=NULL;
    if(dest){
        temp=dest->value;
        dest->value=value;//initialize components of dest hashtable struct
        dest->key=key;
    }
    else{//if no dest
        HashTableEntry* y  = createHashTableEntry(key,value);//new hashtable y
        y->next=hashTable->buckets[insert];
        hashTable->buckets[insert]=y;//create new hashtable entry in y and put bucket value in entries
    }
    return temp;
}


void* getItem(HashTable* hashTable, unsigned int key) {
    HashTableEntry* treasure=findItem(hashTable, key);//loops through list from head and looks for matches to key
    if(treasure){//if key in hashtable
        return(treasure->value);}//return value of key in hashtable
    else{
        return NULL;//if no key match return NULL
    }
}

void* removeItem(HashTable* hashTable, unsigned int key) {
    unsigned int index = hashTable->hash(key);
    HashTableEntry* current = hashTable->buckets[index];//current hashtable is accessing index of buckets
    HashTableEntry* pre = NULL;
    while(current){
            if (current->key == key){//checks if key in current hashtable equals unsigned int key
                if(pre !=NULL){//if pre has a actual value you want to continue to go through the hashtable
                    pre->next=current->next;// by relinking
                }else{//if pre = NULL
                    hashTable->buckets[index]=current->next;
                }
                void*value=current->value;//add a void/stand alone statement that has the hashtable current pointing to the value
                free(current);//frees the hashtable entry from heap this does not free the value in the entry
                return value;//return value
                }
        pre=current;//set pre hashtbale to current hashtable
        current=current->next;//current goes to next node
        }
        return NULL;
    }

void deleteItem(HashTable* hashTable, unsigned int key) {
    free(removeItem(hashTable,key));//frees/deletes the hashtable value by calling removeItem and freeing what removeItem returns based on key
    return;
}
