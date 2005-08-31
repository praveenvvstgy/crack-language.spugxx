
#file MsgString

#intinclude MsgBase
#intinclude String

class MsgString : public MsgBase, public String {

	public:
		/*---------------------------------------------------------------------*\
			Constructs an empty message string.
		\*---------------------------------------------------------------------*/
		MsgString()
			{
			}
		
		/*---------------------------------------------------------------------*\
			Constructs a message string from a null terminated character
			string.
		\*---------------------------------------------------------------------*/
		MsgString(const char *val) :
				String(val)
			{
			}
	
		
		/*---------------------------------------------------------------------*\
			Returns the type id (MsgBase::string)
		\*---------------------------------------------------------------------*/
		virtual MsgBase::TypeId type() const
			{
			return string;
			}

		/*---------------------------------------------------------------------*\
			Writes the string out to a nicely indented ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out, unsigned indent) const 
			{
			doIndent(out, indent);
			out << '"';
			for (const char *c = *this; *c; c++)
				switch (*c)
					{
					case '"':
						out << "\\\"";
						break;
						
					case '\\':
						out << "\\\\";
						break;
						
					default:
						out << *c;
						break;
					}
			out << '"' << endl;
			return out;
			}
		
		/*---------------------------------------------------------------------*\
			Writes the message object out to an unformatted ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out) const
			{
			out << '"';
			for (const char *c = *this; *c; c++)
				switch (*c)
					{
					case '"':
						out << "\\\"";
						break;
						
					case '\\':
						out << "\\\\";
						break;
						
					default:
						out << c;
						break;
					}
			out << '"';
			return out;
			}
	
	}
	