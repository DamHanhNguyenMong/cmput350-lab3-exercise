#ifndef SHARED_PTR_HEADER
#define SHARED_PTR_HEADER
#include <utility>
#include <cassert>

class ControlBlockBase {
public:
    // TODO: implement the default constructor.
    ControlBlockBase() : refcount(0) { //set refcount to 0 in the constructor
        
    }; 

    // dtor is virtual, so that we can call derived class's dtor from a ptr to this base class.
    virtual ~ControlBlockBase() = default; // TODO: implement the destructor.

    // pure virtual function; must be overriden by derived classes
    virtual void* managedAddress() = 0;

    // Delete copies, which also implicitly deletes moves.
    ControlBlockBase(const ControlBlockBase&) = delete;
    ControlBlockBase& operator=(const ControlBlockBase&) = delete;

    long increment()
    {
        // TODO: increment refcount by 1 and return result.
        assert(refcount >= 0); // cant be negative
        return ++refcount;
    }

    long decrement()
    {
        // TODO: decrement refcount by 1 and return result.
        assert(refcount > 0); //refcount must be larger than 0 to decrement
        return --refcount;
    }

    long refCount() const
    {
        // TODO: just return the refcount.
        return refcount;
    }

private:
    // TODO: add field(s) which both control block types need to have
    long refcount;

};

// Publicly derive from ControlBlockBase a new concrete class template ControlBlock<T>
template <typename T>
class ControlBlock : public ControlBlockBase {
public:
    
    // A constructor which takes a T* (pointer to resource) to manage
    ControlBlock(T* ptr) : managed_ptr(ptr) {
        
    }
    // A overriding destructor which deletes its managed pointer
    ~ControlBlock() override {
        delete managed_ptr;
    }

    // An overriding implementation of void* managedAddress() which returns the address of the managed resource.
    void* managedAddress() override {
        return managed_ptr;
    }
private:
    // store a (private) T*, pointing to the managed object.
    T* managed_ptr;
};

template <typename T>
class SharedPtr {
public:
    // A default constructor which sets both its stored pointer and its control block pointer to nullptr
    SharedPtr(): stored_ptr(nullptr), control_block(nullptr) {}

    // A constructor which takes a T* and both uses it as the “stored pointer”, and takes ownership of it in a new ControlBlock<T>
    SharedPtr(T* ptr): stored_ptr(ptr), control_block(new ControlBlock<T>(ptr)) {
        control_block->increment();
    }

    // A destructor: decrease the refcount of its control block if exists and delete the control block if the refcount reaches 0
    ~SharedPtr() {
        if (control_block) {
            if (control_block->decrement() == 0) {
                delete control_block;
            }
        }
    }

    //copy constructor: shares the same “stored pointer” and the same control block as the original , increase the refcount by 1 (if it exists)
    SharedPtr(const SharedPtr& other) : stored_ptr(other.stored_ptr), control_block(other.control_block) {
        if (control_block) {
            control_block->increment();
        }
    }

    // move operator: transfers ownership of the “stored pointer” and the control
    SharedPtr(SharedPtr&& other) : stored_ptr(other.stored_ptr), control_block(other.control_block) {
        other.stored_ptr = nullptr;
        other.control_block = nullptr;
    }

    // Copy assignment operator: If copied into an existing SharedPtr<T>, destruction semantics should be invoked on the previously managed resource
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) { //self-assignment check
            //Decrement the refcount of the current control block and delete it if neccesary
            if (control_block && control_block->decrement() == 0) {
                delete control_block;
            }
            //Copy the stored pointer and control block from the other SharedPtr
            stored_ptr = other.stored_ptr;
            control_block = other.control_block;
            //Increment the refcount of the new control block if it exists
            if (control_block) {
                control_block->increment();
            }
        }
        return *this;   
    }

    // Move assignment operator: If moved into an existing SharedPtr<T>, destruction semantics should be invoked on the previously managed resource
    SharedPtr& operator=(SharedPtr&& other) {
        if (this != &other) { //self-assignment check
            //Decrement the refcount of the current control block and delete it if necessary
            if (control_block && control_block->decrement() == 0) {
                delete control_block;
            }
            //Transfer ownership of the stored pointer and control block from the other SharedPtr
            stored_ptr = other.stored_ptr;
            control_block = other.control_block;
            //Set the other SharedPtr to nullptr
            other.stored_ptr = nullptr;
            other.control_block = nullptr;
        }
        return *this;
    }

    // A dereference operator T& operator*(), which returns a reference to the stored object, and allows you to dereference the stored pointer
    T& operator*() const {
        assert(stored_ptr != nullptr);
        return *stored_ptr; 
    }

    // An arrow operator T* operator->(). returns the stored pointer itself.
    T* operator->() const {
        assert(stored_ptr != nullptr);
        return stored_ptr;
    }

    // T* get(), which just returns the stored pointer
    T* get() const {
        return stored_ptr;
    }

    // A equality comparison operator compares the stored pointers
    bool operator==(const SharedPtr<T>& other) const {
        return stored_ptr == other.stored_ptr;
    }

    // A boolean conversion operator, which returns true iff the stored pointer is not nullptr
    operator bool() const { //const because we don't want to modify the object
        return stored_ptr != nullptr; //return true if the unique pointer is not empty, false
    } 

    // swap: both the stored pointer and the managed control block pointer with other. Swap does not change refCount
    void swap(SharedPtr<T>& other) {
        //swap the stored pointer
        T* temp_ptr = stored_ptr;
        stored_ptr = other.stored_ptr;
        other.stored_ptr = temp_ptr;

        // swap the control block
        ControlBlockBase* temp_control = control_block;
        control_block = other.control_block;
        other.control_block = temp_control; 
    }

    // reset: release ownership of the currently managed object
    void reset() {
        SharedPtr<T> temp;
        swap(temp); //its destructor decrement the refcount when goes out of scope 
    }
    
    // reset(T* other): release current object and begin managing other
    void reset(T* other) {
        if (other == stored_ptr) { // check self-reset
            return;
        }
        SharedPtr<T> temp(other);
        swap(temp);
    }

    // return the refcount of the currently managed object
    long useCount() const {
        if (control_block) {
            return control_block->refCount();
        }
        return 0;
    }

private:
    // Two private pointer fields, one for the “stored pointer”, and one pointing to ControlBlockBase (NOTControlBlock<T>)
    T* stored_ptr; // the stored pointer
    ControlBlockBase* control_block; // pointer to the control block
};

// Creating a Shared Pointer
template <typename T, typename... Args>
SharedPtr<T> makeSharedBasic(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif
