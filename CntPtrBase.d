
#file CntPtrBase

#impinclude CntObj

class CntObj;

/*--------------------------------------------------------------------------*\
   The base class for the CntPtrBase template.  This class holds all
   of the "meat" of the managed object pointer, allowing CntPtrBase
   methods to be completely inlined without bloating the generated code.
\*--------------------------------------------------------------------------*/
class CntPtrBase {
   protected:
      CntObj *obj;

   public:
      /*--------------------------------------------------------------------*\
         Constructs a managed object pointer initialized to NULL.
      \*--------------------------------------------------------------------*/
      CntPtrBase() : obj(0)
			{
			}

      /*--------------------------------------------------------------------*\
         Constructs a managed object pointer from a pointer to a
         *CntObj*
      \*--------------------------------------------------------------------*/
      CntPtrBase(CntObj *obj0) : obj(0)
			{
			assign(obj0);
			}

      /*--------------------------------------------------------------------*\
         Destructs a managed object pointer and unlinks the associated
         *CntObj* (if any).
      \*--------------------------------------------------------------------*/
      ~CntPtrBase()
		   {
		   if (obj)
		      obj->unlink();
		   }

      /*--------------------------------------------------------------------*\
         Links the managed object pointer to a *CntObj.*  A value
         of NULL clears the managed object pointer.  If the pointer
         is already linked to an object, that object is first unlinked.
      \*--------------------------------------------------------------------*/
      void assign(CntObj *obj0)
		   {
		   if (obj)
		      obj->unlink();

		   obj = obj0;

		   if (obj)
		      obj->link();
		   }
   }
