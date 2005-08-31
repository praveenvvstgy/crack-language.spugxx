
#file MsgId

#intinclude MsgBase
#intinclude String

#impinclude iostream

class MsgId : public MsgBase, public String {
	
	public:
		
		/*---------------------------------------------------------------------*\
			Returns the type id (MsgBase::id)
		\*---------------------------------------------------------------------*/
		virtual MsgBase::TypeId type() const
			{
			return id;
			}

		/*---------------------------------------------------------------------*\
			Writes the list out to a nicely indented ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out, unsigned indent) const 
			{
			doIndent(out, indent);
			out << *(String*)this << endl;
			return out;
			}
		
		/*---------------------------------------------------------------------*\
			Writes the message object out to an unformatted ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out) const
			{
			out << *(String*)this << ' ';
			return out;
			}

	}

		