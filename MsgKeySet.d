
#file MsgKeySet

#intinclude MsgBase
#intinclude MsgId
#intinclude Ptrs

#impinclude stdlib

class MsgKeySet : public MsgBase {
   private:
		class Assoc {
			public:
				ObjPtr<MsgId> key;
				ObjPtr<MsgBase> val;

				/*---------------------------------------------------------------*\
					Transfers ownership of key and val to 'other'
				\*---------------------------------------------------------------*/
				void transfer(Assoc &other)
					{
					other.key = key.release();
					other.val = val.release();
					}
			}
			
			
		Assoc *hash;
      unsigned hashCount, hashSize;
      static const unsigned hashStartSize;

      static unsigned hashVal(const char *key, unsigned size)
         {
			unsigned val = 0;
			int len = strlen(key);
			for (int i = 0;  i < len; i++)
				val ^= val << 3 ^ key[i];
			return val % size;
         }
         
      static bool _set(Assoc *hash, unsigned size,
                       MsgId *key, MsgBase *val)
         {
         bool incrementCount;
         unsigned index = hashVal(*key, size);
         while (hash[index].key && *hash[index].key != *key)
            ++index %= size;
			hash[index].key = key;
         hash[index].val = val;
         return incrementCount;
         }

		/*---------------------------------------------------------------------*\
			Deletes the internal hash array.   Exists mainly to circumvent
			a G++ 2.7.2 compiler bug which prohibits allocating and deleting
			arrays in the same scope.
		\*---------------------------------------------------------------------*/
		void deleteHash()
			{
			delete [] hash;
			}
		
		/*---------------------------------------------------------------------*\
			allocates a new hash.
		\*---------------------------------------------------------------------*/
		MsgKeySet::Assoc *newHash(int size)
			{
			return new Assoc[size];
			}
			
   public:

      MsgKeySet() :
            	 hash(0),
            	 hashCount(0),
            	 hashSize(0)
         {
         }

      MsgKeySet(const MsgKeySet &other);
//		   {
//		   unsigned i;
//         
//		   // copy 'other's hash
//		   for (i = 0; i < hashSize; i++)
//		      if (other.hash[i].key) {
//		         hash[i] = other.hash[i];
//      			hash[i].val->link();
//			      }
//
//			}
                  
      void set(MsgId *key, MsgBase *val)
         {
         // if there's no hash, build one
         if (!hashSize)
            {
            hash = newHash(hashStartSize);
            hashSize = hashStartSize;
            }
            
         // if the hash >= 3/4 full, regrow it
         else if (hashCount >= 3 * hashSize / 4) {
            Assoc *hash2 = newHash(hashSize * 2);
            for (int i = 0; i < hashSize; i++)
					if (hash[i].key)
	               _set(hash2, hashSize * 2,
							  hash[i].key.release(), hash[i].val.release());
				deleteHash();
            hash = hash2;
            hashSize *= 2;
            }
            
         _set(hash, hashSize, key, val);
         }
         
      MsgBase *get(const char *key)
         {
         if (hash) {
            unsigned index = hashVal(key, hashSize);
            while (hash[index].key && *hash[index].key != key)
               ++index %= hashSize;
            return hash[index].val;
            }
         else
            return 0;
         }         
         
      ~MsgKeySet()
			{
//		   unsigned i;
//		   for (i = 0; i < hashSize; i++)
//      		if (hash[i])
//		         hash[i].val->unlink();
   
		   deleteHash();
			}	

		/*---------------------------------------------------------------------*\
			Returns the type id (MsgBase::keySet)
		\*---------------------------------------------------------------------*/
		virtual MsgBase::TypeId type() const
			{
			return keySet;
			}

		/*---------------------------------------------------------------------*\
			Writes the keyset out to a nicely indented ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out, unsigned indent) const 
			{
			doIndent(out, indent);
			out << '{' << endl;
			for (unsigned i = 0; i < hashSize; i++)
				{
				if (hash[i].key)
					{
					doIndent(out, indent);
					out << *hash[i].key << ':' << endl;
					indent += tabIncrement;
					hash[i].val->putOn(out, indent);
					indent -= tabIncrement;
					}
				}
			doIndent(out, indent);
			out << '}' << endl;
			return out;
			}
		
		/*---------------------------------------------------------------------*\
			Writes the message object out to an unformatted ostream.
		\*---------------------------------------------------------------------*/
		virtual ostream &putOn(ostream &out) const
			{
			out << '{';
			for (unsigned i = 0; i < hashSize; i++)
				{
				if (hash[i].key)
					{
					out << *hash[i].key << ": " << endl;
					hash[i].val->putOn(out);
					}
				}
			out << '}';
			return out;
			}
		

      }


#>const unsigned MsgKeySet::hashStartSize = 4;
