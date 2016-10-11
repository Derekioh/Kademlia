//Justin Giguere
//Routing Table cc file

#include "KBucket.h"
#include "constants.h"
#include "RoutingTable.h"
#include <stdint.h>
#include <iostream>

using namespace std;

//Pre: id refers to a valid node object
//Post: meBuckets is an array of K kBuckets, that are each empty
RoutingTable(int id) {
  myId = id;
  meBuckets = KBucket[K];
}

//Pre: The respected node is going poof
//Post: deletes each kBucket
~RoutingTable() {
  for (int index = 0; (index < K); index++) {
    delete meBuckets[index];
  }
}
  
//Pre: id is some valid node or key
//Post: RV = the nth kBucket such that d = findDist(id) where 2^n <= d < 2^n+1
int RoutingTable::findKBucket(uint32_t id) {
  int myDist = findDist(myId, id);
  int tempt = myDist;
  int twoPower = 0;
  while (tempt % 2 == 0) {
    tempt = tempt / 2;
    twoPower++;
  }
  return (twoPower);
}

//Pre: target is some id, closeNodes is an array of K -1's
//Post: RV = array of the K closest nodes to the target
//      Less than K are returned iff less than K nodes are in the table
//      The list is ordered by distance, that is, the closest node is
//      at the head
void RoutingTable::getNodes(uint32_t target, Triple* closeNodes) {
  int nthBucket = findKBucket(target);
  int nextBucket = nthBucket + 1;
  int lastBucket = nthBucket - 1;
  int sizeClose = 0;
  meBuckets[nthBucket].getNodes(target, closeNodes, sizeClose);
  if (nextBucket < K) {
    meBuckets[nextBucket].getNodes(target, closeNodes, sizeClose);
  }
  if (lastBucket > -1) {
    meBuckets[lastBucket].getNodes(target, closeNodes, sizeClose);
  }
  while (sizeClose < K) and ((nextBucket < K) or (lastBucket > -1))) {
    nextBucket++;
    lastBucket--;
    if (nextBucket < K) {
      meBuckets[nextBucket].getNodes(target, closeNodes, sizeClose);
    } 
    if (lastBucket > -1) {
      meBuckets[lastBucket].getNodes(target, closeNodes, sizeClose);
    }     
  }
}

//Pre: id and address represents a new node not seen before
//Post: RV = Triple object representing this new node
Triple* RoutingTable::createTriple(uint32_t id, uint32_t address) {
  Triple* newTriple = new Triple;
  newTriple.address = address;
  newTriple.nodeID = id;
  newTriple.port = UDPPORT;
  return (newTriple);
}

//Pre: node is a valid node object not in the routing table (id)
//     address correlates to where node is from
//Post: RV = true if node was added to the table
//           false otherwise
bool RoutingTable::addNode(uint32_t node, uint32_t address) {
  int myKBucket = findKBucket(node);
  Triple* currTriple = myKBucket[0];
  bool added = false;
  Triple* newTriple = createTriple(node, address);
  if (myKBucket.numTriples == K) {  //the bucket is full
    bool dead = ping(curTriple.address);
    if (dead) {
      //the least recently seen node is dead
      myKBucket.deleteNode(currTriple);
      myKBucket.addNode(newTriple);
    }
  }
  else {
    myKBucket.addNode(newTriple);
    added = true;
  }
  return (added);
}
  
//Pre: myNode exists inside meBuckets
//Post: myNode is placed at the tail of its respected kBucket
//      the other triples are left shifted as needed
void RoutingTable::updateTable(Triple* myNode) {
  uint32_t nodeID = myNode->nodeID;
  int myKBucket = findKBucket(nodeID);
  meBuckets[myKBucket].adjustNode(myNode);
}
