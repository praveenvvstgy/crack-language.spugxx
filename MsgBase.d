
#file MsgBase

class istream;
class ostream;
class MsgNumber;
class MsgList;
class MsgId;
class MsgKeySet;
class MsgString;

#impinclude iostream
#impinclude MsgList
#impinclude MsgNumber
#impinclude MsgId
#impinclude MsgKeySet
#impinclude MsgString
#impinclude Exception
#impinclude ctype

class MsgBase {

	private:
		static MsgNumber *parseNumber(istream &src, char c)
			{
			unsigned val = 0;
			while (src)
				{
				val = val * 10 + c - '0';
				src >> c;
				if (!isdigit(c))
					{
					src.putback(c);
					break;
					}
				}
			
			return new MsgNumber(val);
			}
			
		static MsgList *parseList(istream &src)
			{
			ObjPtr<MsgList> list(new MsgList());
			while (src)
				{
				char c = ' ';
				while (src && isspace(c))
					src >> c;
				
				// find balance for '('
				if (c == ')')
					return list.release();
				
				src.putback(c);
				
				// try to parse the next object
				ObjPtr<MsgBase> obj(parse(src));
				list->append(obj.release());
				}
				
			// stream ended before we finished parsing list
			throw Exception("Premature end of stream, ')' expected.");
			}
		
		static MsgId *parseId(istream &src, char c)
			{
			ObjPtr<MsgId> ident(new MsgId());
			while (src)
				{
				*ident += c;
				src >> c;
				if (isspace(c) || c == '{' || c == '}' || c == ')' || c == '(' ||
						c == ':' || c == '"'
					 )
					{
					src.putback(c);
					return ident.release();
					}
				}
			
			return ident.release();
			}

		static MsgKeySet *parseKeySet(istream  &src)
			{
			ObjPtr<MsgKeySet> set(new MsgKeySet());
			while (src)
				{
				char c = ' ';
				while (src && isspace(c))
					src >> c;
				
				// find balance for '{'
				if (c == '}')
					return set.release();
				
				src.putback(c);
				
				// try to parse the key
				ObjPtr<MsgBase> key(parse(src));
				if (key->type() != id)
					throw Exception("Identifier expected in KeySet.");
					
				// make sure there's a colon
				do
					src >> c;
				while (src && isspace(c));
					
				if (c != ':')
					throw Exception("Identifier must be followed by a colon in KeySet.");
					
				// try to parse the value
				ObjPtr<MsgBase> val(parse(src));
				set->set((MsgId*)key.release(), val.release());
				}
				
			// stream ended before we finished parsing list
			throw Exception("Premature end of stream, ')' expected.");
			}

		static MsgString *parseString(istream &src)
			{
			char c;
			ObjPtr<MsgString> str(new MsgString());
			
			// read from the source stream until we come to the end of the
			// stream or a double quote.
			do
				{
				src >> c;
				if (src && c != '"')
					//  check for the escape character (\), if it is found
					// always add the next character to the string
					if (c == '\\')
						{
						src >> c;
						if (src)
							*str += c;
						
						// clear 'c' so we don't terminate loop due to a '"'
						c = 0;
						}
					else
						*str += c;
				}
			while (src && c != '"');
			
			// if we came to the end of the stream, report a problem
			if (!src)
				throw Exception("Premature end of stream, '\"' expected.");
			else
				return str.release();
			}

	protected:
	
		// 3 character tabs
		enum { tabIncrement = 3 };

		/*---------------------------------------------------------------------*\
			Streams out enough spaces to indent to the specified level.
		\*---------------------------------------------------------------------*/
		static void doIndent(ostream &out, unsigned indent)
			{
			while (indent--)
				out << ' ';
			}

	public:

		enum TypeId { number, list, string, id, keySet };

		/*---------------------------------------------------------------------*\
			Makes sure everything is cleaned up properly.
		\*---------------------------------------------------------------------*/
		~MsgBase()
			{
			}

		/*---------------------------------------------------------------------*\
			Po man's RTTI
		\*---------------------------------------------------------------------*/
		virtual TypeId type() const = 0;

		/*---------------------------------------------------------------------*\
			Writes the message object out to a nicely indented ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out, unsigned indent) const = 0;
		
		/*---------------------------------------------------------------------*\
			Writes the message object out to an unformatted ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out) const = 0;

		static MsgBase * parse(istream &src)
			{
			char c;
			src.unsetf(ios::skipws);
			if (!src)
				return 0;
			else
				{
				c = ' ';
				while (src && isspace(c))
					src >> c;

				if (isdigit(c))
					return parseNumber(src, c);
				else if (c == '(')
					return parseList(src);
				else if (c == '{')
					return parseKeySet(src);
				else if (c == '"')
					return parseString(src);
//				else if (c == '#')
//					return parseBlob(src);
				else if (c == ':')
					throw Exception("You may not begin a token with a colon.");
				else
					return parseId(src, c);
				}
			}

	}

