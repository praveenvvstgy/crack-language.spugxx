

#file MsgNumber

#intinclude MsgBase

class ostream;

#impinclude iostream

class MsgNumber : public MsgBase	{
	
	private:
		unsigned _val;
		
	public:
	
		MsgNumber(unsigned val0) : _val(val0)
			{
			}
		
		virtual MsgBase::TypeId type() const 
			{
			return number;
			}
			
		virtual ostream &putOn(ostream &out, unsigned indent) const
			{
			doIndent(out,  indent);
			out << _val << endl;
			return out;
			}
		
		virtual ostream &putOn(ostream &out) const
			{
			out << _val << ' ';
			return out;
			}
			
		unsigned val() const
			{
			return _val;
			}
	}