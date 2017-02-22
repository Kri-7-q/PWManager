#ifndef SORTLIST_H
#define SORTLIST_H


template<class T> class SortList;
template<class T> class SortListIterator;


/**
 * ------------------------------------------------------------------------------
 * Template class:      ListNode
 * ------------------------------------------------------------------------------
 * A node for the linked list which sorts the inserted elments. The ListNode
 * class takes any data type. But the data object must handle the operators <, ==
 * and >. These operators are used for sorting.
 */
template<class T>
class ListNode
{
    friend class SortList<T>;
    friend class SortListIterator<T>;

public:
    /**
     * Constructs a ListNode object with a data object. These ListNode object must
     * be linked in the list afterwarts. Pointer to previous and next node are null.
     * @param data          The data object for this list node.
     */
    ListNode(const T& data) :
        m_data(data),
        m_previous(0),
        m_next(0)
    {

    }

    /**
     * Constructs a ListNode object and intializes it. The data object is set. The
     * object is linked with pointers to previous and next object.
     * @param prev          Pointer to a previous list node.
     * @param data          The data object of this node.
     * @param next          Pointer to the following list node.
     */
    ListNode(ListNode<T>* prev, const T& data, ListNode<T>* next) :
        m_previous(prev),
        m_data(data),
        m_next(next)
    {

    }

protected:
    ListNode<T>* m_previous;
    T m_data;
    ListNode<T>* m_next;
};


/**
 * -------------------------------------------------------------------------------------
 * Template class:          SortList
 * -------------------------------------------------------------------------------------
 * A list which sortes the inserted elements. Each element is inserted at the position
 * where is should take place to make the list soted. The list elements are sorted
 * ascending. To get the elements descending get a iterator with end(). Iterate backwards
 * through the list.
 */
template<class T>
class SortList
{
public:

    enum NodeCompareResult { Lesser = -1, Equal = 0, Greater = 1 };
    enum EqualBehaviour { UniteNodeData, InsertDoublicate, DropDoublicate };


    /**
     * Constructs a SortList with standard behaviour.
     */
    SortList() :
        m_pHead(0),
        m_pTail(0),
        m_size(0),
        m_equalBehaviour(InsertDoublicate)
    {

    }

    /**
     * Constructs a SortList with a given behaviour.
     * @param behaviour     Behaviour when a duplicate was found (equal object).
     */
    SortList(EqualBehaviour behaviour) :
        m_pHead(0),
        m_pTail(0),
        m_size(0),
        m_equalBehaviour(behaviour)
    {

    }

    /**
     * Destructor of SortList.
     * Deletes all nodes from the heap memory.
     */
    ~SortList()
    {
        clear();
    }

    void clear();
    int insert(const T& data);
    bool isEmpty() const                                { return m_pHead == 0; }
    int size() const                                    { return m_size; }
    T* toArray() const;
    SortListIterator<T> begin() const                  { return SortListIterator<T>(m_pHead); }
    SortListIterator<T> end() const                    { return SortListIterator<T>(m_pTail); }
    void setEqualBehaviour(const EqualBehaviour behaviour)      { m_equalBehaviour = behaviour; }

private:
    NodeCompareResult compare(const T& data, const ListNode<T>* const pNode) const;
    void insertBefore(const T &data, ListNode<T>* pNode);
    void insertEqual(const T& data, ListNode<T>* pNode);

private:
    ListNode<T>* m_pHead;
    ListNode<T>* m_pTail;
    int m_size;
    EqualBehaviour m_equalBehaviour;
};


/**
 * -------------------------------------------------------------------------------
 * Template class:      SortListIterator
 * -------------------------------------------------------------------------------
 * An iterator class to iterate through the SortList elements. It has a private
 * constructor and can not be instantiated manualy. The SortList methods begin()
 * and end() returns a instance of this class.
 * You can get a copy of the data object with data() and iterate with next() and
 * previous() through the SortList elements.
 * The returned iterator from list has the first element. Call next() or previous()
 * to get another element.
 * If the iterator reaches the end of the list it returns 'false' and keeps its
 * position.
 */
template<class T>
class SortListIterator
{
    friend class SortList<T>;

private:
    SortListIterator(ListNode<T>* pNode)            { m_pNode = pNode; }

public:
    /**
     * Positions the iterator to the next element in the SortList. If the end of list is reached
     * than the methode will return 'false' and keep its position.
     * @return      true if there is another object in the list. Flase if the end of list is reached.
     */
    bool next()                 { if (m_pNode == 0 || m_pNode->m_next == 0) return false; m_pNode = m_pNode->m_next; return true; }
    /**
     * Posiitions the iterator to a previous element in the SortList. If the end of the list is reached
     * the iterator will keep its position and returns 'false'.
     * @return      true if there is a previous element in the list. False if end of list is reached.
     */
    bool previous()             { if (m_pNode == 0 || m_pNode->m_previous == 0) return false; m_pNode = m_pNode->m_previous; return true; }
    /**
     * Get the data object of the ierators position.
     * @return      The data object of iterators position.
     */
    T data() const              { return m_pNode->m_data; }

private:
    ListNode<T>* m_pNode;
};

// ---------------------------------------------------------------------------------------------------------------
// Definition

/**
 * Removes all elements from teh list.
 * Delete all objects from the heap memory.
 */
template<class T>
void SortList<T>::clear()
{
    ListNode<T>* pNode = m_pHead;
    ListNode<T>* pTempNode = 0;
    while (pNode != 0) {
        pTempNode = pNode;
        pNode = pNode->m_next;
        delete pTempNode;
    }
    m_pHead = m_pTail = 0;
    m_size = 0;
}

/**
 * Compares the given data object with a data object of a list node. If data is lesser the the one
 * in the list node than Lesser (-1) will be returned. If both object are equal than compare()
 * returns Equal (0). Else Greater (1) is returned.
 * @param data          A new data object.
 * @param pNode         A list node object.
 * @return              Lesser if data is lesser. Equal if both are equal. Else Grater.
 */
template<class T>
typename SortList<T>::NodeCompareResult SortList<T>::compare(const T& data, const ListNode<T>* const pNode) const
{
    if (data < pNode->m_data) {
        return Lesser;
    }
    if (data == pNode->m_data) {
        return Equal;
    }

    return Greater;
}

/**
 * Inserts a new data object into the list. The element will be placed so that
 * the list is sorted ascending.
 * @param data          A data object.
 * @return position     The position where the new element is inserted.
 */
template<class T>
int SortList<T>::insert(const T& data)
{
    if (isEmpty()) {
        m_pHead = m_pTail = new ListNode<T>(data);
        m_size = 1;

        return 0;
    }
    int position = 0;
    ListNode<T>* pNode = m_pHead;
    while (pNode != 0) {
        NodeCompareResult compareResult = compare(data, pNode);
        switch (compareResult) {
        case Lesser:
            insertBefore(data, pNode);
            return position;
            break;
        case Equal:
            insertEqual(data, pNode);
            return position;
            break;
        case Greater:
            break;
        }
        ++position;
        pNode = pNode->m_next;
    }
    pNode = new ListNode<T>(m_pTail, data, 0);
    m_pTail->m_next = pNode;
    m_pTail = pNode;
    ++m_size;

    return m_size - 1;
}

/**
 * Insert the new element before the given list node.
 * @param data          The new data object to insert.
 * @param pNode         The list node where the new element should be placed before.
 */
template<class T>
void SortList<T>::insertBefore(const T& data, ListNode<T>* pNode)
{
    ListNode<T>* pNewNode = new ListNode<T>(pNode->m_previous, data, pNode);
    if (m_pHead == pNode) {
        m_pHead = pNewNode;
    } else {
        ListNode<T>* pPrevNode = pNode->m_previous;
        pPrevNode->m_next = pNewNode;
    }
    pNode->m_previous = pNewNode;
    ++m_size;
}

/**
 * Insert a equal element into the list. If a equal list element is found than
 * there are three posibilities to handle that. If 'InsertDoublicates' is set
 * as list behaviour then the new element is set above of the equal element.
 * If equal behaviour is set to 'DropDoublicates than the element won't be
 * set to the list.
 * The third method is 'UniteNodeData'. This will unite the content of the
 * equal elements. But it requires the << operator in the data object to be
 * overriden. The overriding method must handle the merge of the elements.
 * Important: To unite data elements you must '#define CanUniteData' before
 *            include this header file.
 * @param data
 * @param pNode
 */
template<class T>
void SortList<T>::insertEqual(const T& data, ListNode<T>* pNode)
{
    switch (m_equalBehaviour) {
    case UniteNodeData:
#ifdef CanUniteData
        pNode->m_data << data;
#endif
        break;
    case InsertDoublicate:
        insertBefore(data, pNode);
        break;
    case DropDoublicate:
        // Do not insert this data.
        break;
    }
}

/**
 * Returns the list elements in an array. The array is allocated in
 * the heap and MUST be deleted by the user.
 * @return array        An array with the sorted list elements.
 */
template<class T>
T* SortList<T>::toArray() const
{
    if (isEmpty()) {
        return 0;
    }
    T* array = new T[m_size];
    int position = 0;
    ListNode<T>* pNode = m_pHead;
    while (pNode != 0) {
        array[position] = pNode->m_data;
        pNode = pNode->m_next;
        ++position;
    }

    return array;
}

#endif // SORTLIST_H
