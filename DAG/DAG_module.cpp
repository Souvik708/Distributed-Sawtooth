#include <pthread.h>

#include <atomic>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace std::this_thread;
using std::atomic;
using std::string;
using std::thread;
using std::vector;

const int maxTxns = 1200;

struct Transaction {
  string txnId;
  int txnNo;
  int inputLen;
  string inputs[10];
  int outputLen;
  string outputs[10];
  bool flag = false;
};

// Node structure for the linked list
struct addressNode {
  int addId;
  std::string address;
  std::vector<int> readList;
  std::vector<int> writeList;
  addressNode* next;
  addressNode(int nodeId, const std::string& addr)
      : addId(nodeId), address(addr), next(nullptr) {}
};

// Global head pointer for the linked list
addressNode* head = nullptr;

int totalTxns, addressId = 0, threadCount = 30;
int transactionAdj[maxTxns][maxTxns] = {0};
int verifyAdj[maxTxns][maxTxns] = {0};
atomic<int> txnCounter{0}, au{0}, svCounter{0}, barrier{-1}, position{0};
atomic<bool> minerFlag(false);
atomic<bool> mMiner(false);
atomic<int> inDegree[maxTxns];

vector<Transaction> currTxns, emptyTxns;

class Geek {
 public:
  int DAGSelect() {
    int i, positionLocal = position.load();
    int varZero = 0;
    // Check transactions from positionLocal to totalTxns
    for (i = positionLocal; i < totalTxns; i++) {
      // cout<<i<<" "<<"varZero:"<<varZero<<"    ";
      if (inDegree[i] == 0 &&
          inDegree[i].compare_exchange_strong(varZero, -1)) {
        position.store(i, std::memory_order_release);
        return i;
      }
    }

    // Check transactions from 0 to positionLocal
    for (int i = 0; i < positionLocal; i++) {
      if (inDegree[i] == 0 &&
          inDegree[i].compare_exchange_strong(varZero, -1)) {
        position.store(i, std::memory_order_release);  // Update position if a
                                                       // transaction is found
        return i;
      }
    }

    return -1;
  }

  static void addNodes(int PID) {
    int txnNo, batNo, inpLo, inpEx, outLo, outEx, miner = 0;
    int prevTxn, txn;
    while (true) {
      int edgeTxn;
      bool flag;
      txnNo = txnCounter++;
      if (txnNo >= totalTxns) {
        return;
      }
      inpLo = currTxns[txnNo].inputLen;
      outLo = currTxns[txnNo].outputLen;

      for (int i = 0; i < txnNo; i++) {
        flag = false;
        edgeTxn = currTxns[i].txnNo;

        if (inDegree[edgeTxn] != -2) {
          inpEx = currTxns[i].inputLen;
          outEx = currTxns[i].outputLen;
          for (int j = 0; j < inpLo; j++) {
            for (int k = 0; k < outEx; k++) {
              if (currTxns[txnNo].inputs[j] == currTxns[i].outputs[k]) {
                flag = true;
              }
            }
          }
          for (int j = 0; j < outLo; j++) {
            for (int k = 0; k < inpEx; k++) {
              if (currTxns[txnNo].outputs[j] == currTxns[i].inputs[k]) {
                flag = true;
              }
            }

            for (int k = 0; k < outEx; k++) {
              if (currTxns[txnNo].outputs[j] == currTxns[i].outputs[k]) {
                flag = true;
              }
            }
          }
          if (flag) {
            transactionAdj[edgeTxn][txnNo] = 1;
            inDegree[txnNo]++;
          }
        }
      }
    }
  }

  void DAGPrune() {
    for (int i = 0; i < totalTxns; i++) {
      fill_n(transactionAdj[i], totalTxns, 0);
      inDegree[i] = 0;
    }
    // Reset other variables
    txnCounter = 0;
    totalTxns = 0;
    currTxns.clear();
  }

  void DAGPrint() {
    // Print transactionAdj matrix
    for (int i = 0; i < 11; ++i) {
      for (int j = 0; j < 11; ++j) {
        std::cout << transactionAdj[i][j] << " ";
      }
      std::cout << std::endl;  // Newline for new row
    }

    std::cout << "-------------------------------------------" << std::endl;

    // Print inDegree array
    for (int j = 0; j < totalTxns; ++j) {
      std::cout << inDegree[j] << " ";
    }
  }

  void DAGDelete(int n) {
    // Decrement inDegree for each transaction that has an edge from n
    for (int i = 0; i < totalTxns; ++i) {
      if (transactionAdj[n][i] == 1 && inDegree[i] > 0) {
        --inDegree[i];
      }
    }
    // Decrement inDegree for the completed transaction
    --inDegree[n];
  }

  void read_adjacency_matrix() {
    std::ifstream file("DAG/adjMatrix.txt", ios::in);
    int num_vertices = 0;
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      int num;
      int col = 0;
      while (iss >> num) {
        transactionAdj[num_vertices][col++] = num;
        if (num == 1) {
          // Increment in-degree for the current column/vertex
          inDegree[col - 1]++;
        }
      }
      ++num_vertices;
    }

    file.close();
  }

  // Function to check conflicts between read and write operations
  static void DAGVerify(int PID) {
    int addNo = svCounter++;
    addressNode* node = find_id(addNo);
    if (node == nullptr) {
      return;
    }

    for (int writeOp : node->writeList) {
      for (int readOp : node->readList) {
        if (writeOp < readOp) {
          verifyAdj[writeOp][readOp] = 1;
          if (transactionAdj[writeOp][readOp] == 0) {
            mMiner.store(true);  // Conflict detected
            return;
          }
        }
        if (writeOp > readOp) {
          verifyAdj[readOp][writeOp] = 1;
          if (transactionAdj[readOp][writeOp] == 0) {
            mMiner.store(true);  // Conflict detected
            return;
          }
        }
      }
      for (int writeOp2 : node->writeList) {
        if (writeOp < writeOp2) {
          verifyAdj[writeOp][writeOp2] = 1;
          if (transactionAdj[writeOp][writeOp2] == 0) {
            mMiner.store(true);  // Conflict detected
            return;
          }
        }
        if (writeOp2 < writeOp) {
          verifyAdj[writeOp2][writeOp] = 1;
          if (transactionAdj[writeOp2][writeOp] == 0) {
            mMiner.store(true);  // Conflict detected
            return;
          }
        }
      }
    }

    return;  // No conflict
  }

  // Function to find a node by ID
  static addressNode* find_id(int id) {
    addressNode* current = head;
    while (current != nullptr) {
      if (current->addId == id) {
        return current;
      }
      current = current->next;
    }
    return nullptr;  // ID not found
  }

  // Function to find a node
  addressNode* find(const std::string& address) {
    addressNode* current = head;
    while (current != nullptr) {
      if (current->address == address) {
        return current;
      }
      current = current->next;
    }
    return nullptr;
  }

  addressNode* createAddress(const std::string& address) {
    addressNode* node = find(address);
    if (node) {
      // Address already exists, return pointer
      return node;
    } else {
      // Address doesn't exist, create new node with ID
      addressNode* newNode = new addressNode(++addressId, address);
      newNode->next = head;
      head = newNode;
      return newNode;
    }
  }

  // Function to add a read operation to the read list of a node
  void addReadOperation(addressNode* node, int value) {
    // Add value to read list
    node->readList.push_back(value);
  }

  // Function to add a write operation to the write list of a node
  void addWriteOperation(addressNode* node, int value) {
    // Add value to write list
    node->writeList.push_back(value);
  }

  bool secureValidator() {
    int i, n, j, txnCount = 0;
    addressNode* nodePtr;
    std::fstream batchFile;
    thread threads[threadCount];
    struct Transaction txn;
    batchFile.open("DAG/batch_for_DAG.txt", ios::in);
    if (batchFile.is_open()) {
      string transactionProperty;
      while (getline(batchFile, transactionProperty)) {
        getline(batchFile, transactionProperty);
        txn.inputLen = stoi(transactionProperty);
        for (i = 0; i < txn.inputLen; i++) {
          getline(batchFile, transactionProperty);
          nodePtr = createAddress(transactionProperty);
          addReadOperation(nodePtr, txnCount);
        }
        getline(batchFile, transactionProperty);
        txn.outputLen = stoi(transactionProperty);
        for (i = 0; i < txn.outputLen; i++) {
          getline(batchFile, transactionProperty);
          nodePtr = createAddress(transactionProperty);
          addWriteOperation(nodePtr, txnCount);
        }
        txnCount++;
      }
    }
    batchFile.close();
    totalTxns = txnCount;
    read_adjacency_matrix();

    for (i = 0; i < threadCount; i++) {
      threads[i] = thread(DAGVerify, i);
    }
    for (i = 0; i < threadCount; i++) {
      threads[i].join();
    }

    n = totalTxns;
    cout << "the miner is not:" << mMiner.load() << endl;
    // Calculate in-degree matrix from adjacency matrix
    vector<int> calculatedInDegree(n, 0);
    vector<int> sharedInDegree(n, 0);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (verifyAdj[i][j] == 1) {
          calculatedInDegree[j]++;
        }
        if (transactionAdj[i][j] == 1) {
          sharedInDegree[j]++;
        }
      }
    }

    // Compare calculated in-degree matrix with provided in-degree matrix
    for (int i = 0; i < n; ++i) {
      if (calculatedInDegree[i] != sharedInDegree[i]) {
        return true;
      }
    }
    return mMiner.load();
  }

  void DAGCreate() {
    int i, j, txnCount = 0;
    std::fstream batchFile;
    thread threads[threadCount];
    struct Transaction txn;
    batchFile.open("DAG/batch_for_DAG.txt", ios::in);
    if (batchFile.is_open()) {
      string transactionProperty;
      while (getline(batchFile, transactionProperty)) {
        txn.txnId = transactionProperty;
        txn.txnNo = txnCount++;
        getline(batchFile, transactionProperty);
        txn.inputLen = stoi(transactionProperty);
        for (i = 0; i < txn.inputLen; i++) {
          getline(batchFile, transactionProperty);
          txn.inputs[i] = transactionProperty;
        }
        getline(batchFile, transactionProperty);
        txn.outputLen = stoi(transactionProperty);
        for (i = 0; i < txn.outputLen; i++) {
          getline(batchFile, transactionProperty);
          txn.outputs[i] = transactionProperty;
        }
        currTxns.push_back(txn);
        cout << txn.txnNo << " ";
      }
    }
    batchFile.close();
    totalTxns = currTxns.size();
    // cout << totalTxns;
    // cout << "-------------------------------------------\n";

    for (i = 0; i < threadCount; i++) {
      threads[i] = thread(addNodes, i);
    }
    for (i = 0; i < threadCount; i++) {
      threads[i].join();
    }
  }
};
int main() {
  // Creating an object
  Geek t;
  // Calling function
  t.DAGCreate();
  t.DAGPrint();
  return 0;
}

extern "C" {

Geek* Geek_new() { return new Geek(); }
void DAGPrune(Geek* geek) { geek->DAGPrune(); }
bool SecureValidator(Geek* geek, bool flag) { geek->secureValidator(); }
void DAGCreate(Geek* geek) { geek->DAGCreate(); }
int DAGSelect(Geek* geek) { geek->DAGSelect(); }
void DAGPrint(Geek* geek) { geek->DAGPrint(); }
void DAGDelete(Geek* geek, int n) { geek->DAGDelete(n); }
}
