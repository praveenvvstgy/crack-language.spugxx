
#file Exception

#intinclude String
#intinclude Ptrs

/*---------------------------------------------------------------------------*\
	Base class for all exceptions.  Can be used stand-alone to throw
	string-described exceptions.
\*---------------------------------------------------------------------------*/
class Exception {

	protected:
		// user can  optionally define the exception as just a string.
		ObjPtr<String> str;

	public:
		/*---------------------------------------------------------------------*\
			Default constructor for exception.
		\*---------------------------------------------------------------------*/
		Exception()
			{
			}
		
		/*---------------------------------------------------------------------*\
			Copy constructor.
		\*---------------------------------------------------------------------*/
		Exception(const Exception &other) :
				str(other.str)
			{
			}
		
		/*---------------------------------------------------------------------*\
			Construct an exception from a string.
		\*---------------------------------------------------------------------*/
		Exception(const char *str0) :
				str(new String(str0))
			{
			}
	
		/*---------------------------------------------------------------------*\
			Return the string value of an exception.
		\*---------------------------------------------------------------------*/
		virtual String asString() const
			{
			if (str)
				return *str;
			else
				return "Exception";
			}

		/*---------------------------------------------------------------------*\
			Primitive RTTI.
		\*---------------------------------------------------------------------*/
		virtual String className() const
			{
			return "Exception";
			}

	};
