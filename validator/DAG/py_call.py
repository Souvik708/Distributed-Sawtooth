
from ctypes import cdll
from ctypes import c_int
import ctypes

# load the library
lib = cdll.LoadLibrary('./libgeek.so')



# create a Geek class
class Geek(object):
  
    # constructor
    def __init__(self):
  
        # attribute
        self.obj = lib.Geek_new()
  

    # define method
    def DAG_prune(self):
        lib.DAGPrune(self.obj)

    # define method
    def DAG_create(self):
        lib.DAGCreate(self.obj)

    # define method
    def DAG_create2(self):
        lib.DAGPrint(self.obj)

    # define method
    def DAG_select(self):
        return lib.DAGSelect(self.obj)

     # define method
    def DAG_secureValidator(self):
        return lib.SecureValidator(self.obj)
	

    # define method
    def DAG_delete(self, int):
        lib.DAGDelete(self.obj, int )
  
# create a Geek class object
f = Geek()
f.DAG_create()
f.DAG_create2()
x=f.DAG_select()
print("----------------------------------\n")
print(x)
x=f.DAG_select()
print(x)
x=f.DAG_select()
print(x)
f.DAG_delete(x)

x=f.DAG_select()
print(x)
f.DAG_delete(x)

x=f.DAG_select()
print(x)
f.DAG_delete(x)
#print("The miner is not:")
print(f.DAG_secureValidator())
f.DAG_prune()
