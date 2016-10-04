#include <Vector>
#include "RoutingTable.h"

#define K 5
#define ALPHA 3
#define INT_SIZE 32
#define TIME_TO_RESPOND 2000	//in milliseconds, the time to wait before moving on for an operation (such as findNode)
#define TIME_TO_PING 3600000	//in milliseconds, the time to wait before pinging a node

class Node{

 private:
  uint32_t ID;						//The ID of this computer
  vector<uint32_t> keys;				//The keys of the files stored on this pc
  RoutingTable routingTable;	//The K-Buckets

  uint32_t getID();
  void setID(uint32_t ID);

  char * getIP();
  char * getPort();

  //PRE: the node we want to ping
  //POST: checks to make sure that node is still alive. Removes from the k-bucket 
  //      if dead node
  void ping(uint32_t nodeID);

  //PRE: the key that represents our file
  //POST: Finds the distance (ID XOR key) and sends the store command to the k closest nodes 
  void findCandidatesForStore(uint32_t key);

  //PRE: the key that represents our file
  //POST: adds the key to our list of keys
  void store(uint32_t key);

  //PRE: the node ID we are looking for in our network
  //POST: Starts by picking ALPHA nodes from the closest non-empty bucket. Then call findNode
  //      on each of those nodes asyncronously. Nodes that fail to respond quickly (TIME_TO_RESPOND)
  //      are removed from consideration until and unless they do respond. If these nodes fail to return
  //      nodes that are closer than the closest we have already seen, resend findNode to another ALPHA number
  //      of nodes from our list up until k times.
  void nodeLookup(uint32_t nodeID);

  //PRE: the key of the file we want to lookup
  //POST: acts the same as the nodeLookup, but now we return true if we can find the key, false otherwise.
  bool valueLookup(uint32_t keyID);

  //PRE: a node ID we want to find in the network
  //POST: returns up to k nodes that are closest to the requested nodeID.
  //NOTE: if a node fails to respond, they will get removed for the coorsponding bucket
  //      until and unless they respond. additionally, the node we are looking for is
  //      updated in the appropriate bucket for this node so we can find it later
  //      (including adding to our bucket if it does not exist).
  //NOTE: by return we mean sending a message
  void findNode(uint32_t nodeID);

  //PRE: a key we want to find in the network
  //POST: acts the same as findNode, but now we return true if we have found a value
  //NOTE: by return we mean sending a message
  void findValue(uint32_t key);

 public:
  //PRE: 
  //POST: lets create a new network and init this node with 32 empty k buckets and a random id
  Node();

  //PRE: the contact node we know about that will allow us to join the network
  //POST: create a random id that is unique in this network, create our 32 k buckets that
  //      correspond to the network
  Node(Triple contact);

  //---------------------------------------------------------------------------------
  //            THREAD FUNCTIONS
  //---------------------------------------------------------------------------------

  //PRE:
  //POST: parent thread that spawns other threads as needed
  void main_T();

  //PRE: 
  //POST: the thread that handles pinging every node in our k buckets every TIME_TO_PING amount of time
  void refresher_T();

  //PRE: 
  //POST: responds to other nodes asking for things like findNode and findValue. This thread
  //      will spawn sendMessage and recieveMessage threads
  void responder_T();

  //PRE: 
  //POST: responds to messages thread
  void sendMessage_T();

  //PRE: 
  //POST: recieves messages thread
  void receiveMessage_T();

  //PRE: 
  //POST: handles the command line prompt and ui thread
  void UI_T();
}
