
#file CntObj

#impinclude Exception

class CntPtrBase;

/*--------------------------------------------------------------------------*\
   CntObj is that base class from which all classes that wish to
   employ this reference-counting scheme must be derived.  It basically
   contains the reference count variable and provides public methods with
   which instances of *ManagedObjectPtr* can *link* to and *unlink* from the
   object.
      *Safety*Note:* reference counting mechanisms fail when used in
   cyclic data structures.  For example, if object A has a reference counted
   pointer to object B, and object B has (directly or indirectly through
   any intermediate chain of objects) a reference counter pointer to A,
   the entire loop A -> B -> ... -> A will /never/be/deleted./  In situations
   where such constructs are neccessary, at least one non-reference counted
   pointer must be used somewhere in the chain.
\*--------------------------------------------------------------------------*/
class CntObj {

   private:
      unsigned refCount;

	protected:

		/*---------------------------------------------------------------------*\
			"delete" operator is defined as private so that a compiler error
			will  be generated if someone other than CntObj::unlink() attempts
			to delete the reference counted object.
		\*---------------------------------------------------------------------*/
		void operator delete(void *p)
			{
			::delete(p);
			}

   public:
      /*--------------------------------------------------------------------*\
         Constructs a managed object.  Initializes refCount.
      \*--------------------------------------------------------------------*/
      CntObj() : refCount(0)
			{
			}

      /*--------------------------------------------------------------------*\
         virtual destructor: used to insure proper deletion of derived
         classes by *unlink()*
      \*--------------------------------------------------------------------*/
      virtual ~CntObj()
		   {
		   if (refCount)
		      throw Exception("Destructor invoked on a CntObj with a non-zero"
									 "reference count.");
		   }

      /*--------------------------------------------------------------------*\
         Creates a "link" to the receiver.  Increments the reference count.
      \*--------------------------------------------------------------------*/
      void link()
		   {
		   refCount++;
		   }

      /*--------------------------------------------------------------------*\
         Removes a "link" from the receiver.  Decrements the reference
         count.  If the reference count has dropped to 0, deletes the
         receiver.
      \*--------------------------------------------------------------------*/
      void unlink()
		   {
			if (!refCount)
				throw Exception("Attempted to unlink a CntObj with a reference "
									 "count of zero.");
		   if (!--refCount)
      		delete this;
		   }
   }
