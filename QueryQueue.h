//QueryQueue

#ifndef INCLUDE_QueryQueue
#define INCLUDE_QueryQueue

#include "constants.h"

class QueryQueue {
  
 private:

  Triple* data;
  bool* queryStatus;
  int size;

  //data and queryStatus are of the same size

  public:

  //Pre: id and address describes some node
  //Post: data[0] = triple object containing id and address
  //      queryStatus[0] = false, size = 1
  QueryQueue(uint32_t id, uint32_t address);

  //Pre: N/A
  //Post: data and queryStatus are empty, size = 0
  QueryQueue();

  //Pre: N/A
  //Post: data and queryStatus are deleted
  ~QueryQueue();

  //Pre: newNode exists in the network
  //Post: If newNode is already not in data, 
  //      data[size] = newNode, queryStatus[size] = false, size = size + 1
  //      and return true, false otherwise
  bool add(Triple newNode);

  //Pre: contacted has been quereied
  //Post: data[index] = contacted, queryStatus[index] = true
  void updateStatus(Triple contacted);

  //Pre: 0 <= contacted < size
  //Post: queryStatus[index] = true
  void updateStatus(int contacted);

  //Pre: N/A
  //Post: RV = true if for all 0 <= index < size, queryStatus[index] = true
  //      false otherwise
  bool isAllQueried();
  
};

#endif
