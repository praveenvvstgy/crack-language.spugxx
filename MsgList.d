
#file MsgList

#intinclude MsgBase
#intinclude PtrSList

#impinclude iostream

class MsgList : public MsgBase, public PtrSList<MsgBase> {
	public:
		/*---------------------------------------------------------------------*\
			Returns the type id (MsgBase::list)
		\*---------------------------------------------------------------------*/
		virtual MsgBase::TypeId type() const
			{
			return list;
			}

		/*---------------------------------------------------------------------*\
			Writes the list out to a nicely indented ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out, unsigned indent) const 
			{
			doIndent(out, indent);
			out << '(' << endl;
			for (Pix p = first(); p; next(p))
				{
				TypeId childType = (*this)(p)->type();
				if (childType == list || childType == keySet)
					indent += tabIncrement;
				(*this)(p)->putOn(out, indent);
				if (childType == list || childType == keySet)
					indent -= tabIncrement;
				}
			doIndent(out, indent);
			out << ')' << endl;
			return out;
			}
		
		/*---------------------------------------------------------------------*\
			Writes the message object out to an unformatted ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out) const
			{
			out << '(';
			for (Pix p = first(); p; next(p))
				(*this)(p)->putOn(out);
			out << ')';
			return out;
			}

	}
