
#file PSLBase

#intinclude Pix

class PtrSListBase {

   private:
      unsigned count;

#<#ifdef __IBMCPP__
	// compensate for a bug in the IBM compiler which doesn't do protected
	// properly for templates.
	public:
#<#else
   protected:
#<#endif

      class Node {
         public:
            Node *next;
            Node(Node *next0 = 0) : next(next0)
					{
					}
         }

	protected:

		Node *head, *tail;
         
      PtrSListBase() : count(0), head(0), tail(0)
         {
         }
         
      Pix append(Node *node)
         {
         if (tail)
            {
            tail->next = node;
            tail = node;
            }
         else
            {
            head = tail = node;
            }
         count++;
         return tail;
         }

      Pix prepend(Node *node)
         {
         if (head)
            {
            node->next = head;
            head = node;
            }
         else
            {
            head = tail = node;
            }
         count++;
         return head;
         }

      void clear(void (*deletor)(Node *node))
         {
			Pix q;
         for (Pix p = first(); p;)
				{
				q = p;
				next(p);
            deletor((Node*)q);
				}
			head = tail = 0;
         count = 0;
         }

		/*---------------------------------------------------------------------*\
			Removes the node pointed to by the Pix p, returns a Pix to the node
			immediately following it or null.
		\*---------------------------------------------------------------------*/
      Pix remove(Pix p, void (*deletor)(Node *node))
         {
			Pix following;
         if ((Node*)p == head && (Node*)p == tail)
            following = head = tail = 0;
         else
            {
            if (p == head)
               following = head = head->next;
         
            else
               {
               // find the node before 'p'
               Pix q;
               for (q = first(); ((Node*)q)->next != p; next(q)) ;
               following = ((Node*)q)->next = ((Node*)p)->next;
               
               if (p == tail)
                  tail = (Node*)q;
               }
            }
         deletor((Node*)p);
         count--;
			return following;
         }
         
      void ripOff(PtrSListBase &other)
         {
         head = other.head;
         tail = other.tail;
         other.head = 0;
         other.tail = 0;
         }

   public:
      Pix first() const
         {
         return head;
         }
         
      Pix last() const
         {
         return tail;
         }
         
      Pix next(Pix &p) const
         {
         return p = ((Node*)p)->next;
         }
         
      unsigned length() const
         {
         return count;
         }
         
   }
